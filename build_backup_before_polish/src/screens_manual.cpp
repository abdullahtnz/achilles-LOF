#include "game_app.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>
#include <vector>

// ============================================================
// Pause menu — press P from combat
// ============================================================
void GameApp::frame_pause() {
    const int R = ui::rows();
    const int C = ui::cols();
    int w = 50, h = 12;
    int y = (R - h) / 2;
    int x = (C - w) / 2;

    ui::draw_box(y, x, h, w, " PAUSED ", {ui::ACCENT});

    ui::draw_center(y + 3, "[ R ] Resume",          {ui::INFO, true});
    ui::draw_center(y + 4, "[ H ] Open manual",     {ui::INFO, true});
    ui::draw_center(y + 5, "[ Q ] Quit to menu",    {ui::DANGER, true});
    ui::draw_center(y + 6, "[ X ] Quit game",       {ui::DANGER, true});
    ui::draw_center(y + 9, "progress is saved automatically",
                    {ui::DIM});

    int k = ui::get_key_ms(200);
    switch (k) {
        case 'r': case 'R':
            paused_ = false;
            transition_to(AppState::Combat);
            break;
        case 'h': case 'H':
            return_to_ = AppState::Pause;
            transition_to(AppState::Manual);
            break;
        case 'q': case 'Q':
            paused_ = false;
            transition_to(AppState::Menu);
            break;
        case 'x': case 'X':
            paused_ = false;
            transition_to(AppState::Quit);
            break;
    }
}

