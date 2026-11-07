#include "game_app.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <memory>
#include <string>

GameApp::GameApp() = default;

int GameApp::Run() {
    last_frame_ = ui::now();
    while (state_ != AppState::Quit) {
        auto t0 = ui::now();
        long long dt = ui::ms_since(last_frame_);
        last_frame_ = t0;

        ui::clear();

        if (state_ == AppState::Combat && combat_) {
            combat_->tick(dt);
        }

        switch (state_) {
            case AppState::Menu:        frame_menu();        break;
            case AppState::RunStart:    frame_run_start();   break;
            case AppState::LayerIntro:  frame_layer_intro(); break;
            case AppState::Combat:      frame_combat();      break;
            case AppState::BoonChoice:  frame_boon_choice(); break;
            case AppState::Death:       frame_death();       break;
            case AppState::Loom:        frame_loom();        break;
            case AppState::Quit:        break;
        }

        ui::refresh();

        long long used = ui::ms_since(t0);
        if (used < 16) napms((int)(16 - used));
    }
    return 0;
}

// ----------------------------------------------------------------
// Combat setup helpers
// ----------------------------------------------------------------

void GameApp::start_combat_phantom(int index) {
    Combatant player;
    player.name        = "Achilles";
    player.hp_max      = player.hp = 100;
    player.stamina_max = 3;
    player.stamina     = 3;

    Combatant enemy = make_l1_phantom(index);
    combat_ = std::make_unique<CombatSession>(player, enemy, Patroclus{});
    state_ = AppState::Combat;
}

void GameApp::start_combat_chiron() {
    Combatant player;
    player.name        = "Achilles";
    player.hp_max      = player.hp = 100;
    player.stamina_max = 3;
    player.stamina     = 3;

    Combatant boss = make_l1_chiron();
    combat_ = std::make_unique<CombatSession>(player, boss, Patroclus{});
    state_ = AppState::Combat;
}

void GameApp::start_encounter_for_layer() {
    int layer = ctx_.CurrentLayer();
    int enc   = ctx_.Run().encounter;

    // ---- Layer 1 ----
    if (layer == 1) {
        if (enc >= 3) start_combat_chiron();
        else          start_combat_phantom(enc);
        return;
    }

    // ---- Layer 2 ----
    if (layer == 2) {
        Combatant player;
        player.name        = "Achilles";
        player.hp_max      = player.hp = 100;
        player.stamina_max = 3;
        player.stamina     = 3;

        if (enc >= 3) {
            Combatant boss = make_l2_deidamia();
            combat_ = std::make_unique<CombatSession>(player, boss, Patroclus{});
        } else {
            Combatant e = make_l2_illusion(enc);
            combat_ = std::make_unique<CombatSession>(player, e, Patroclus{});
        }
        state_ = AppState::Combat;
        return;
    }

    // ---- Layer 3 ----
    if (layer == 3) {
        Patroclus pat;
        pat.active       = true;
        pat.alive        = ctx_.Run().patroclus_alive &&
                           !ctx_.Run().patroclus_dead_this_run;
        pat.used_support = false;

        Combatant player;
        player.name        = (ctx_.Run().path == "athena")
                              ? "Achilles (Athena)"
                              : "Achilles";
        player.hp_max      = player.hp = 100;
        player.stamina_max = 3;
        player.stamina     = 3;

        if (enc >= 3) {
            Combatant boss = make_l3_hector_vanguard();
            combat_ = std::make_unique<CombatSession>(player, boss, pat);
        } else {
            Combatant e = make_l3_trojan(enc);
            combat_ = std::make_unique<CombatSession>(player, e, pat);
        }
        state_ = AppState::Combat;
        return;
    }

    // ---- Layers 4-7 stubs ----
    ctx_.MutableRun().run_kleos += 20;
    ctx_.PersistRun();
    ctx_.AdvanceLayer();
    state_ = ctx_.HasRun() ? AppState::LayerIntro : AppState::Menu;
}