#include "game_app.hpp"
#include "dialogue.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_dialogue() {
    auto& run = ctx_.MutableRun();
    const Scene* scene = find_scene(run.dialogue.scene_id);
    if (!scene) { transition_to(AppState::Menu); return; }

    const int R = ui::rows();
    const int C = ui::cols();

    // Title (small, top)
    ui::draw_center(2, scene->title, {ui::ACCENT, true});
    std::string rule;
    for (int i = 0; i < 40; ++i) rule += "\u2500";
    ui::draw_center(3, rule, {ui::ACCENT});

    // Prose region — currently-shown line only (line-by-line advance).
    // For richer feel, show last N lines stacked.
    int prose_top = 6;
    int prose_width = std::min(64, C - 8);
    int prose_x = (C - prose_width) / 2;

    int start_idx = std::max(0, run.dialogue.line_index - 5);
    int end_idx   = run.dialogue.line_index;
    int cur_y = prose_top;
    for (int i = start_idx; i <= end_idx && i < (int)scene->lines.size(); ++i) {
        const auto& ln = scene->lines[i];
        bool is_current = (i == end_idx);
        ui::Attr a = is_current ? ui::Attr{ui::DEFAULT, false}
                                : ui::Attr{ui::DIM};

        if (!ln.speaker.empty()) {
            ui::draw(cur_y, prose_x, ln.speaker + ":", {ui::ACCENT, true});
            cur_y++;
        }
        int h = ui::draw_wrapped(cur_y, prose_x + 2, prose_width - 2,
                                 ln.text, a);
        cur_y += h + 1;
    }

    // Choices or advance prompt
    int bottom_y = R - 5;
    if (run.dialogue.pending_choice) {
        ui::draw_center(bottom_y - 2, "choose:", {ui::DIM});
        for (size_t i = 0; i < scene->choices.size(); ++i) {
            std::string line = "[" + std::to_string(i + 1) + "]  " +
                               scene->choices[i].label;
            ui::draw_center(bottom_y - 1 + (int)i, line, {ui::INFO, true});
        }
    } else {
        bool can_advance = run.dialogue.line_index + 1 < (int)scene->lines.size();
        if (can_advance) {
            ui::draw_center(bottom_y, "[ ENTER ] continue", {ui::DIM});
        } else {
            ui::draw_center(bottom_y, "[ ENTER ] continue", {ui::DIM});
        }
    }

    // Input
    int k = ui::get_key_ms(200);
    if (k == ERR) return;

    if (run.dialogue.pending_choice) {
        if (k >= '1' && k <= '9') {
            int idx = k - '1';
            if (idx < (int)scene->choices.size()) {
                dialogue_choose(idx);
                return;
            }
        }
        return;
    }

    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        dialogue_advance();
    } else if (k == 'p' || k == 'P') {
        paused_ = true;
        transition_to(AppState::Pause);
    }
}