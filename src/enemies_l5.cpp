#include "enemies.hpp"

Combatant make_l5_wall(int index) {
    Combatant c;
    c.hp_max = 95 + index * 18;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Wall Defender";
            c.attacks = {
                {"Gate spear",   "bracing against the wall", 15, false, true, 1500},
                {"Shield shove", "stepping in",              13, false, true, 1500},
                {"Low stab",     "dropping the point",        0, true,  true, 1700},
            };
            break;
        case 1:
            c.name     = "Apollo's Archer";
            c.behavior = "ranged";
            c.attacks = {
                {"Sunlit arrow", "drawing in the light",      16, false, true, 1500,
                 "Apollo guides my hand."},
                {"Aimed shot",   "waiting for your step",     12, false, true, 1400},
                {"Heel shot",    "aiming at the ground",       0, true,  true, 1600,
                 "The god showed me where to aim."},
            };
            break;
        default:
            c.name = "Honor Guard";
            c.attacks = {
                {"Guard's spear", "advancing in step",        16, false, true, 1500},
                {"Shield wall",   "locking shields",          12, false, true, 1500},
                {"Heel breaker",  "striking beneath the shield",0, true, true, 1600},
            };
            break;
    }
    return c;
}

Combatant make_l5_hector() {
    Combatant c;
    c.name     = "Hector, Prince of Troy";
    c.behavior = "honor";
    c.hp_max   = 340;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Spear of Troy", "leveling his spear",        18, false, true, 1700,
         "For my city."},
        {"Shield answer", "raising the great shield",  15, false, true, 1600,
         "You are not the only one who can be brave."},
        {"Honor's edge",  "stepping inside your guard",17, false, true, 1600},
        {"Heel cut",      "dropping his point",         0, true,  true, 1800,
         "I am sorry. This is the only way I win."},
        {"Last stand",    "setting his feet",          20, false, true, 1800,
         "For Astyanax."},
    };
    return c;
}
