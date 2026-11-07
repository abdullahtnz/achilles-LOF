#include "tutorial.hpp"

namespace tutorial {

std::vector<Page> script() {
    return {
        // Page 1 — The Myth
        {
            "THE MYTH",
            {
                "Achilles, greatest of the Greeks, was born of Thetis,",
                "a sea-nymph, and Peleus, a mortal king.",
                "",
                "The Fates told Thetis: \"Your son will be great, but",
                "short-lived. He will die beneath Troy \u2014 unless he",
                "forsakes glory and lives long in obscurity.\"",
                "",
                "She dipped him in the river Styx to make him invulnerable,",
                "holding him by the heel. Every part of him grew immortal \u2014",
                "except the heel she held.",
                "",
                "One place. One flaw. One death.",
            },
            false, '\0'
        },

        // Page 2 — The Loom
        {
            "THE LOOM OF FATE",
            {
                "On the eve of the Trojan War, Achilles hid among the women",
                "of Skyros, dressed as one of them, to escape the prophecy.",
                "",
                "Odysseus found him. Laid out a sword among trinkets.",
                "Achilles reached for the sword.",
                "",
                "In that moment the world tore open. He fell into the Loom",
                "of Fate \u2014 where the three Moirai weave every mortal's",
                "thread.",
                "",
                "One of them spoke: \"You have chosen the short, bright",
                "thread. Very well. We will weave it \u2014 again and again",
                "\u2014 until you understand what you have chosen.\"",
            },
            false, '\0'
        },

        // Page 3 — You are Achilles
        {
            "YOU ARE ACHILLES",
            {
                "You are invulnerable everywhere except your heel.",
                "",
                "A blow to the heel ends the run. Everything else is",
                "survivable \u2014 HP damage, healing, stagger.",
                "",
                "Every death is a verse in the legend. Every death",
                "leaves you with kleos \u2014 glory \u2014 that persists",
                "across runs. The Fates remember.",
                "",
                "You will die. That is the loop. Each thread is cut and",
                "woven again.",
            },
            false, '\0'
        },

               // Page 4 — The actions
        {
            "YOUR ACTIONS",
            {
                "ATTACK   \u2014 deal 12\u201318 damage. Consecutive",
                "           attacks shrink your dodge window.",
                "",
                "RECKLESS \u2014 spend 20 HP for 3\u00D7 damage.",
                "           Exposes the heel. Unlocks at Layer 3.",
                "",
                "FOCUS    \u2014 spend 1 stamina to auto-dodge",
                "           the next attack. No timing needed.",
                "",
                "DODGE    \u2014 spend 1 stamina, 400ms window.",
                "           Press during the enemy's coil.",
                "",
                "HEAL     \u2014 spend 1 stamina, restore 20 HP.",
                "           Cannot heal a heel strike.",
                "",
                "STANCE   \u2014 Aggressive / Balanced / Defensive.",
                "           Defensive negates the next heel hit.",
                "",
                "FLEE     \u2014 withdraw. Confirmation if HP is high.",
            },
            false, '\0'
        },

        // Page 5 — The coil
        {
            "THE ENEMY COIL",
            {
                "Every enemy attack has three phases:",
                "",
                "  1. IDLE      \u2014 the enemy rests. Watch the \"\u2026\".",
                "",
                "  2. PRE-COIL  \u2014 the border turns yellow. Stillness.",
                "                 The enemy is deciding.",
                "",
                "  3. COIL      \u2014 the attack winds up. A meter fills.",
                "                 This is your window to press D.",
                "",
                "When the coil is a HEEL STRIKE, the border turns red",
                "and the tell line screams it. Press D in time or die.",
            },
            false, '\0'
        },

        // Page 6 — The lesson
        {
            "REMEMBER",
            {
                "Spamming attacks will kill you. Each attack you chain",
                "shortens your dodge window until you cannot react.",
                "",
                "The game rewards rhythm, not rage \u2014 at first.",
                "",
                "Later, rage becomes a tool. Later, you will choose.",
                "For now: read the enemy. Breathe. Strike when it's",
                "safe. Then strike again.",
            },
            true, '\n'
        },
    };
}

} // namespace tutorial