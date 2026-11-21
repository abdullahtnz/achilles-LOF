#include "game_app.hpp"
#include "dialogue.hpp"
#include "screens.hpp"
#include "story.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>

GameApp::GameApp() {
    // Validate story registry at startup
    (void)story::scene_count();
}

int GameApp::Run() {
    last_frame_ = ui::now();
    state_entered_at_ = ui::now();
    while (state_ != AppState::Quit) {
        auto t0 = ui::now();
        long long dt = ui::ms_since(last_frame_);
        last_frame_ = t0;

        ui::clear();

        if (state_ == AppState::Combat && combat_ && !paused_) {
            combat_->tick(dt);
        }

        switch (state_) {
            case AppState::Boot:       frame_boot();        break;
            case AppState::Tutorial:   frame_tutorial();    break;
            case AppState::Menu:       frame_menu();        break;
            case AppState::RunStart:   frame_run_start();   break;
            case AppState::LayerIntro: frame_layer_intro(); break;
            case AppState::Dialogue:   frame_dialogue();    break;
            case AppState::Combat:     frame_combat();      break;
            case AppState::BoonChoice: frame_boon_choice(); break;
            case AppState::Death:      frame_death();       break;
            case AppState::Ending:     frame_ending();      break;
            case AppState::Loom:       frame_loom();        break;
            case AppState::Manual:     frame_manual();      break;
            case AppState::Pause:      frame_pause();       break;
            case AppState::Help:       frame_help();        break;
            case AppState::Quit:       break;
        }

        ui::refresh();

        long long used = ui::ms_since(t0);
        if (used < 16) ui::sleep_ms((int)(16 - used));
    }
    return 0;
}

void GameApp::transition_to(AppState next) {
    ui::fade_out(180);
    state_ = next;
    state_entered_at_ = ui::now();
    ui::fade_in(180);
}

bool GameApp::hint_shown(const std::string& key) {
    return ctx_.Meta().fates_flags.count(key) > 0;
}

void GameApp::mark_hint_shown(const std::string& key) {
    ctx_.SetFlag(key, 1);
}

// ----------------------------------------------------------------
// Dialogue
// ----------------------------------------------------------------

void GameApp::begin_scene(const std::string& scene_id) {
    const Scene* scene = find_scene(scene_id);
    if (!scene) {
        // Fallback: unknown scene → menu. Story data validation.
        transition_to(AppState::Menu);
        return;
    }
    auto& run = ctx_.MutableRun();
    run.dialogue.scene_id       = scene_id;
    run.dialogue.line_index     = 0;
    run.dialogue.pending_choice = false;
    // Don't clear choice_path — that's the historical record.

    // Chapter id tracks which story chapter the run is currently in.
    if (scene_id.size() > 2 && scene_id[0] == 'c' && scene_id[1] == 'h') {
        int n = std::atoi(scene_id.c_str() + 2);
        if (n >= 1 && n <= 7) {
            run.chapter_id = "chapter_" + std::to_string(n);
        }
    }

    // Patroclus, if left behind, dies at Troy while you are at the wall.
    if (scene_id == "ch5_arrival" && !run.patroclus_alive) {
        run.patroclus_dead_this_run = true;
    }

    grant_scene_fragment(scene_id);
    ctx_.PersistRun();
    transition_to(AppState::Dialogue);
}

void GameApp::grant_scene_fragment(const std::string& id) {
    struct Entry { const char* scene; const char* fragment; };
    static const Entry entries[] = {
        {"ch1_outro",    "frag_chiron"},
        {"ch2_post_boss","frag_deidamia"},
        {"ch3_outro",    "frag_patroclus"},
        {"ch4_outro",    "frag_scamander"},
        {"ch5_outro",    "frag_hector"},
        {"ch6_outro",    "frag_moirai"},
    };
    for (const auto& e : entries) {
        if (id == e.scene) {
            auto& run = ctx_.MutableRun();
            if (run.fragments_this_run.insert(e.fragment).second) {
                ctx_.AddFragment(e.fragment);
            }
            return;
        }
    }
}

