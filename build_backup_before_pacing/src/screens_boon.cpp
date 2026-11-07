#include "game_app.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_boon_choice() {
    const int R = ui::rows();
    const int C = ui::cols();
    const int w = std::min(76, C - 4);
    const int h = 20;
    const int y = (R - h) / 2;
    const int x = (C - w) / 2;

    ui::draw_box(y, x, h, w, " THE FATES DEMAND A CHOICE ", {ui::ACCENT});

    ui::draw_center(y + 2,
        "The path you walk now determines how your thread is cut.",
        {ui::INFO});

    ui::draw(y + 5, x + 6, "ARES", {ui::DANGER, true});
    ui::draw(y + 6, x + 6, "Rage & Bloodlust", {ui::DEFAULT});
    ui::draw(y + 7, x + 6, "+30% damage",     {ui::DEFAULT});
    ui::draw(y + 8, x + 6, "+20% heel chance",{ui::DANGER});
    ui::draw(y + 9, x + 6, "blood pays for blood.", {ui::DIM});

    ui::draw(y + 5, x + w - 26, "ATHENA", {ui::INFO, true});
    ui::draw(y + 6, x + w - 26, "Tactics & Precision", {ui::DEFAULT});
    ui::draw(y + 7, x + w - 26, "+50% dodge window",   {ui::DEFAULT});
    ui::draw(y + 8, x + w - 26, "-25% damage",         {ui::DEFAULT});
    ui::draw(y + 9, x + w - 26, "wisdom is a longer life.", {ui::DIM});

    ui::draw_center(y + h - 3, "[1] Take Ares' path      [2] Take Athena's path",
                    {ui::ACCENT, true});

    int k = ui::get_key_ms(200);
    if (k == '1' || k == '2') {
        auto& run = ctx_.MutableRun();
        run.path = (k == '1') ? "ares" : "athena";
        run.boon_chosen = true;
        if (k == '1') {
            run.boons.push_back("ares_rage");
            // Ares tag on player name enables +damage in future tuning;
            // for now gameplay effect is stored, not yet applied numerically.
        } else {
            run.boons.push_back("athena_tactics");
        }
        ctx_.PersistRun();
        boon_pending_ = false;
        state_ = AppState::LayerIntro;
    }
}