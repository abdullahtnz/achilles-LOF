#include "enemies.hpp"

Combatant make_l3_trojan(int index) {
    Combatant c;
    c.hp_max = 65 + index * 20;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Trojan Soldier";
            c.attacks = {
                {"Shield thrust", "shoving forward",  11, false, true, 1500},
                {"Sword cut",     "cutting diagonally",10, false, true, 1500},
                {"Heel gouge",    "dropping low",     0, true, true, 1700},
            };
            break;
        case 1:
            c.name     = "Trojan Archer";
            c.behavior = "ranged";
            c.attacks = {
                {"Loosed arrow", "loosing an arrow", 12, false, true, 1600},
                {"Aimed shot",   "drawing the bow",   8, false, true, 1500},
                {"Heel arrow",   "aiming low",        0, true, true, 1800},
            };
            break;
        default:
            c.name     = "Charioteer";
            c.behavior = "chariot";
            c.attacks = {
                {"Wheel cut",    "spinning the wheel", 8, false, true, 1500},
                {"Spear thrust", "thrusting down",     9, false, true, 1500},
                {"Trample",      "driving the horses", 7, false, true, 1400},
                {"Heel drag",    "cutting low",        0, true, true, 1700},
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
        {"Shield bash", "bracing the shield", 12, false, true, 1600},
        {"Spear lunge", "lunging forward",    14, false, true, 1600},
        {"Heel pierce", "dropping the tip",    0, true, true, 1800},
        {"Wall slam",   "charging",           11, false, true, 1600},
    };
    return c;
}