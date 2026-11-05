#include "game_app.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>
#include <vector>

// Layer names and their accent colors — used here and elsewhere.
// Exposed in screens.hpp.
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
            case 1: return ui::ACCENT;       // warm gold
            case 2: return ui::INFO;         // marble cyan
            case 3: return ui::ACCENT;       // bronze
            case 4: return ui::SAFE;         // river teal-ish (green in ncurses)
            case 5: return ui::DANGER;       // war red
            case 6: return ui::HIGHLIGHT;    // thread violet
            case 7: return ui::WARNING;      // dusk gold
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
    const int h = std::min(18, R - 2);
    const int y = (R - h) / 2;
    const int x = (C - w) / 2;

    ui::draw_box(y, x, h, w, " ACHILLES \u00B7 THE LOOM OF FATE ", {ui::BORDER});
    ui::draw_center(y + 2, "kleos   deaths   runs", {ui::DIM});
    ui::draw_center(y + 3,
        std::to_string(ctx_.Meta().kleos) + "        " +
        std::to_string(ctx_.Meta().total_deaths) + "        " +
        std::to_string(ctx_.Meta().total_runs),
        {ui::ACCENT, true});

    struct Item { std::string label; int key; };
    std::vector<Item> items;

    if (ctx_.HasRun()) {
        items.push_back({"Continue run \u2014 Layer " +
                          std::to_string(ctx_.CurrentLayer()), '1'});
        items.push_back({"Abandon and start new run", '2'});
    } else {
        items.push_back({"New run", '1'});
    }
    items.push_back({"Enter the Loom", '3'});
    items.push_back({"Quit", '4'});

    int row = y + 6;
    for (const auto& it : items) {
        std::string line = "[" + std::string(1, (char)it.key) + "]  " + it.label;
        ui::draw(row, x + 4, line, {ui::INFO, true});
        row += 2;
    }

    ui::draw(y + h - 1, x + 2, " press a number ", {ui::DIM});

    int k = ui::get_key_ms(200);
    if (k == '1') {
        if (ctx_.HasRun()) { state_ = AppState::LayerIntro; return; }
        ctx_.NewRun();
        state_ = AppState::RunStart;
        return;
    }
    if (k == '2' && ctx_.HasRun()) {
        ctx_.EndRun();
        ctx_.NewRun();
        state_ = AppState::RunStart;
        return;
    }
    if (k == '3') { state_ = AppState::Loom; return; }
    if (k == '4') { state_ = AppState::Quit; return; }
}

// ====================================================================
// RUN START (the myth quote)
// ====================================================================
void GameApp::frame_run_start() {
    const int R = ui::rows();

    static auto opened_at = ui::now();
    static bool first = true;
    if (first) { opened_at = ui::now(); first = false; }

    long long elapsed = ui::ms_since(opened_at);

    if (elapsed < 1200) {
        ui::draw_center(R / 2 - 3, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        ui::draw_center(R / 2 - 3, "THE THREAD BEGINS", {ui::ACCENT, true});
        ui::draw_center(R / 2 - 1, "The Fates have woven again.",
                        {ui::INFO});
        ui::draw_wrapped(R / 2 + 2, (ui::cols() - 56) / 2, 56,
            "You have chosen the short, bright thread. Very well. "
            "We will weave it \u2014 again and again \u2014 until you "
            "understand what you have chosen.",
            {ui::DEFAULT});
    }

    ui::draw_center(R - 3, "press ENTER to walk to Phthia.", {ui::DIM, true});

    int k = ui::get_key_ms(16);
    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        first = true;
        state_ = AppState::LayerIntro;
    }
}

// ====================================================================
// LAYER INTRO (the card before every layer)
// ====================================================================
void GameApp::frame_layer_intro() {
    const int R = ui::rows();
    const int C = ui::cols();

    static auto opened_at = ui::now();
    static bool first = true;
    if (first) { opened_at = ui::now(); first = false; }

    long long elapsed = ui::ms_since(opened_at);

    int layer = ctx_.CurrentLayer();
    const char* name = screens::layer_name(layer);
    ui::Color accent = screens::layer_accent(layer);

    if (elapsed < 800) {
        ui::draw_center(R / 2, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        std::string title = std::string("LAYER ") +
                             std::to_string(layer) + " \u2014 " + name;
        ui::draw_center(R / 2 - 2, title, {accent, true});
        // \u2500 = "─" = UTF-8 bytes: E2 94 80
std::string underline;
underline.reserve(title.size() * 3);
for (size_t i = 0; i < title.size(); ++i) underline += "\u2500";  // string literal, not char
ui::draw_center(R / 2 - 1, underline, {accent});

        // Short per-layer flavor line (kept minimal in Step 2C; Step 3 enriches).
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
            ui::draw_center(R / 2 + 1, flavor[layer], {ui::INFO});
        }
    }

    ui::draw_center(R - 3, "press ENTER to proceed.", {ui::DIM, true});

    int k = ui::get_key_ms(16);
    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        first = true;
        // Dispatch: Layer 1 goes to a phantom encounter; other layers currently stub.
        start_encounter_for_layer();
    }
}