bool GameApp::is_combat_scene(const std::string& id, int& layer, int& encounter) const {
    if (id.rfind("ch", 0) != 0) return false;
    size_t us = id.find('_');
    if (us == std::string::npos) return false;

    int n = std::atoi(id.substr(2, us - 2).c_str());
    if (n < 1 || n > 7) return false;
    std::string rest = id.substr(us + 1);

    if (rest == "pre_boss") {
        layer = n;
        encounter = 3;
        return true;
    }
    if (rest.rfind("combat_", 0) == 0) {
        int k = std::atoi(rest.c_str() + 7);
        if (k >= 1 && k <= 3) {
            layer = n;
            encounter = k - 1;
            return true;
        }
    }
    return false;
}

bool GameApp::is_outro_scene(const std::string& id, int& chapter) const {
    if (id == "ch2_post_boss") {
        chapter = 2;
        return true;
    }
    if (id.rfind("ch", 0) != 0) return false;
    size_t us = id.find('_');
    if (us == std::string::npos) return false;
    std::string rest = id.substr(us + 1);
    if (rest != "outro") return false;
    int n = std::atoi(id.substr(2, us - 2).c_str());
    if (n < 1 || n > 7) return false;
    chapter = n;
    return true;
}

std::string GameApp::arrival_scene_for(int layer) const {
    if (layer < 1 || layer > 7) return "";
    return "ch" + std::to_string(layer) + "_arrival";
}

std::string GameApp::boss_outro_scene_for(int layer) const {
    if (layer == 2) return "ch2_post_boss";
    if (layer < 1 || layer > 7) return "";
    return "ch" + std::to_string(layer) + "_outro";
}

void GameApp::dialogue_advance() {
    auto& run = ctx_.MutableRun();
    const Scene* scene = find_scene(run.dialogue.scene_id);
    if (!scene) { transition_to(AppState::Menu); return; }

    if (run.dialogue.pending_choice) return;   // handled by choice keys

    if (!advance_line(run.dialogue, *scene)) {
        // Scene ended
        if (scene->choices.empty()) {
            if (!scene->next_scene_id.empty()) {
                begin_scene(scene->next_scene_id);
            } else {
                dialogue_exit();
            }
            return;
        }
        // Choices pending — nothing more to do, waiting on player
    }
    ctx_.PersistRun();
}

void GameApp::dialogue_choose(int index) {
    auto& run = ctx_.MutableRun();
    const Scene* scene = find_scene(run.dialogue.scene_id);
    if (!scene) return;
    if (index < 0 || index >= (int)scene->choices.size()) return;

    // Story choices with mechanical consequences.
    const std::string cur = run.dialogue.scene_id;
    if (cur == "ch2_mid_1" && index == 0) {
        // Odysseus's bronze token: one heel guard.
        run.heel_guards += 1;
    }
    if (cur == "ch3_patroclus_intro") {
        run.patroclus_alive = (index == 0);
        if (index == 0) run.patroclus_saved = false;
    }
    if (cur == "ch3_mid_1") {
        run.patroclus_saved = (index == 0);
    }

    std::string next = apply_choice(run.dialogue, *scene, index);
    ctx_.PersistRun();
    if (next.empty()) {
        dialogue_exit();
    } else {
        begin_scene(next);
    }
}

