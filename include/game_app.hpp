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
    Tutorial,
    Menu,
    RunStart,
    LayerIntro,
    Dialogue,
    Combat,
    BoonChoice,
    Death,
    Loom,
    Manual,
    Pause,
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
    bool death_shook_  = false;

    std::string god_ally_;

    bool hint_shown(const std::string& key);
    void mark_hint_shown(const std::string& key);

    // Dialogue
    void begin_scene(const std::string& scene_id);
    void dialogue_advance();
    void dialogue_choose(int index);
    void dialogue_exit();

    void frame_boot();
    void frame_tutorial();
    void frame_menu();
    void frame_run_start();
    void frame_layer_intro();
    void frame_dialogue();
    void frame_combat();
    void frame_boon_choice();
    void frame_death();
    void frame_loom();
    void frame_manual();
    void frame_pause();
    void frame_help();

    void start_encounter_for_layer();
    void start_combat_phantom(int index);
    void start_combat_chiron();

    void transition_to(AppState next);
};