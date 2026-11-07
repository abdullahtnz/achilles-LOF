#include "enemies.hpp"

Combatant make_l1_phantom(int index) {
    Combatant c;
    c.hp_max = 40 + index * 10;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Sparring Phantom";
            c.attacks = {
                {"Straight thrust",       6,  false, true,  900},
                {"Overhead cut",          8,  false, true,  900},
                {"Heel tap (warning)",    0,  true,  true,  1300}, // very slow, very forgiving
            };
            break;
        case 1:
            c.name = "Phantom Hoplite";
            c.attacks = {
                {"Shield bash",           9,  false, true,  850},
                {"Spear sweep",           7,  false, true,  850},
                {"Heel jab",              0,  true,  true,  1100},
                {"Overhead cut",          10, false, true,  850},
            };
            break;
        default:
            c.name = "Old Tutor's Shade";
            c.attacks = {
                {"Feint",                 6,  false, true,  800},
                {"Quarterstaff sweep",    8,  false, true,  800},
                {"Heel-seeking low cut",  0,  true,  false, 800}, // NOT telegraphed
                {"Overhead smash",        10, false, true,  800},
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
        {"Bow sweep",             10, false, true, 900},
        {"Hoof kick",             12, false, true, 900},
        {"Heel strike",            0, true,  true, 1200},  // long coil = tutorial
        {"Staff cross",           10, false, true, 900},
        {"Heel strike",            0, true,  true, 1100},
    };
    return c;
}