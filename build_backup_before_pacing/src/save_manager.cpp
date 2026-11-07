#include "save_manager.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <stark.hpp>
#include <sys/stat.h>

namespace fs = std::filesystem;

// ---------- construction / teardown ----------

std::string SaveManager::default_save_dir() {
    const char* home = std::getenv("HOME");
    if (!home || !*home) {
        throw std::runtime_error(
            "HOME is not set; cannot determine save directory");
    }
    return std::string(home) + "/.achilles";
}

SaveManager::SaveManager(const std::string& save_dir) : save_dir_(save_dir) {
    // 1. Ensure save directory exists.
    std::error_code ec;
    fs::create_directories(save_dir_, ec);
    if (ec) {
        throw std::runtime_error(
            "Cannot create save directory '" + save_dir_ + "': " + ec.message());
    }

    // 2. Open the database.
    const std::string prefix = save_dir_ + "/save";
    try {
        db_ = new stark::Database(prefix);
    } catch (const stark::Error& e) {
        throw std::runtime_error(
            "Cannot open database at '" + prefix + "': " + e.what() +
            "  (is another instance of Achilles running?)");
    }
}

SaveManager::~SaveManager() {
    if (db_) {
        auto* d = static_cast<stark::Database*>(db_);
        try { d->sync(); } catch (...) { /* ignore on teardown */ }
        delete d;
        db_ = nullptr;
    }
}

// ---------- raw JSON (with NUL stripping + transactions) ----------

std::string SaveManager::ReadJson(uint32_t key, const std::string& fallback) {
    auto* d = static_cast<stark::Database*>(db_);
    std::string raw = d->get(key);           // "" if absent
    // C++ binding writes value.size()+1 → strip trailing NUL(s).
    while (!raw.empty() && raw.back() == '\0') raw.pop_back();
    return raw.empty() ? fallback : raw;
}

void SaveManager::WriteJson(uint32_t key, const std::string& payload) {
    auto* d = static_cast<stark::Database*>(db_);
    if (payload.empty()) {
        // STARKDB rejects empty values (STARK_INVALID_ARG). Never write "".
        throw std::runtime_error("SaveManager::WriteJson: refusing empty payload");
    }
    try {
        d->begin();
        d->add(key, payload);                // add == insert-or-update
        d->commit();
    } catch (const stark::Error& e) {
        try { d->rollback(); } catch (...) {}
        throw std::runtime_error(
            std::string("Write failed for key ") + std::to_string(key) +
            ": " + e.what());
    }
}

// ---------- meta ----------

MetaState SaveManager::LoadMeta() {
    std::string raw = ReadJson(keys::META, "");
    if (raw.empty()) return MetaState{};     // first launch: defaults
    try {
        return nlohmann::json::parse(raw).get<MetaState>();
    } catch (const std::exception& e) {
        std::cerr << "[save] Meta parse failed (" << e.what()
                  << "); starting fresh.\n";
        return MetaState{};
    }
}

void SaveManager::SaveMeta(const MetaState& m) {
    nlohmann::json j = m;
    WriteJson(keys::META, j.dump());
    Backup();                                // rolling snapshot after each save
}

// ---------- run ----------

bool SaveManager::HasRun() {
    auto* d = static_cast<stark::Database*>(db_);
    return d->exists(keys::RUN);
}

RunState SaveManager::LoadRun() {
    std::string raw = ReadJson(keys::RUN, "");
    if (raw.empty()) {
        throw std::runtime_error("LoadRun: no run in progress");
    }
    return nlohmann::json::parse(raw).get<RunState>();
}

void SaveManager::SaveRun(const RunState& r) {
    nlohmann::json j = r;
    WriteJson(keys::RUN, j.dump());
}

void SaveManager::ClearRun() {
    auto* d = static_cast<stark::Database*>(db_);
    try {
        d->begin();
        d->remove(keys::RUN);
        d->commit();
    } catch (const stark::Error& e) {
        try { d->rollback(); } catch (...) {}
        throw std::runtime_error(std::string("ClearRun failed: ") + e.what());
    }
}

// ---------- settings ----------

Settings SaveManager::LoadSettings() {
    std::string raw = ReadJson(keys::SETTINGS, "");
    if (raw.empty()) return Settings{};
    try {
        return nlohmann::json::parse(raw).get<Settings>();
    } catch (...) {
        return Settings{};
    }
}

void SaveManager::SaveSettings(const Settings& s) {
    nlohmann::json j = s;
    WriteJson(keys::SETTINGS, j.dump());
}

// ---------- backup ----------

void SaveManager::Backup() {
    auto* d = static_cast<stark::Database*>(db_);
    const std::string dest = save_dir_ + "/backup";
    try {
        d->backup(dest);
    } catch (const stark::Error& e) {
        // Non-fatal — the primary save already succeeded.
        std::cerr << "[save] backup failed: " << e.what() << "\n";
    }
}