// ============================================================
// Manual — 8 sections, tab-navigated
// ============================================================
void GameApp::frame_manual() {
    static int section = 0;
    static bool started = false;
    if (!started) { section = 0; started = true; }

    const int R = ui::rows();
    const int C = ui::cols();
    int w = std::min(76, C - 4);
    int h = std::min(26, R - 2);
    int y = (R - h) / 2;
    int x = (C - w) / 2;

    static const char* titles[] = {
        "1. THE STORY", "2. THE HEEL", "3. ACTIONS", "4. STANCES",
        "5. ENEMIES",   "6. KLEOS",    "7. SAVES",   "8. ENDINGS"
    };

    ui::draw_box(y, x, h, w, " MANUAL ", {ui::ACCENT});

    // Tab strip
    for (int i = 0; i < 8; ++i) {
        int col = x + 3 + i * 8;
        if (col > x + w - 8) break;
        ui::Attr a = (i == section) ? ui::Attr{ui::ACCENT, true}
                                    : ui::Attr{ui::DIM};
        char lbl[4] = { (char)('1' + i), '.', '\0', '\0' };
        ui::draw(y + 1, col, lbl, a);
    }

    std::string title = titles[section];
    ui::draw_center(y + 3, title, {ui::ACCENT, true});

    int row = y + 5;
    auto line = [&](const std::string& s, ui::Attr a = {ui::DEFAULT}) {
        if (row >= y + h - 3) return;
        ui::draw(row, x + 4, s, a);
        row += 1;
    };

    switch (section) {
        case 0:
            line("Achilles chose the sword. The Fates weave.");
            line("");
            line("Each run is one weaving of the thread. Each death");
            line("is a strand cut. The Loom persists between runs.");
            line("The Fates remember. You remember.");
            line("");
            line("Six chapters lie ahead:");
            line("");
            line("  I    \u2014 PHTHIA          the boy becomes the weapon");
            line("  II   \u2014 SKYROS          the sword over the spindle");
            line("  III  \u2014 TROY SHORES     the host gathers");
            line("  IV   \u2014 SCAMANDER       wrath becomes slaughter");
            line("  V    \u2014 WALLS OF TROY   the greatest duel");
            line("  VI   \u2014 GATE OF HORN    the prophecy comes due");
            line("  VII  \u2014 SCAEAN GATE     the heel");
            break;

        case 1:
            line("THE HEEL IS THE ONLY DEATH THAT MATTERS.", {ui::DANGER, true});
            line("");
            line("You are invulnerable everywhere else. HP damage");
            line("is survivable \u2014 healing, stagger, retreat. A blow");
            line("to the heel ends the run instantly.");
            line("");
            line("How to defend the heel:");
            line("");
            line("  \u2022 DODGE  \u2014 press D during the enemy's coil.");
            line("  \u2022 FOCUS  \u2014 pre-commits a dodge, no timing needed.");
            line("  \u2022 DEFENSIVE STANCE \u2014 negates the next heel hit.");
            line("  \u2022 HEEL GUARD TOKEN \u2014 earned via story choices.");
            line("");
            line("Watch for the red border. That is your warning.");
            break;

        case 2:
            line("ATTACK    deal 12\u201318. Spam shrinks your dodge window.");
            line("");
            line("RECKLESS  spend 20 HP for 3\u00D7 damage. Exposes heel.");
            line("          Unlocks at Layer 3.");
            line("");
            line("FOCUS     spend 1 stamina to auto-dodge next attack.");
            line("          Unlocks at Layer 2.");
            line("");
            line("DODGE     spend 1 stamina, 400ms window during a coil.");
            line("");
            line("HEAL      spend 1 stamina, restore 20 HP. Cannot heal");
            line("          a heel strike.");
            line("");
            line("STANCE    cycle Aggressive / Balanced / Defensive.");
            line("          Unlocks at Layer 2.");
            line("");
            line("FLEE      withdraw. Confirmation if HP is high.");
            break;

        case 3:
            line("BALANCED (default)", {ui::INFO, true});
            line("  Baseline damage, baseline risk.");
            line("");
            line("AGGRESSIVE", {ui::DANGER, true});
            line("  +50% damage, +30% heel risk to you.");
            line("  Ares-flavored. For those who choose glory.");
            line("");
            line("DEFENSIVE", {ui::SAFE, true});
            line("  -30% damage, -30% incoming damage.");
            line("  IMMUNE TO HEEL STRIKES while in this stance.");
            line("  Athena-flavored. For those who choose life.");
            line("");
            line("Switching stance costs one turn and breaks combo.");
            break;

        case 4: {
            line("Bestiary \u2014 entries unlock as you fight.", {ui::DIM});
            line("");
            // Placeholder — real bestiary populates from meta later
            line("  SPARRING PHANTOM       Layer 1  \u2014 taught you to dodge");
            line("  PHANTOM HOPLITE        Layer 1  \u2014 multiple patterns");
            line("  OLD TUTOR'S SHADE      Layer 1  \u2014 untelegraphed heel");
            line("  CHIRON, THE CENTAUR    Layer 1  \u2014 long coils, teaching");
            line("  ILLUSION               Layer 2  \u2014 mirrors your action");
            line("  MIRRORED DUELIST       Layer 2  \u2014 you, mirrored");
            line("  LYCOMEDES' GUARD       Layer 2  \u2014 phalanx");
            line("  DEIDAMIA'S GRIEF       Layer 2  \u2014 charges when idle");
            line("  TROJAN SOLDIER         Layer 3");
            line("  TROJAN ARCHER          Layer 3  \u2014 ranged");
            line("  CHARIOTEER             Layer 3  \u2014 multiple attacks");
            line("  HECTOR'S VANGUARD      Layer 3  \u2014 shield wall");
            break;
        }

        case 5:
            line("KLEOS is glory. It persists across deaths.");
            line("");
            line("You earn kleos by:");
            line("  \u2022 Winning fights (10\u201315 base + encounter bonus)");
            line("  \u2022 Surviving with HP intact (up to +10 bonus)");
            line("  \u2022 Dying heroically (15)");
            line("  \u2022 Finding story fragments");
            line("");
            line("Kleos unlocks weapons, boons, story branches, and");
            line("the true ending. Spending arrives in Step 5.");
            break;

        case 6:
            line("SAVES");
            line("");
            line("The game saves automatically on:");
            line("  \u2022 Every scene transition");
            line("  \u2022 Every choice made");
            line("  \u2022 Every combat start (checkpoint)");
            line("  \u2022 Every death");
            line("");
            line("You can close the game at any time and resume");
            line("exactly where you were. Dialogue progress is");
            line("preserved.");
            line("");
            line("DEATH sends you to your last checkpoint. All kleos");
            line("is kept. Story flags are kept. The fight resumes");
            line("from its start.");
            break;

        case 7:
            line("Three endings. Each is earned differently.");
            line("");
            line("A \u2014 KLEOS");
            line("  Die at Troy. The arrow finds your heel. Your name");
            line("  becomes myth.");
            line("");
            line("B \u2014 THE PHANTOM");
            line("  Die too many times consumed by rage. Become one of");
            line("  the past-death phantoms that hunts future Achilles.");
            line("  Hint: choose Ares. Spam Reckless. Do not grieve.");
            line("");
            line("C \u2014 THE TRUE ENDING");
            line("  Collect every Memory Fragment. Learn the truth: you");
            line("  did not die of an arrow. You died of grief.");
            line("  Hint: explore. Read every line. Do not rush.");
            break;
    }

    ui::draw_center(y + h - 2,
        "[1-8] section   [ESC] back",
        {ui::DIM});

    int k = ui::get_key_ms(200);
    if (k == 27) {           // ESC
        started = false;
        transition_to(return_to_);
        return;
    }
    if (k >= '1' && k <= '8') {
        section = k - '1';
    }
}