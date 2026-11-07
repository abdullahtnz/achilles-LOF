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
        persist_meta();
    }
}

void GameContext::AddFragment(const std::string& id) {
    meta_.fragments_collected.insert(id);
    persist_meta();
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
    if (!run_->patroclus_alive) {
        meta_.patroclus_death_count += 1;
    }
    // For now, all deaths are Ending A. Later: detect Ending B via rage/ares thresholds.
    last_ending_ = Ending::A_Kleos;
    MarkEnding(last_ending_);
    persist_meta();

    run_.reset();
    saves_.ClearRun();
}

void GameContext::persist_meta() {
    saves_.SaveMeta(meta_);
}