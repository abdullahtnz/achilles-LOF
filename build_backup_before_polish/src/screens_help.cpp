#include "game_app.hpp"
#include "ui.hpp"
#include <ncurses.h>

void GameApp::frame_help() {
    const int R = ui::rows();
    const int C = ui::cols();

    ui::draw_box(2, 2, R - 4, C - 4, " HELP ", {ui::INFO});

    int y = R / 2 - 6;
    ui::draw_center(y,     "A  \u2014  Attack",          {ui::DEFAULT});
    ui::draw_center(y + 1, "R  \u2014  Reckless Strike", {ui::DEFAULT});
    ui::draw_center(y + 2, "F  \u2014  Focus",           {ui::DEFAULT});
    ui::draw_center(y + 3, "D  \u2014  Dodge",           {ui::DEFAULT});
    ui::draw_center(y + 4, "H  \u2014  Heal",            {ui::DEFAULT});
    ui::draw_center(y + 5, "S  \u2014  Cycle Stance",    {ui::DEFAULT});
    ui::draw_center(y + 6, "X  \u2014  Flee",            {ui::DEFAULT});
    ui::draw_center(y + 8, "P  \u2014  Pause",           {ui::DIM});
    ui::draw_center(y + 9, "?  \u2014  This help screen", {ui::DIM});

    ui::draw_center(R - 4, "press any key to return", {ui::DIM, true});

    int k = ui::get_key_ms(200);
    if (k != ERR) transition_to(return_to_);
}