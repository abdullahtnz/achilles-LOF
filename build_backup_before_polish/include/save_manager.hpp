#pragma once
#include <string>
#include "game_state.hpp"

// Numeric key layout (STARKDB shares one uint32 keyspace)
//   1 = meta     (JSON)
//   2 = run      (JSON; absent when no run is in progress)
//   3 = settings (JSON)
namespace keys {
    constexpr uint32_t META     = 1;
    constexpr uint32_t RUN      = 2;
    constexpr uint32_t SETTINGS = 3;
}

class SaveManager {
public:
    // Resolves save directory (creates ~/.achilles/ if missing) and opens <dir>/save.
    // Throws std::runtime_error with a human-readable message if opening fails
    // (locked by another instance, corrupt, etc.).
    explicit SaveManager(const std::string& save_dir = default_save_dir());
    ~SaveManager();

    SaveManager(const SaveManager&)            = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    // ----- Meta -----
    MetaState LoadMeta();               // returns defaults if absent
    void      SaveMeta(const MetaState&);

    // ----- Run (in-progress) -----
    bool      HasRun();
    RunState  LoadRun();                // throws if no run exists
    void      SaveRun(const RunState&);
    void      ClearRun();

    // ----- Settings -----
    Settings  LoadSettings();
    void      SaveSettings(const Settings&);

    // Back up the whole DB to <save_dir>/backup. Non-fatal on failure — logs.
    void      Backup();

    // Absolute path of the save directory (e.g. /home/you/.achilles)
    const std::string& SaveDir() const { return save_dir_; }

private:
    std::string save_dir_;
    void*       db_ = nullptr;          // opaque; hides stark::Database from header

    std::string ReadJson(uint32_t key, const std::string& fallback);
    void        WriteJson(uint32_t key, const std::string& payload);

    static std::string default_save_dir();
};