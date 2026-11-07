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
                {"Reflected thrust", "reflecting yours",  8, false, true, 1600},
                {"Mirrored cut",     "matching your line",9, false, true, 1600},
                {"Heel echo",        "echoing the low cut",0, true, true, 1800},
            };
            break;
        case 1:
            c.name = "Mirrored Duelist";
            c.behavior = "mirror";
            c.attacks = {
                {"Your own feint", "aping your feint",     10, false, true, 1500},
                {"Your own sweep", "aping your sweep",     12, false, true, 1500},
                {"Your own heel cut","aping the low strike",0,  true, true, 1700},
            };
            break;
        default:
            c.name = "Lycomedes' Guard";
            c.attacks = {
                {"Phalanx spear", "stepping forward",  11, false, true, 1600},
                {"Shield bash",   "raising the shield", 9, false, true, 1600},
                {"Heel seeker",   "probing low",        0,  true, true, 1700},
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
        {"Sorrowful echo", "weeping",              9, false, true, 1700},
        {"Veil of memory", "raising the veil",     7, false, true, 1700},
        {"Grief's reach",  "reaching for the heel",0, true, true, 1900},
    };
    return c;
}