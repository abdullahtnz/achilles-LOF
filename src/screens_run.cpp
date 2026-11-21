#include "game_app.hpp"
#include "dialogue.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <algorithm>
#include <string>
#include <vector>

namespace screens {
    const char* layer_name(int layer) {
        static const char* names[] = {
            "", "PHTHIA", "SKYROS", "THE SHORES OF TROY",
            "THE PLAIN OF SCAMANDER", "THE WALLS OF TROY",
            "THE GATE OF HORN", "THE SCAEAN GATE",
        };
        if (layer < 1 || layer > 7) return "?";
        return names[layer];
    }

    ui::Color layer_accent(int layer) {
        switch (layer) {
            case 1: return ui::ACCENT;
            case 2: return ui::INFO;
            case 3: return ui::ACCENT;
            case 4: return ui::SAFE;
            case 5: return ui::DANGER;
            case 6: return ui::HIGHLIGHT;
            case 7: return ui::WARNING;
            default:return ui::DEFAULT;
        }
    }
}

// ====================================================================
// MENU
// ====================================================================
void GameApp::frame_menu() {
    const int R = ui::rows();
    const int C = ui::cols();
    const int w = std::min(64, C - 4);
    const int h = std::min(22, R - 2);
    const int y = (R - h) / 2;
    const int x = (C - w) / 2;

    ui::draw_box(y, x, h, w, " ACHILLES \u00B7 THE LOOM OF FATE ", {ui::BORDER});

    ui::draw_center(y + 2, "kleos", {ui::DIM});
    ui::draw_center(y + 3, std::to_string(ctx_.Meta().kleos), {ui::ACCENT, true});
    ui::draw_center(y + 5, "deaths   " + std::to_string(ctx_.Meta().total_deaths),
                    {ui::DIM});
    ui::draw_center(y + 6, "runs     " + std::to_string(ctx_.Meta().total_runs),
                    {ui::DIM});
    ui::draw_center(y + 7,
        "fragments " + std::to_string((int)ctx_.Meta().fragments_collected.size()) +
        "/" + std::to_string(FRAGMENT_TOTAL),
        {ui::DIM});

    struct Item { std::string label; int key; };
    std::vector<Item> items;
    if (ctx_.HasRun()) {
        items.push_back({"CONTINUE RUN \u2014 LAYER " +
                          std::to_string(ctx_.CurrentLayer()), '1'});
        items.push_back({"ABANDON AND START NEW", '2'});
    } else {
        items.push_back({"NEW RUN", '1'});
    }
    items.push_back({"ENTER THE LOOM", '3'});
    items.push_back({"QUIT", '4'});
    if (ctx_.Meta().fates_flags.count("hard_mode_unlocked") > 0) {
        bool on = ctx_.Meta().fates_flags.count("hard_mode_active") > 0 &&
                  ctx_.Meta().fates_flags.at("hard_mode_active") > 0;
        items.push_back({std::string("THE NAMELESS THREAD — HARD MODE: ") +
                         (on ? "ON" : "OFF"), '5'});
    }

    int row = y + 9;
    for (const auto& it : items) {
        std::string line = "[" + std::string(1, (char)it.key) + "]  " + it.label;
        ui::draw_center(row, line, {ui::INFO, true});
        row += 2;
    }

    ui::draw_center(y + h - 2, "press a number to choose   \u00B7   [?] help",
                    {ui::DIM});

    int k = ui::get_key_ms(200);
    if (k == '1') {
        if (ctx_.HasRun()) {
            const auto& run = ctx_.Run();
            if (run.in_combat) {
                transition_to(AppState::LayerIntro);
            } else if (!run.boon_chosen && run.layer == 3 &&
                       run.chapter_id == "chapter_2") {
                transition_to(AppState::BoonChoice);
            } else if (!run.dialogue.scene_id.empty() &&
                       find_scene(run.dialogue.scene_id)) {
                // Resume the exact line / choice we left off at.
                transition_to(AppState::Dialogue);
            } else {
                // Legacy save or a run that has no scene yet: start at the
                // beginning of the chapter rather than skipping its story.
                std::string opening = (run.layer == 1)
                    ? "ch1_birth"
                    : arrival_scene_for(run.layer);
                if (find_scene(opening)) {
                    begin_scene(opening);
                } else {
                    transition_to(AppState::LayerIntro);
                }
            }
            return;
        }
        ctx_.NewRun();
        transition_to(AppState::RunStart);
        return;
    }
    if (k == '2' && ctx_.HasRun()) {
        ctx_.EndRun();
        ctx_.NewRun();
        transition_to(AppState::RunStart);
        return;
    }
    if (k == '3') { transition_to(AppState::Loom); return; }
    if (k == '4') { transition_to(AppState::Quit); return; }
    if (k == '5' && ctx_.Meta().fates_flags.count("hard_mode_unlocked") > 0) {
        int on = 0;
        auto it = ctx_.Meta().fates_flags.find("hard_mode_active");
        if (it != ctx_.Meta().fates_flags.end()) on = it->second;
        ctx_.SetFlag("hard_mode_active", on ? 0 : 1);
        return;
    }
    if (k == '?') { return_to_ = AppState::Menu; transition_to(AppState::Manual); return; }
}

