#include "enemies.hpp"

Combatant make_l6_phantom(int index) {
    Combatant c;
    c.hp_max = 110 + index * 18;
    c.hp     = c.hp_max;
    switch (index) {
        case 0:
            c.name = "Past Death";
            c.attacks = {
                {"Remembered wound", "reaching for your chest", 15, false, true, 1500},
                {"Old fall",         "moving as you once did",   13, false, true, 1400},
                {"Heel memory",      "striking where it struck",   0, true,  true, 1600,
                 "You died here before."},
            };
            break;
        case 1:
            c.name = "Nameless Thread";
            c.attacks = {
                {"Unmade blow", "fraying at the edges",  14, false, true, 1400},
                {"Cut line",    "narrowing",             16, false, true, 1400},
                {"Thread pull", "catching your ankle",    0, true,  true, 1500},
            };
            break;
        default:
            c.name = "Phantom Achilles";
            c.attacks = {
                {"Your thrust",  "mirroring you perfectly", 18, false, true, 1400},
                {"Your fury",    "roaring",                  16, false, true, 1400},
                {"Your death",   "aiming low",                0, true,  true, 1500,
                 "Stay."},
            };
            break;
    }
    return c;
}

Combatant make_l6_moirai() {
    Combatant c;
    c.name     = "The Moirai";
    c.behavior = "fate";
    c.hp_max   = 380;
    c.hp       = c.hp_max;
    c.attacks = {
        {"Clotho spins",     "spinning a new thread",   16, false, true, 1700,
         "Spin the thread."},
        {"Lachesis measures","drawing the measure",     14, false, true, 1500,
         "Measure the life."},
        {"Atropos cuts",     "opening the shears",       0, true,  true, 1700,
         "Cut."},
        {"Three as one",     "moving in three directions",19, false, true, 1600,
         "We are not three. We are the ending."},
        {"Atropos cuts",     "finding the heel again",    0, true,  true, 1900,
         "Give us a reason not to."},
    };
    return c;
}
