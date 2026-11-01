#pragma once
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "json.hpp"

// ----- Save version (bump when MetaState/RunState schema changes) -----
inline constexpr int SAVE_VERSION = 1;

// ----- Which ending the player has seen -----
enum class Ending { None, A_Kleos, B_Phantom, C_True };

// ----- Persistent meta-progression (survives every run) -----
struct MetaState {
    int         save_version        = SAVE_VERSION;
    uint64_t    kleos               = 0;      // currency
    uint32_t    total_deaths        = 0;
    uint32_t    total_runs          = 0;
    std::vector<std::string> unlocked_weapons { "spear" };
    std::vector<std::string> unlocked_boons;             // god path names
    std::set<std::string>    fragments_collected;        // fragment ids
    std::map<std::string,int> fates_flags;               // dialogue flags
    std::vector<std::string> endings_seen;               // "A","B","C"
    uint32_t    patroclus_death_count = 0;
    bool        first_launch_seen    = false;
};

// ----- Per-run state (cleared after each run) -----
struct RunState {
    int         layer              = 1;
    int         encounter          = 0;
    std::string path;                                    // "ares" | "athena" | ""
    std::vector<std::string> boons;
    bool        patroclus_alive    = false;
    int         heel_hits_taken    = 0;                  // always 0 or 1 per run (1 = dead)
    int         rage               = 0;
    uint32_t    run_kleos          = 0;
    std::set<std::string> fragments_this_run;
};

// ----- Settings -----
struct Settings {
    bool        first_launch_done = false;
};

// ----- JSON serialization (declared here, defined inline) -----
inline void to_json(nlohmann::json& j, const MetaState& m) {
    j = nlohmann::json{
        {"save_version", m.save_version},
        {"kleos", m.kleos},
        {"total_deaths", m.total_deaths},
        {"total_runs", m.total_runs},
        {"unlocked_weapons", m.unlocked_weapons},
        {"unlocked_boons", m.unlocked_boons},
        {"fragments_collected", m.fragments_collected},
        {"fates_flags", m.fates_flags},
        {"endings_seen", m.endings_seen},
        {"patroclus_death_count", m.patroclus_death_count},
        {"first_launch_seen", m.first_launch_seen},
    };
}

inline void from_json(const nlohmann::json& j, MetaState& m) {
    m.save_version          = j.value("save_version", SAVE_VERSION);
    m.kleos                 = j.value("kleos", 0ull);
    m.total_deaths          = j.value("total_deaths", 0u);
    m.total_runs            = j.value("total_runs", 0u);
    m.unlocked_weapons      = j.value("unlocked_weapons", std::vector<std::string>{"spear"});
    m.unlocked_boons        = j.value("unlocked_boons", std::vector<std::string>{});
    m.fragments_collected   = j.value("fragments_collected", std::set<std::string>{});
    m.fates_flags           = j.value("fates_flags", std::map<std::string,int>{});
    m.endings_seen          = j.value("endings_seen", std::vector<std::string>{});
    m.patroclus_death_count = j.value("patroclus_death_count", 0u);
    m.first_launch_seen     = j.value("first_launch_seen", false);
}

inline void to_json(nlohmann::json& j, const RunState& r) {
    j = nlohmann::json{
        {"layer", r.layer},
        {"encounter", r.encounter},
        {"path", r.path},
        {"boons", r.boons},
        {"patroclus_alive", r.patroclus_alive},
        {"heel_hits_taken", r.heel_hits_taken},
        {"rage", r.rage},
        {"run_kleos", r.run_kleos},
        {"fragments_this_run", r.fragments_this_run},
    };
}

inline void from_json(const nlohmann::json& j, RunState& r) {
    r.layer              = j.value("layer", 1);
    r.encounter          = j.value("encounter", 0);
    r.path               = j.value("path", std::string{});
    r.boons              = j.value("boons", std::vector<std::string>{});
    r.patroclus_alive    = j.value("patroclus_alive", false);
    r.heel_hits_taken    = j.value("heel_hits_taken", 0);
    r.rage               = j.value("rage", 0);
    r.run_kleos          = j.value("run_kleos", 0u);
    r.fragments_this_run = j.value("fragments_this_run", std::set<std::string>{});
}

inline void to_json(nlohmann::json& j, const Settings& s) {
    j = nlohmann::json{ {"first_launch_done", s.first_launch_done} };
}
inline void from_json(const nlohmann::json& j, Settings& s) {
    s.first_launch_done = j.value("first_launch_done", false);
}