// ====================================================================
// RUN START
// ====================================================================
void GameApp::frame_run_start() {
    const int R = ui::rows();
    const int C = ui::cols();
    long long elapsed = ui::ms_since(state_entered_at_);

    if (elapsed < 1200) {
        ui::draw_center(R / 2 - 3, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        ui::draw_center(R / 2 - 5, "THE THREAD BEGINS", {ui::ACCENT, true});
        int w = std::min(60, C - 10);
        ui::draw_wrapped(R / 2 - 2, (C - w) / 2, w,
            "You stand before the Loom. The three sisters watch.\n\n"
            "One speaks: \"You have chosen the short, bright thread. "
            "Very well. We will weave it \u2014 again and again \u2014 "
            "until you understand what you have chosen.\"",
            {ui::DEFAULT});
    }

    ui::draw_center(R - 4, "press ENTER to walk to Phthia", {ui::DIM, true});

        int k = ui::get_key_ms(16);
    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        // NEW: begin the chapter intro dialogue before LayerIntro.
        auto& run = ctx_.MutableRun();
        run.chapter_id = "chapter_1_phthia";
        run.checkpoint_scene = "ch1_intro_thetis";
        ctx_.PersistRun();
        begin_scene("ch1_birth");
    }
}

// ====================================================================
// LAYER INTRO
// ====================================================================
void GameApp::frame_layer_intro() {
    const int R = ui::rows();
    const int C = ui::cols();
    long long elapsed = ui::ms_since(state_entered_at_);

    int layer = ctx_.CurrentLayer();
    const char* name = screens::layer_name(layer);
    ui::Color accent = screens::layer_accent(layer);

    if (elapsed < 800) {
        ui::draw_center(R / 2, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        std::string title = std::string("LAYER ") +
                             std::to_string(layer) + " \u2014 " + name;
        ui::draw_center(R / 2 - 3, title, {accent, true});
        std::string underline;
        for (size_t i = 0; i < title.size(); ++i) underline += "\u2500";
        ui::draw_center(R / 2 - 2, underline, {accent});

        static const char* flavor[] = {
            "",
            "the training grounds \u2014 the boy becomes the weapon",
            "the court of women \u2014 the sword chosen over the spindle",
            "the war camp \u2014 the host gathers",
            "the river plain \u2014 wrath becomes slaughter",
            "the walls of troy \u2014 the greatest duel",
            "the loom itself \u2014 the prophecy comes due",
            "the gate at dusk \u2014 paris's arrow finds its mark",
        };
        if (layer >= 1 && layer <= 7) {
            ui::draw_center(R / 2, flavor[layer], {ui::INFO});
        }
    }

    ui::draw_center(R - 4, "press ENTER to proceed", {ui::DIM, true});

    int k = ui::get_key_ms(16);
    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        start_encounter_for_layer();
    }
}