#include "game_app.hpp"
#include "ascii_art.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_loom() {
    const int R = ui::rows();
    const int C = ui::cols();

    static auto opened_at = ui::now();
    static bool first = true;
    if (first) { opened_at = ui::now(); first = false; }

    long long elapsed = ui::ms_since(opened_at);

    // Art fades in
    auto loom_art = art::loom();
    int art_y = 3;
    if (elapsed < 600) {
        ui::draw_center(art_y + 4, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        art::draw_centered(loom_art, art_y, ui::HIGHLIGHT, true);
    }

    int title_y = art_y + (int)loom_art.size() + 1;
    ui::draw_center(title_y, "THE LOOM", {ui::HIGHLIGHT, true});
    ui::draw_center(title_y + 1, "the three sisters weave on", {ui::DIM});

    int info_y = title_y + 4;
    ui::draw_center(info_y, "kleos " + std::to_string(ctx_.Meta().kleos) +
                    "    deaths " + std::to_string(ctx_.Meta().total_deaths) +
                    "    runs " + std::to_string(ctx_.Meta().total_runs),
                    {ui::ACCENT, true});

    ui::draw_center(info_y + 2, "spending arrives in a later step.",
                    {ui::DIM});

    ui::draw_center(R - 3, "press ENTER to step back from the loom.",
                    {ui::DIM, true});

    int k = ui::get_key_ms(16);
    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        first = true;
        state_ = AppState::Menu;
    }
}