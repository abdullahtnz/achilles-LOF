#include "enemies.hpp"

Combatant make_l7_gate(int index) {
    Combatant c;
    c.hp_max = 120 + index * 18;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name     = "Divine Archer";
            c.behavior = "ranged";
            c.attacks = {
                {"Arrow of light", "drawing a sunlit bow", 17, false, true, 1400,
                 "Apollo sees you."},
                {"Guided shot",    "letting the god aim",  15, false, true, 1300},
                {"Heel shot",      "aiming at the ground",  0, true,  true, 1500,
                 "The god showed me the way."},
            };
            break;
        case 1:
            c.name = "Hector's Ghost";
            c.attacks = {
                {"Ghost spear", "rising from the gate", 16, false, true, 1400},
                {"Dead shield", "blocking the light",   14, false, true, 1400},
                {"Remembered cut","striking low",        0, true,  true, 1500,
                 "You dragged me."},
            };
            break;
        default:
            c.name = "Gate Guardian";
            c.attacks = {
                {"Gate blade",  "standing in the doorway", 18, false, true, 1400},
                {"Wall slam",   "using the gate itself",   15, false, true, 1400},
                {"Heel pierce", "striking beneath",         0, true,  true, 1600},
            };
            break;
    }
    return c;
}

Combatant make_l7_apollo_chosen() {
    Combatant c;
    c.name     = "Apollo's Chosen";
    c.behavior = "divine";
    c.hp_max   = 400;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Sunlit arrow",  "Apollo draws with him",   0, true,  true, 1500,
         "The heel, Paris. Always the heel."},
        {"Divine volley", "loosing three arrows",    20, false, true, 1400,
         "The god does not miss."},
        {"Guided shot",   "following the light",      0, true,  true, 1300,
         "I am not aiming. He is."},
        {"Piercing light","burning the air",         17, false, true, 1300,
         "You cannot outrun a god."},
        {"The last arrow","drawing the black arrow",  0, true,  true, 1700,
         "This is the one the story promised."},
        {"Apollo's will", "standing in the god's hand",21, false, true, 1400,
         "And stories must end."},
    };
    return c;
}
