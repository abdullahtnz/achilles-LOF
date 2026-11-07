#pragma once
#include <optional>
#include <string>
#include "game_state.hpp"
#include "save_manager.hpp"

// Central game state — the only place game logic reads/writes save data.
// Every mutator persists to STARKDB immediately, so if the process is killed,
// the next launch resumes from the last completed action.
class GameContext {
public:
    GameContext();

    // ----- Lifecycle -----
    void NewRun();                 // wipe current run, write fresh RunState{layer=1}
    void EndRun();                 // clear current run; used after death or quit
    bool HasRun() const;

    // ----- Meta (persistent across runs) -----
    const MetaState& Meta() const { return meta_; }
    void AddKleos(uint32_t amount);            // persists
    void IncrementDeaths();                    // persists (also patroclus counter)
    void MarkEnding(Ending e);                 // records ending string; persists
    void AddFragment(const std::string& id);   // set insert; persists

    // ----- Current run -----
    const RunState& Run() const;               // throws if none
    RunState&       MutableRun();              // marks dirty; caller must call PersistRun()
    void PersistRun();

    // ----- Layer navigation helpers -----
    void AdvanceLayer();
    int  CurrentLayer() const;

    // Called from the state machine when the heel is struck.
    // Computes kleos from run_kleos, adds it to meta, increments death counters,
    // clears the run, persists.
    void OnHeelHit();

    // Whether the run was just ended by a heel hit or a normal completion.
    // Distinguishes the two endings; populated by OnHeelHit / AdvanceLayer.
    Ending LastEnding() const { return last_ending_; }

    SaveManager& Saves() { return saves_; }

private:
    SaveManager         saves_;
    MetaState           meta_;
    std::optional<RunState> run_;
    Ending              last_ending_ = Ending::None;

    void persist_meta();
};