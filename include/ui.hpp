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

// ---------- Color pairs ----------
enum Color : short {
    DEFAULT   = 0,
    DIM       = 1,
    ACCENT    = 2,
    DANGER    = 3,
    SAFE      = 4,
    INFO      = 5,
    WARNING   = 6,
    HIGHLIGHT = 7,
    BORDER    = 8,
};

struct Attr {
    short color = DEFAULT;
    bool  bold  = false;
};

// ---------- Basic draw ----------
void clear();
void refresh();

void draw(int y, int x, const std::string& text, Attr a = {});
void draw_center(int y, const std::string& text, Attr a = {});

void draw_box(int y, int x, int h, int w, const std::string& label = "",
              Attr border_attr = {BORDER});

void draw_bar(int y, int x, int w, double pct, Attr fill_attr,
              Attr empty_attr = {DIM});

void draw_big_bar(int y, int x, int w, double pct, Attr fill_attr,
                  Attr shadow_attr = {DIM});

int  draw_wrapped(int y, int x, int w, const std::string& text,
                  Attr a = {}, int line_gap = 0);

struct Panel { int y, x, h, w; };
Panel draw_panel(int y, int x, int h, int w,
                 const std::string& label = "",
                 Attr border_attr = {BORDER});

// ---------- Input ----------
int get_key();
int get_key_ms(int ms);

// ---------- Timing ----------
using Clock = std::chrono::steady_clock;
Clock::time_point now();
long long ms_since(Clock::time_point t);
void sleep_ms(int ms);

// ---------- Effects ----------
void beep_error();
void flash_screen(int ms = 120);
void fade_in(int ms = 250);
void fade_out(int ms = 250);
void screen_shake(int frames = 3, int col_offset = 1);

// ---------- Overlays ----------
void draw_dim_overlay();                                 // dims the whole screen
void draw_overlay_banner(const std::string& text,
                         Attr a = {DANGER, true});       // top banner

// ---------- Utilities ----------
std::string repeat(const std::string& s, int n);
std::string centered_pad(const std::string& s, int width);

} // namespace ui