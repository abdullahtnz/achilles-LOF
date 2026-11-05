#include "game_app.hpp"
#include "ascii_art.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_death() {
    const int R = ui::rows();
    const int C = ui::cols();

    static auto opened_at = ui::now();
    static bool first = true;
    if (first) { opened_at = ui::now(); first = false; }

    long long elapsed = ui::ms_since(opened_at);

    // Brief screen shake: shift content by 1 column twice at start.
    int shake = 0;
    if (elapsed < 200) {
        shake = ((elapsed / 50) % 2 == 0) ? -1 : 1;
    }

    auto cut = art::thread_cut();
    if (elapsed < 400) {
        ui::draw_center(R / 2 - 2, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        art::draw_centered(cut, R / 2 - 5 + shake, ui::DANGER, true);
        ui::draw_center(R / 2 + 4, "THE THREAD IS CUT", {ui::DANGER, true});
    }

    if (elapsed > 800) {
        std::string line = "kleos " + std::to_string(ctx_.Meta().kleos) +
                           "    deaths " + std::to_string(ctx_.Meta().total_deaths);
        ui::draw_center(R - 6, line, {ui::ACCENT});
        ui::draw_center(R - 3, "press ENTER to return to the loom.",
                        {ui::DIM, true});
    }

    int k = ui::get_key_ms(16);
    if (elapsed > 800 && (k == '\n' || k == '\r' || k == KEY_ENTER)) {
        first = true;
        state_ = AppState::Loom;
    }
}