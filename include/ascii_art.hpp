#pragma once
#include "ui.hpp"
#include <string>
#include <vector>

namespace art {

// ============================================================
// Layer 1 — Chiron (centaur teacher)
// ============================================================
inline std::vector<std::string> chiron() {
    return {
        "         /\\_/\\   ",
        "        ( o.o )  ",
        "         > ^ <   ",
        "        /  |  \\  ",
        "       /   |   \\ ",
        "      *----+----*",
        "          / \\    ",
        "         /   \\   ",
        "        /     \\  ",
        "       /       \\ ",
    };
}

// ============================================================
// Layer 1 — generic phantom (used for the sparring shades)
// ============================================================
inline std::vector<std::string> phantom() {
    return {
        "       .-\"\"\"-.  ",
        "      /       \\ ",
        "     |  o   o  |",
        "     |    ^    |",
        "     |   ---   |",
        "      \\_______/ ",
        "       |     |  ",
        "      /       \\ ",
        "     /         \\",
    };
}

// ============================================================
// The Loom (Moirai's weaving frame)
// ============================================================
inline std::vector<std::string> loom() {
    return {
        "   |    |    |    |   ",
        "   |    |    |    |   ",
        "  ~~+~~~~+~~~~+~~~~+~ ",
        "   |    |    |    |   ",
        "  ~~+~~~~+~~~~+~~~~+~ ",
        "   |    |    |    |   ",
        "   |    |    |    |   ",
        "   |    |    |    |   ",
        "  / \\  / \\  / \\  / \\ ",
        " (   )(   )(   )(   )",
        "  \\ /  \\ /  \\ /  \\ / ",
    };
}

// ============================================================
// Thread cut (death screen)
// ============================================================
inline std::vector<std::string> thread_cut() {
    return {
        "     \\    |    /     ",
        "      \\   |   /      ",
        "       \\  |  /       ",
        "        \\ | /        ",
        "         \\|/         ",
        "   ------- X ------- ",
        "         /|\\         ",
        "        / | \\        ",
        "       /  |  \\       ",
        "      /   |   \\      ",
        "     /    |    \\     ",
    };
}

// ============================================================
// The three sisters (used on the Loom screen optionally)
// ============================================================
inline std::vector<std::string> moirai() {
    return {
        "  ___      ___      ___  ",
        " /   \\    /   \\    /   \\ ",
        "| o o |  | o o |  | o o |",
        "|  ^  |  |  ^  |  |  ^  |",
        "| --- |  | --- |  | --- |",
        " \\___/    \\___/    \\___/ ",
        "   |        |        |   ",
        "   |        |        |   ",
        "  / \\      / \\      / \\ ",
        " /   \\    /   \\    /   \\ ",
    };
}

// ============================================================
// Helper — draw an art block centered horizontally at row y0.
// Returns the number of rows drawn (art.size()).
// ============================================================
inline int draw_centered(const std::vector<std::string>& lines, int y0,
                         int color = 0, bool bold = false) {
    if (ui::cols() <= 0) return 0;
    int y = y0;
    for (const auto& ln : lines) {
        int x = (ui::cols() - (int)ln.size()) / 2;
        if (x < 0) x = 0;
        ui::draw(y, x, ln, { (short)color, bold });
        ++y;
    }
    return (int)lines.size();
}

// ============================================================
// Helper — draw art with a subtle shadow offset for depth.
// Adds one row of drop-shadow (dimmer, +1 row, +1 col).
// ============================================================
inline int draw_centered_with_shadow(const std::vector<std::string>& lines,
                                     int y0, int color = 0, bool bold = false) {
    if (ui::cols() <= 0) return 0;
    // Shadow pass first, offset by (1, 1).
    int y = y0 + 1;
    for (const auto& ln : lines) {
        int x = (ui::cols() - (int)ln.size()) / 2 + 1;
        if (x < 0) x = 0;
        std::string shadow(ln.size(), ' ');
        // Only draw shadow under non-space glyphs.
        for (size_t i = 0; i < ln.size(); ++i) {
            shadow[i] = (ln[i] == ' ') ? ' ' : '#';
        }
        ui::draw(y, x, shadow, { ui::DIM });
        ++y;
    }
    // Foreground pass on top.
    return draw_centered(lines, y0, color, bold);
}

} // namespace art