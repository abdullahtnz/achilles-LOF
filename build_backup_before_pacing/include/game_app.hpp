#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "combat.hpp"
#include "enemies.hpp"
#include "game_context.hpp"
#include "ui.hpp"

enum class AppState {
    Boot,
    Menu,
    RunStart,
    LayerIntro,
    Combat,
    BoonChoice,
    Death,
    Loom,
    Help,
    Quit,
};

class GameApp {
public:
    GameApp();
    int Run();

private:
    GameContext  ctx_;
    AppState     state_      = AppState::Boot;
    AppState     return_to_  = AppState::Menu;

    std::unique_ptr<CombatSession> combat_;

    ui::Clock::time_point last_frame_;
    ui::Clock::time_point state_entered_at_;

    bool boon_pending_ = false;
    bool paused_       = false;

    // NEW: seed for god intervention per encounter
    std::string god_ally_;   // "ares" | "athena" | ""

    void frame_boot();
    void frame_menu();
    void frame_run_start();
    void frame_layer_intro();
    void frame_combat();
    void frame_boon_choice();
    void frame_death();
    void frame_loom();
    void frame_help();

    void start_encounter_for_layer();
    void start_combat_phantom(int index);
    void start_combat_chiron();

    void transition_to(AppState next);
};