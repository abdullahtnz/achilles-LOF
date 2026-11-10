#include "ui.hpp"

#include <ncursesw/curses.h>
#include <clocale>
#include <algorithm>

namespace ui {

static bool inited = false;

void init() {
    if (inited) return;
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, FALSE);
    set_escdelay(25);
    start_color();
    use_default_colors();

    init_pair(DEFAULT,   -1,           -1);
    init_pair(DIM,       COLOR_WHITE,  -1);
    init_pair(ACCENT,    COLOR_YELLOW, -1);
    init_pair(DANGER,    COLOR_RED,    -1);
    init_pair(SAFE,      COLOR_GREEN,  -1);
    init_pair(INFO,      COLOR_CYAN,   -1);
    init_pair(WARNING,   COLOR_YELLOW, -1);
    init_pair(HIGHLIGHT, COLOR_MAGENTA,-1);
    init_pair(BORDER,    COLOR_BLUE,   -1);

    inited = true;
}

void shutdown() {
    if (!inited) return;
    endwin();
    inited = false;
}

int rows() { return LINES; }
int cols() { return COLS; }

static attr_t to_attr(const Attr& a) {
    attr_t at = COLOR_PAIR(a.color);
    if (a.bold) at |= A_BOLD;
    if (a.color == DIM) at |= A_DIM;
    return at;
}

void clear() { ::clear(); }
void refresh() { ::refresh(); }

void draw(int y, int x, const std::string& text, Attr a) {
    if (y < 0 || y >= LINES || x >= COLS || x < 0) return;
    attron(to_attr(a));
    mvaddstr(y, x, text.c_str());
    attroff(to_attr(a));
}

void draw_center(int y, const std::string& text, Attr a) {
    int x = (COLS - (int)text.size()) / 2;
    if (x < 0) x = 0;
    draw(y, x, text, a);
}

void draw_box(int y, int x, int h, int w, const std::string& label, Attr b) {
    if (h < 3 || w < 3) return;
    attron(to_attr(b));

    mvaddstr(y,           x,         "\u250C");
    mvaddstr(y,           x + w - 1, "\u2510");
    mvaddstr(y + h - 1,   x,         "\u2514");
    mvaddstr(y + h - 1,   x + w - 1, "\u2518");

    for (int i = 1; i < h - 1; ++i) {
        mvaddstr(y + i, x,         "\u2502");
        mvaddstr(y + i, x + w - 1, "\u2502");
    }
    for (int i = 1; i < w - 1; ++i) {
        mvaddstr(y,         x + i, "\u2500");
        mvaddstr(y + h - 1, x + i, "\u2500");
    }
    attroff(to_attr(b));

    if (!label.empty()) {
        std::string framed = " " + label + " ";
        draw(y, x + 2, framed, b);
    }
}

void draw_bar(int y, int x, int w, double pct, Attr fill, Attr empty) {
    if (pct < 0) pct = 0;
    if (pct > 1) pct = 1;
    int n = (int)(pct * w);
    if (n > w) n = w;
    draw(y, x, std::string(n, '#'), fill);
    draw(y, x + n, std::string(w - n, '.'), empty);
}

void draw_big_bar(int y, int x, int w, double pct, Attr fill, Attr shadow) {
    if (pct < 0) pct = 0;
    if (pct > 1) pct = 1;
    int n = (int)(pct * w);
    if (n > w) n = w;

    std::string top_fill, top_empty;
    for (int i = 0; i < n; ++i)     top_fill  += "\u2588";
    for (int i = 0; i < w - n; ++i) top_empty += "\u2591";
    draw(y, x, top_fill, fill);
    draw(y, x + n, top_empty, shadow);

    std::string shadow_row;
    for (int i = 0; i < w; ++i) shadow_row += "\u2591";
    draw(y + 1, x, shadow_row, shadow);
}

int draw_wrapped(int y, int x, int w, const std::string& text, Attr a, int gap) {
    int cur_y = y;
    size_t i = 0;
    while (i <= text.size()) {
        size_t nl = text.find('\n', i);
        std::string chunk = (nl == std::string::npos)
                              ? text.substr(i)
                              : text.substr(i, nl - i);
        i = (nl == std::string::npos) ? text.size() + 1 : nl + 1;

        size_t start = 0;
        while (start < chunk.size()) {
            size_t take = std::min((size_t)w, chunk.size() - start);
            if (start + take < chunk.size()) {
                size_t sp = chunk.rfind(' ', start + take);
                if (sp != std::string::npos && sp > start) take = sp - start;
            }
            draw(cur_y, x, chunk.substr(start, take), a);
            cur_y += 1 + gap;
            start += take;
            while (start < chunk.size() && chunk[start] == ' ') ++start;
        }
        if (nl == std::string::npos) break;
    }
    return cur_y - y;
}

Panel draw_panel(int y, int x, int h, int w, const std::string& label, Attr b) {
    draw_box(y, x, h, w, label, b);
    return Panel{y + 1, x + 1, h - 2, w - 2};
}

int get_key() { return getch(); }

int get_key_ms(int ms) {
    timeout(ms);
    int c = getch();
    timeout(-1);
    return c;
}

Clock::time_point now() { return Clock::now(); }
long long ms_since(Clock::time_point t) {
    auto d = Clock::now() - t;
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

void sleep_ms(int ms) { napms(ms); }

void beep_error() { beep(); }

void flash_screen(int ms) {
    // Invert briefly to simulate a flash.
    attrset(A_REVERSE);
    refresh();
    napms(ms);
    attrset(A_NORMAL);
    refresh();
}

void fade_in(int ms) {
    // ncurses cannot truly fade; simulate with a very short delay and
    // an incremental reveal by clearing content then redrawing.
    // Callers use this to smooth state transitions.
    refresh();
    napms(ms);
}

void fade_out(int ms) {
    // Brief pause before clearing — visually reads as "settling".
    refresh();
    napms(ms);
}

void screen_shake(int frames, int col_offset) {
    // ncurses cannot truly shake; a quick inverted flash reads as impact.
    for (int i = 0; i < frames; ++i) {
        attrset(i % 2 == 0 ? A_REVERSE : A_NORMAL);
        refresh();
        napms(40);
    }
    attrset(A_NORMAL);
    refresh();
    (void)col_offset;
}

void draw_dim_overlay() {
    // Re-draw every cell in DIM attribute to dim the current content.
    attrset(A_DIM | COLOR_PAIR(DIM));
    for (int y = 0; y < LINES; ++y) {
        mvaddstr(y, 0, std::string(COLS, ' ').c_str());
    }
    attrset(A_NORMAL);
}

void draw_overlay_banner(const std::string& text, Attr a) {
    int y = 2;
    int x = (COLS - (int)text.size()) / 2;
    if (x < 0) x = 0;
    draw(y, x, text, a);
}

std::string repeat(const std::string& s, int n) {
    std::string out;
    out.reserve(s.size() * n);
    for (int i = 0; i < n; ++i) out += s;
    return out;
}

std::string centered_pad(const std::string& s, int width) {
    if ((int)s.size() >= width) return s;
    int pad = (width - (int)s.size()) / 2;
    return std::string(pad, ' ') + s;
}

} // namespace ui