void GameApp::dialogue_exit() {
    auto& run = ctx_.MutableRun();
    const std::string scene_id = run.dialogue.scene_id;

    // Endings resolve the run.
    if (scene_id == "ch7_end_a") {
        ctx_.CompleteRun(Ending::A_Kleos);
        transition_to(AppState::Ending);
        return;
    }
    if (scene_id == "ch7_end_c") {
        ctx_.CompleteRun(Ending::C_True);
        transition_to(AppState::Ending);
        return;
    }

    // Any combat scene: set the encounter and begin the fight.
    int combat_layer = 0, encounter = 0;
    if (is_combat_scene(scene_id, combat_layer, encounter)) {
        run.layer = combat_layer;
        run.encounter = encounter;
        run.in_combat = true;
        ctx_.PersistRun();
        start_encounter_for_layer();
        return;
    }

    // Chapter outro: advance the layer and begin the next chapter.
    int chapter = 0;
    if (is_outro_scene(scene_id, chapter)) {
        if (chapter == 2) {
            // Skyros ends at the god-choice screen.
            ctx_.AdvanceLayer();
            boon_pending_ = true;
            transition_to(AppState::BoonChoice);
            return;
        }
        ctx_.AdvanceLayer();
        if (!ctx_.HasRun()) {
            transition_to(AppState::Menu);
            return;
        }
        int next_layer = ctx_.CurrentLayer();
        std::string next_scene = arrival_scene_for(next_layer);
        if (!next_scene.empty() && find_scene(next_scene)) {
            begin_scene(next_scene);
        } else {
            transition_to(AppState::LayerIntro);
        }
        return;
    }

    // Default: intro/mid scene with no follow-up → layer intro.
    transition_to(AppState::LayerIntro);
}

// ----------------------------------------------------------------
// Combat setup
// ----------------------------------------------------------------

void GameApp::start_combat_phantom(int index) {
    (void)index;
    start_encounter_for_layer();
}

void GameApp::start_combat_chiron() {
    start_encounter_for_layer();
}

void GameApp::start_encounter_for_layer() {
    int layer = ctx_.CurrentLayer();
    int enc   = ctx_.Run().encounter;
    god_ally_ = ctx_.Run().path;

    auto make_player = [&]() {
        Combatant player;
        player.name = (god_ally_ == "athena") ? "Achilles (Athena)" : "Achilles";
        player.weapon      = ctx_.Run().weapon;
        player.hp_max      = player.hp = 100;
        player.stamina_max = 3 + (player.weapon == "shield" ? 1 : 0);
        player.stamina     = player.stamina_max;
        player.heel_guards = ctx_.Run().heel_guards;
        return player;
    };

    Patroclus pat;
    if (layer >= 3) {
        pat.active       = true;
        pat.alive        = ctx_.Run().patroclus_alive &&
                           !ctx_.Run().patroclus_dead_this_run;
        pat.used_support = false;
    }

    Combatant player = make_player();
    Combatant enemy;
    switch (layer) {
        case 1: enemy = (enc >= 3) ? make_l1_chiron()      : make_l1_phantom(enc); break;
        case 2: enemy = (enc >= 3) ? make_l2_deidamia()    : make_l2_illusion(enc); break;
        case 3: enemy = (enc >= 3) ? make_l3_hector_vanguard() : make_l3_trojan(enc); break;
        case 4: enemy = (enc >= 3) ? make_l4_scamander()   : make_l4_river(enc); break;
        case 5: enemy = (enc >= 3) ? make_l5_hector()      : make_l5_wall(enc); break;
        case 6: enemy = (enc >= 3) ? make_l6_moirai()      : make_l6_phantom(enc); break;
        case 7: enemy = (enc >= 3) ? make_l7_apollo_chosen(): make_l7_gate(enc); break;
        default:
            enemy = make_l1_phantom(enc);
            break;
    }

    // The Nameless Thread: the loom's hard mode.
    auto hm = ctx_.Meta().fates_flags.find("hard_mode_active");
    if (hm != ctx_.Meta().fates_flags.end() && hm->second > 0) {
        enemy.hp_max = (int)(enemy.hp_max * 1.3);
        enemy.hp     = enemy.hp_max;
        for (auto& atk : enemy.attacks) {
            atk.coil_ms = std::max(900, atk.coil_ms - 150);
        }
    }

    ctx_.MutableRun().in_combat = true;
    ctx_.PersistRun();

    combat_ = std::make_unique<CombatSession>(player, enemy, pat, god_ally_);
    combat_->set_unlocked_actions(layer);
    transition_to(AppState::Combat);
}