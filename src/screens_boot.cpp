#include "game_app.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_boot() {
    const int R = ui::rows();
    long long elapsed = ui::ms_since(state_entered_at_);

    auto draw_title = [&]() {
        ui::draw_center(R / 2 - 2, "a c h i l l e s", {ui::ACCENT, true});
        std::string rule;
        for (int i = 0; i < 22; ++i) rule += "\u2500";
        ui::draw_center(R / 2, rule, {ui::ACCENT});
        ui::draw_center(R / 2 + 1, "the loom of fate", {ui::INFO});
    };

    if (elapsed < 800) {
        ui::draw_center(R / 2 - 2, "a c h i l l e s", {ui::ACCENT, true});
    } else if (elapsed < 1400) {
        draw_title();
    } else {
        draw_title();
        ui::draw_center(R / 2 + 5,
            "\"You have chosen the short, bright thread.",
            {ui::DEFAULT});
        ui::draw_center(R / 2 + 6,
            " Very well. We will weave it \u2014 again and again \u2014",
            {ui::DEFAULT});
        ui::draw_center(R / 2 + 7,
            " until you understand what you have chosen.\"",
            {ui::DEFAULT});
    }

    if (elapsed > 2200) {
        // Gentle pulsing prompt
        bool visible = ((elapsed / 600) % 2) == 0;
        if (visible) {
            ui::draw_center(R - 4, "[ press any key ]", {ui::DIM, true});
        }
    }

    int k = ui::get_key_ms(16);
    if (elapsed > 2200 && k != ERR) {
        if (!ctx_.Meta().first_launch_seen) {
            transition_to(AppState::Tutorial);
        } else {
            transition_to(AppState::Menu);
        }
    }
}