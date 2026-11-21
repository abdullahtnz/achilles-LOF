#include "enemies.hpp"

Combatant make_l4_river(int index) {
    Combatant c;
    c.hp_max = 85 + index * 15;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Drowning Soldier";
            c.attacks = {
                {"River thrust",  "rising from the shallows", 14, false, true, 1600},
                {"Mud grasp",     "reaching up",              12, false, true, 1500},
                {"Heel pull",     "closing on your ankle",     0, true,  true, 1800,
                 "The river wants you."},
            };
            break;
        case 1:
            c.name = "River Nymph";
            c.attacks = {
                {"Current lash",  "gathering water",          13, false, true, 1500},
                {"Drowning song", "singing",                  11, false, true, 1500},
                {"Undertow",      "coiling beneath you",       0, true,  true, 1700,
                 "Come into the water."},
            };
            break;
        default:
            c.name = "Corpse-Puppet";
            c.attacks = {
                {"Dead grip",     "stiffening",               16, false, true, 1600},
                {"Bone flail",    "swinging an arm",          14, false, true, 1500},
                {"Ankle bite",    "dropping its head",         0, true,  true, 1600},
            };
            break;
    }
    return c;
}

Combatant make_l4_scamander() {
    Combatant c;
    c.name     = "Scamander, the River God";
    c.behavior = "flood";
    c.hp_max   = 300;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Wave crash",   "rising into a wall of water", 18, false, true, 1800,
         "You choke me with your dead."},
        {"Undertow",     "pulling at your feet",         0, true,  true, 1700,
         "The water is patient. I am not."},
        {"River spear",  "hardening the current",       16, false, true, 1600,
         "Every stone in my throat has a name."},
        {"Deep current", "coiling below the surface",    0, true,  true, 2000,
         "Drown, son of Thetis. Drown and be quiet."},
    };
    return c;
}
