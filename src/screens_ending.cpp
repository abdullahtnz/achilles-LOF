#include "game_app.hpp"
#include "ascii_art.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>
#include <vector>

void GameApp::frame_ending() {
    const int R = ui::rows();
    long long elapsed = ui::ms_since(state_entered_at_);

    Ending e = ctx_.LastEnding();

    std::string title;
    std::string subtitle;
    std::vector<std::string> lines;
    ui::Color accent = ui::ACCENT;

    switch (e) {
        case Ending::C_True:
            title = "THE WEAVER";
            subtitle = "ending C \u2014 the true ending";
            accent = ui::HIGHLIGHT;
            lines = {
                "You lowered your guard and spoke his name.",
                "The shears did not close. The thread was tied.",
                "",
                "You cannot avoid death. You can only make it yours.",
                "The heel was never the weakness. The rage was.",
            };
            break;
        case Ending::B_Phantom:
            title = "THE PHANTOM";
            subtitle = "ending B \u2014 the nameless thread";
            accent = ui::DANGER;
            lines = {
                "You died consumed by rage, and the loom recycled you.",
                "Now you are one of the past deaths that hunts future Achilles.",
                "",
                "Hard mode unlocked: The Nameless Thread.",
            };
            break;
        case Ending::A_Kleos:
        default:
            title = "KLEOS";
            subtitle = "ending A \u2014 the cut thread";
            accent = ui::ACCENT;
            lines = {
                "The arrow found your heel. The Fates cut the thread.",
                "Greatest of the Greeks. Short-lived. Never forgotten.",
                "",
                "You chose glory. Glory chose you back.",
                "The loom re-spins.",
            };
            break;
    }

    if (elapsed < 450) {
        ui::draw_center(R / 2, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        art::draw_centered(art::thread_cut(), 2, accent, true);
        ui::draw_center(15, title, {accent, true});
        ui::draw_center(16, subtitle, {ui::DIM});

        int y = 19;
        for (const auto& ln : lines) {
            if (!ln.empty()) ui::draw_center(y, ln, {ui::DEFAULT});
            y += 1;
        }
    }

    const auto& meta = ctx_.Meta();
    if (elapsed > 900) {
        ui::draw_center(R - 5,
            "kleos " + std::to_string(meta.kleos) +
            "    deaths " + std::to_string(meta.total_deaths) +
            "    fragments " + std::to_string((int)meta.fragments_collected.size()) +
            "/" + std::to_string(FRAGMENT_TOTAL),
            {ui::ACCENT});
        ui::draw_center(R - 3, "press ENTER to return to the loom",
                        {ui::DIM, true});
    }

    int k = ui::get_key_ms(16);
    if (elapsed > 900 && (k == '\n' || k == '\r' || k == KEY_ENTER)) {
        transition_to(AppState::Loom);
    }
}
