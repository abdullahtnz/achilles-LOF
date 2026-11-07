#include "game_app.hpp"
#include "tutorial.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_tutorial() {
    static int  page_idx = 0;
    static bool started  = false;
    if (!started) { page_idx = 0; started = true; }

    auto pages = tutorial::script();
    if (page_idx >= (int)pages.size()) page_idx = (int)pages.size() - 1;
    const auto& page = pages[page_idx];

    const int R = ui::rows();
    const int C = ui::cols();
    int w = std::min(74, C - 4);
    int h = std::min(24, R - 2);
    int y = (R - h) / 2;
    int x = (C - w) / 2;

    std::string header = " " + page.title + "  \u00B7  " +
                         std::to_string(page_idx + 1) + " / " +
                         std::to_string((int)pages.size()) + " ";
    ui::draw_box(y, x, h, w, header, {ui::ACCENT});

    int row = y + 2;
    for (const auto& line : page.paragraphs) {
        if (row >= y + h - 3) break;
        ui::draw(row, x + 3, line, {ui::DEFAULT});
        row += 1;
    }

    int prompt_y = y + h - 2;
    if (page.show_keystroke_prompt) {
        ui::draw_center(prompt_y, "Press ENTER to begin the weave.",
                        {ui::DIM, true});
    } else {
        ui::draw_center(prompt_y, "[ ENTER ] next     [ Q ] skip tutorial",
                        {ui::DIM});
    }

    int k = ui::get_key_ms(200);
    if (k == ERR) return;

    if (k == 'q' || k == 'Q') {
        auto meta = ctx_.Meta();
        meta.first_launch_seen = true;
        ctx_.Saves().SaveMeta(meta);
        started = false;
        transition_to(AppState::Menu);
        return;
    }

    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        page_idx++;
        if (page_idx >= (int)pages.size()) {
            auto meta = ctx_.Meta();
            meta.first_launch_seen = true;
            ctx_.Saves().SaveMeta(meta);
            started = false;
            page_idx = 0;
            transition_to(AppState::Menu);
        }
    }
}