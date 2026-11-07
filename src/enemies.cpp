#include "enemies.hpp"

Combatant make_l1_phantom(int index) {
    Combatant c;
    c.hp_max = 40 + index * 10;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Sparring Phantom";
            c.attacks = {
                {"Straight thrust", "lunging",           6,  false, true, 1800},
                {"Overhead cut",    "raising its arm",  8,  false, true, 1800},
                {"Heel tap",        "lowering its point", 0, true,  true, 2000},
            };
            break;
        case 1:
            c.name = "Phantom Hoplite";
            c.attacks = {
                {"Shield bash",  "surging forward", 9,  false, true, 1600},
                {"Spear sweep",  "sweeping low",    7,  false, true, 1600},
                {"Heel jab",     "drawing back",    0,  true,  true, 1800},
                {"Overhead cut", "raising high",    10, false, true, 1600},
            };
            break;
        default:
            c.name = "Old Tutor's Shade";
            c.attacks = {
                {"Feint",                "shifting weight",   6,  false, true,  1500},
                {"Quarterstaff sweep",   "spinning",          8,  false, true,  1500},
                {"Heel-seeking low cut", "moving strangely",  0,  true,  false, 1400},
                {"Overhead smash",       "winding up",        10, false, true,  1500},
            };
            break;
    }
    return c;
}

Combatant make_l1_chiron() {
    Combatant c;
    c.name   = "Chiron, the Centaur";
    c.hp_max = 180;
    c.hp     = c.hp_max;
    c.attacks = {
        {"Bow sweep",  "drawing the bow",     10, false, true, 1800},
        {"Hoof kick",  "rearing",             12, false, true, 1800},
        {"Heel strike","coiling to strike low",0, true,  true, 2200},
        {"Staff cross","crossing the staff",  10, false, true, 1800},
        {"Heel strike","coiling to strike low",0, true,  true, 2000},
    };
    return c;
}