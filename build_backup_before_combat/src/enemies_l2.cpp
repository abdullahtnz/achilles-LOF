#include "enemies.hpp"

Combatant make_l2_illusion(int index) {
    Combatant c;
    c.hp_max = 55 + index * 15;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Illusion";
            c.behavior = "mirror";
            c.attacks = {
                {"Reflected thrust",   8,  false, true, 900},
                {"Mirrored cut",       9,  false, true, 900},
                {"Heel echo",          0,  true,  true, 1300},
            };
            break;
        case 1:
            c.name = "Mirrored Duelist";
            c.behavior = "mirror";
            c.attacks = {
                {"Your own feint",     10, false, true, 850},
                {"Your own sweep",     12, false, true, 850},
                {"Your own heel cut",  0,  true,  true, 1100},
            };
            break;
        default:
            c.name = "Lycomedes' Guard";
            c.attacks = {
                {"Phalanx spear",      11, false, true, 850},
                {"Shield bash",        9,  false, true, 850},
                {"Heel seeker",        0,  true,  true, 950},
            };
            break;
    }
    return c;
}

Combatant make_l2_deidamia() {
    Combatant c;
    c.name     = "Deidamia's Grief";
    c.behavior = "grief";
    c.hp_max   = 200;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Sorrowful echo",   9,  false, true, 1000},
        {"Veil of memory",   7,  false, true, 900},
        {"Grief's reach",    0,  true,  true, 1200},
    };
    return c;
}