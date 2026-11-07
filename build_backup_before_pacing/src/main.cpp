#include <cstdio>
#include <exception>

#include "game_app.hpp"
#include "ui.hpp"

int main() {
    try {
        ui::init();
        GameApp app;
        int rc = app.Run();
        ui::shutdown();
        return rc;
    } catch (const std::exception& e) {
        ui::shutdown();
        std::fprintf(stderr, "Fatal: %s\n", e.what());
        return 1;
    }
}