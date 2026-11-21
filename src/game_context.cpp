#include "game_context.hpp"

#include <stdexcept>

GameContext::GameContext() : saves_() {
    meta_ = saves_.LoadMeta();
    if (saves_.HasRun()) {
        try {
            run_ = saves_.LoadRun();
        } catch (...) {
            // Corrupt run blob — treat as no run in progress, drop it.
            saves_.ClearRun();
            run_.reset();
        }
    }
}

bool GameContext::HasRun() const { return run_.has_value(); }

const RunState& GameContext::Run() const {
    if (!run_) throw std::runtime_error("GameContext::Run: no run in progress");
    return *run_;
}

RunState& GameContext::MutableRun() {
    if (!run_) throw std::runtime_error("GameContext::MutableRun: no run in progress");
    return *run_;
}

void GameContext::PersistRun() {
    if (run_) saves_.SaveRun(*run_);
}

void GameContext::NewRun() {
    run_ = RunState{};
    run_->layer = 1;
    run_->encounter = 0;
    run_->run_kleos = 0;

    // Carry the most recently unlocked weapon into the new thread.
    if (!meta_.unlocked_weapons.empty()) {
        run_->weapon = meta_.unlocked_weapons.back();
    }
    // Thetis tokens are consumed at the start of a run.
    auto tok = meta_.fates_flags.find("thetis_tokens");
    if (tok != meta_.fates_flags.end() && tok->second > 0) {
        run_->heel_guards = tok->second;
        tok->second = 0;
    }

    meta_.total_runs += 1;
    persist_meta();
    saves_.SaveRun(*run_);
    last_ending_ = Ending::None;
}

void GameContext::EndRun() {
    run_.reset();
    saves_.ClearRun();
}

void GameContext::AddKleos(uint32_t amount) {
    meta_.kleos += amount;
    persist_meta();
}

void GameContext::IncrementDeaths() {
    meta_.total_deaths += 1;
    if (run_ && !run_->patroclus_alive) {
        // Patroclus died this run — tally it separately.
        meta_.patroclus_death_count += 1;
    }
    persist_meta();
}

void GameContext::MarkEnding(Ending e) {
    const char* tag = nullptr;
    switch (e) {
        case Ending::A_Kleos:  tag = "A"; break;
        case Ending::B_Phantom: tag = "B"; break;
        case Ending::C_True:   tag = "C"; break;
        default: return;
    }
    bool present = false;
    for (const auto& s : meta_.endings_seen) if (s == tag) { present = true; break; }
    if (!present) {
        meta_.endings_seen.push_back(tag);
    }
    if (e == Ending::B_Phantom) {
        meta_.fates_flags["hard_mode_unlocked"] = 1;
    }
    persist_meta();
}

void GameContext::AddFragment(const std::string& id) {
    meta_.fragments_collected.insert(id);
    persist_meta();
}

bool GameContext::AllFragmentsCollected() const {
    return (int)meta_.fragments_collected.size() >= FRAGMENT_TOTAL;
}

void GameContext::SetFlag(const std::string& key, int value) {
    meta_.fates_flags[key] = value;
    persist_meta();
}

bool GameContext::SpendKleos(uint32_t amount) {
    if (meta_.kleos < amount) return false;
    meta_.kleos -= amount;
    persist_meta();
    return true;
}

void GameContext::UnlockWeapon(const std::string& weapon) {
    for (const auto& w : meta_.unlocked_weapons) {
        if (w == weapon) return;
    }
    meta_.unlocked_weapons.push_back(weapon);
    persist_meta();
}

void GameContext::AddThetisToken(int count) {
    meta_.fates_flags["thetis_tokens"] += count;
    persist_meta();
}

void GameContext::CompleteRun(Ending e) {
    if (!run_) return;
    meta_.kleos += run_->run_kleos;
    last_ending_ = e;
    MarkEnding(e);
    persist_meta();
    run_.reset();
    saves_.ClearRun();
}

void GameContext::AdvanceLayer() {
    if (!run_) return;
    run_->layer += 1;
    run_->encounter = 0;
    if (run_->layer > 7) {
        // Cleared the final layer — treat as a (placeholder) victory.
        last_ending_ = Ending::A_Kleos;
        MarkEnding(last_ending_);
        AddKleos(run_->run_kleos);
        run_.reset();
        saves_.ClearRun();
        return;
    }
    saves_.SaveRun(*run_);
}

int GameContext::CurrentLayer() const {
    return run_ ? run_->layer : 0;
}

void GameContext::OnHeelHit() {
    if (!run_) return;
    // Award whatever kleos the player earned this run.
    uint32_t earned = run_->run_kleos;
    meta_.kleos += earned;
    meta_.total_deaths += 1;
    if (run_->patroclus_dead_this_run) {
        meta_.patroclus_death_count += 1;
    }

    // Ending B — the Phantom. A thread consumed by rage, repeated until it
    // becomes one of the past deaths that hunts future Achilles.
    Ending e = Ending::A_Kleos;
    const uint32_t deaths_before = meta_.total_deaths - 1;
    const bool consumed_by_rage = (run_->path == "ares");
    const bool grief_unresolved = run_->patroclus_dead_this_run ||
                                  meta_.patroclus_death_count >= 2;
    if (consumed_by_rage && grief_unresolved && deaths_before >= 3) {
        e = Ending::B_Phantom;
    }

    last_ending_ = e;
    MarkEnding(last_ending_);
    persist_meta();

    run_.reset();
    saves_.ClearRun();
}

void GameContext::persist_meta() {
    saves_.SaveMeta(meta_);
}