#pragma once
#include <chrono>
#include <string>
#include <vector>

namespace ui {

// ---------- Lifecycle ----------
void init();
void shutdown();

// ---------- Geometry ----------
int rows();
int cols();

// ---------- Color pairs (init'd once) ----------
enum Color : short {
    DEFAULT   = 0,
    DIM       = 1,
    ACCENT    = 2,      // gold/kleos
    DANGER    = 3,      // red/heel
    SAFE      = 4,      // green
    INFO      = 5,      // cyan
    WARNING   = 6,      // yellow
    HIGHLIGHT = 7,      // inverse/magenta
    BORDER    = 8,      // subtle frame color
};

// Attributes for a glyph
struct Attr {
    short color = DEFAULT;
    bool  bold  = false;
};

// ---------- Primitives ----------
void clear();
void refresh();

// Draw text at (y,x). Clips silently. Does not move cursor.
void draw(int y, int x, const std::string& text, Attr a = {});

// Draw centered text in a horizontal band starting at `y`.
void draw_center(int y, const std::string& text, Attr a = {});

// Box from (y,x) size (h,w). `label` appears in the top border if non-empty.
void draw_box(int y, int x, int h, int w, const std::string& label = "",
              Attr border_attr = {BORDER});

// Horizontal bar. pct in [0,1]. `glyph` fills; space empties.
void draw_bar(int y, int x, int w, double pct, Attr fill_attr,
              Attr empty_attr = {DIM});

// Multi-line word-wrapped block; returns lines printed.
int draw_wrapped(int y, int x, int w, const std::string& text,
                 Attr a = {}, int line_gap = 0);

// ---------- Input ----------
// Blocking until a key; returns key code (NCurses KEY_* or char).
int get_key();

// Non-blocking-with-timeout: wait up to `ms`. Returns -1 on timeout.
int get_key_ms(int ms);

// ---------- Timing ----------
using Clock = std::chrono::steady_clock;
Clock::time_point now();
long long ms_since(Clock::time_point t);

// ---------- Extras ----------
void beep_error();       // terminal bell
void flash();            // brief inverse flash (screen shake feel)

} // namespace ui