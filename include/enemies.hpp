#pragma once
#include "combat.hpp"
#include <string>

Combatant make_l1_phantom(int index);
Combatant make_l1_chiron();

Combatant make_l2_illusion(int index);
Combatant make_l2_deidamia();

Combatant make_l3_trojan(int index);
Combatant make_l3_hector_vanguard();

Combatant make_l4_river(int index);
Combatant make_l4_scamander();

Combatant make_l5_wall(int index);
Combatant make_l5_hector();

Combatant make_l6_phantom(int index);
Combatant make_l6_moirai();

Combatant make_l7_gate(int index);
Combatant make_l7_apollo_chosen();

// True for the seven named layer bosses.
bool is_boss_name(const std::string& name);