#include "game_app.hpp"
#include "ascii_art.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_death() {
    const int R = ui::rows();
    const int C = ui::cols();
    long long elapsed = ui::ms_since(state_entered_at_);

    // On entry: brief screen shake.
    if (elapsed < 16) death_shook_ = false;
    if (elapsed < 250 && elapsed > 20 && !death_shook_) {
        ui::screen_shake(3, 1);
        death_shook_ = true;
    }

    // Fade in the death content
    auto cut = art::thread_cut();
    if (elapsed < 400) {
        ui::draw_center(R / 2 - 2, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        art::draw_centered(cut, R / 2 - 5, ui::DANGER, true);
        ui::draw_center(R / 2 + 4, "THE THREAD IS CUT", {ui::DANGER, true});
        ui::draw_center(R / 2 + 5, "the loom re-spins",   {ui::DIM});
    }

    if (elapsed > 800) {
        std::string line = "kleos " + std::to_string(ctx_.Meta().kleos) +
                           "    deaths " + std::to_string(ctx_.Meta().total_deaths);
        ui::draw_center(R - 6, line, {ui::ACCENT});
        if (ctx_.LastEnding() == Ending::B_Phantom) {
            ui::draw_center(R - 5, "the rage has made you into a phantom",
                            {ui::DANGER, true});
        }
        ui::draw_center(R - 3, "press ENTER to return to the loom.",
                        {ui::DIM, true});
    }

    int k = ui::get_key_ms(16);
    if (elapsed > 800 && (k == '\n' || k == '\r' || k == KEY_ENTER)) {
        transition_to(AppState::Loom);
    }
}