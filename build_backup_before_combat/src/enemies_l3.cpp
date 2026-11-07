#include "enemies.hpp"

Combatant make_l3_trojan(int index) {
    Combatant c;
    c.hp_max = 65 + index * 20;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Trojan Soldier";
            c.attacks = {
                {"Shield thrust",      11, false, true, 850},
                {"Sword cut",          10, false, true, 850},
                {"Heel gouge",         0,  true,  true, 1000},
            };
            break;
        case 1:
            c.name     = "Trojan Archer";
            c.behavior = "ranged";
            c.attacks = {
                {"Loosed arrow",       12, false, true, 950},
                {"Aimed shot",         8,  false, true, 850},
                {"Heel arrow",         0,  true,  true, 1200},
            };
            break;
        default:
            c.name     = "Charioteer";
            c.behavior = "chariot";
            // Two-hit pattern: wheel cut + spear thrust per cycle.
            c.attacks = {
                {"Wheel cut",          8,  false, true, 900},
                {"Spear thrust",       9,  false, true, 800},
                {"Trample",            7,  false, true, 750},
                {"Heel drag",          0,  true,  true, 1100},
            };
            break;
    }
    return c;
}

Combatant make_l3_hector_vanguard() {
    Combatant c;
    c.name     = "Hector's Vanguard";
    c.behavior = "shield_wall";
    c.hp_max   = 240;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Shield bash",      12, false, true, 900},
        {"Spear lunge",      14, false, true, 900},
        {"Heel pierce",       0,  true,  true, 1100},
        {"Wall slam",        11, false, true, 900},
    };
    return c;
}