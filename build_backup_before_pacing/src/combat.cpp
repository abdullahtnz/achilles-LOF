#include "combat.hpp"
#include <algorithm>
#include <cstdlib>

const char* stance_name(Stance s) {
    switch (s) {
        case Stance::Aggressive: return "Aggressive";
        case Stance::Balanced:   return "Balanced";
        case Stance::Defensive:  return "Defensive";
    }
    return "Balanced";
}

// -------------------------------------------------------------
// Constructors
// -------------------------------------------------------------

CombatSession::CombatSession(Combatant p, Combatant e, Patroclus pat)
    : player_(std::move(p)), enemy_(std::move(e)), pat_(pat) {
    enemy_hp_ = enemy_.hp;
    if (!enemy_.attacks.empty()) next_enemy_attack();
    maybe_trigger_intervention();
}

CombatSession::CombatSession(Combatant p, Combatant e, Patroclus pat,
                             const std::string& god_ally)
    : player_(std::move(p)), enemy_(std::move(e)), pat_(pat) {
    enemy_hp_ = enemy_.hp;
    if (!enemy_.attacks.empty()) next_enemy_attack();

    if (god_ally == "ares") {
        intervention_.god = "ares";
        intervention_.message = "Ares whispers: no more dodging. Only blood.";
        intervention_.turns_left = 5;
        intervention_.triggered = true;
    } else if (god_ally == "athena") {
        intervention_.god = "athena";
        intervention_.message = "Athena steadies your hand. Three strikes cannot miss.";
        intervention_.turns_left = 3;
        intervention_.triggered = true;
    }
}

// -------------------------------------------------------------
// Action availability (progressive unlock)
// -------------------------------------------------------------

void CombatSession::set_unlocked_actions(int layer) {
    unlocked_layer_ = layer;
}

bool CombatSession::action_unlocked(PlayerAction a) const {
    switch (a) {
        case PlayerAction::Attack:
        case PlayerAction::Dodge:
        case PlayerAction::Heal:
        case PlayerAction::Flee:
            return true;                       // always
        case PlayerAction::Focus:
        case PlayerAction::Stance:
            return unlocked_layer_ >= 2;       // Layer 2+
        case PlayerAction::Reckless:
            return unlocked_layer_ >= 3;       // Layer 3+
        default:
            return false;
    }
}

// -------------------------------------------------------------
// Coil helpers
// -------------------------------------------------------------

const Attack& CombatSession::CurrentAttack() const {
    static Attack none;
    if (enemy_.attacks.empty()) return none;
    return enemy_.attacks[current_attack_index_ % enemy_.attacks.size()];
}

bool CombatSession::AttackInProgress() const { return attack_active_; }

long long CombatSession::CoilRemainingMs() const {
    if (!attack_active_) return 0;
    return std::max(0LL, CurrentAttack().coil_ms - coil_elapsed_ms_);
}

long long CombatSession::CoilTotalMs() const { return CurrentAttack().coil_ms; }

// -------------------------------------------------------------
// Stance multipliers
// -------------------------------------------------------------

double CombatSession::stance_damage_mult() const {
    switch (player_.stance) {
        case Stance::Aggressive: return 1.5;
        case Stance::Balanced:   return 1.0;
        case Stance::Defensive:  return 0.7;
    }
    return 1.0;
}

double CombatSession::stance_heel_mult() const {
    switch (player_.stance) {
        case Stance::Aggressive: return 1.3;
        case Stance::Balanced:   return 1.0;
        case Stance::Defensive:  return 0.7;
    }
    return 1.0;
}

int CombatSession::effective_player_damage(int base) {
    double dmg = base * stance_damage_mult();
    // Combo bonus at 3: Heroic Blow (applied by caller — see submit()).
    if (intervention_.god == "ares" && intervention_.turns_left > 0)
        dmg *= 1.5;
    return (int)dmg;
}

// -------------------------------------------------------------
// Attack scheduling
// -------------------------------------------------------------

void CombatSession::next_enemy_attack() {
    attack_active_ = true;
    coil_elapsed_ms_ = 0;
}

void CombatSession::maybe_trigger_intervention() {
    // Interventions already seeded via the second constructor.
    // (Random mid-fight triggers are for a later step.)
}

// -------------------------------------------------------------
// Tick
// -------------------------------------------------------------

void CombatSession::tick(long long dt_ms) {
    just_hit_ = false;
    just_dodged_ = false;
    just_hit_enemy_ = false;
    just_guard_used_ = false;

    if (outcome_ != CombatOutcome::Ongoing) return;

    since_player_action_ms_ += dt_ms;

    // Decrement god intervention duration
    if (intervention_.turns_left > 0) {
        // Decrement once per *enemy attack cycle*, done in resolve.
    }

    if (dodge_pending_) {
        dodge_window_ms_ -= dt_ms;
        if (dodge_window_ms_ <= 0) {
            dodge_pending_ = false;
            dodge_window_ms_ = 400;
        }
    }

    if (!attack_active_) {
        next_enemy_attack();
        return;
    }

    coil_elapsed_ms_ += dt_ms;
    if (coil_elapsed_ms_ >= CurrentAttack().coil_ms) {
        resolve_enemy_attack();
    }
}

void CombatSession::resolve_enemy_attack() {
    const Attack& a = CurrentAttack();

    // God intervention: if Ares is active, dodging is disabled.
    bool dodge_disabled = (intervention_.god == "ares" && intervention_.turns_left > 0);

    // Order of heel defence:
    //   1. Stance Defensive (immunity window)
    //   2. Focus pre-committed dodge
    //   3. Dodge window (if not disabled)
    //   4. Heel Guard token (only against heel strikes)
    //   5. Take damage / die

    bool negate = false;

    if (a.is_heel) {
        if (player_.stance == Stance::Defensive) {
            negate = true;
            last_event_ = "You brace — the heel is safe.";
        } else if (player_.focused) {
            negate = true;
            player_.focused = false;
            last_event_ = "You focus — the blow slips past.";
        } else if (dodge_pending_ && !dodge_disabled) {
            negate = true;
            just_dodged_ = true;
            last_event_ = "You slip aside.";
        } else if (player_.heel_guards > 0) {
            player_.heel_guards -= 1;
            just_guard_used_ = true;
            negate = true;
            last_event_ = "Thetis's hand is on you. The heel holds.";
        }
    } else {
        // Non-heel damage
        if (dodge_pending_ && !dodge_disabled) {
            just_dodged_ = true;
            last_event_ = "You slip aside.";
            negate = true;
        } else if (player_.focused) {
            player_.focused = false;
            last_event_ = "You focus — the blow is read.";
            negate = true;
        }
    }

    if (!negate) {
        if (a.is_heel) {
            last_event_ = "The heel is struck.";
            outcome_ = CombatOutcome::PlayerHeelHit;
            attack_active_ = false;
            return;
        }
        int dmg = a.hp_damage;
        if (player_.stance == Stance::Aggressive) dmg = (int)(dmg * 1.2);
        if (player_.stance == Stance::Defensive)  dmg = (int)(dmg * 0.7);
        if (intervention_.god == "ares" && intervention_.turns_left > 0)
            dmg = (int)(dmg * 1.5);

        player_.hp -= dmg;
        just_hit_ = true;
        last_event_ = enemy_.name + " hits you for " + std::to_string(dmg) + ".";
        if (player_.hp <= 0) {
            outcome_ = CombatOutcome::PlayerDied;
            attack_active_ = false;
            return;
        }
    }

    // Reset dodge / focus
    dodge_pending_ = false;
    dodge_window_ms_ = 400;

    // Advance attack cycle
    attack_active_ = false;
    current_attack_index_++;
    player_.stamina = std::min(player_.stamina_max, player_.stamina + 1);

    // Decrement intervention duration
    if (intervention_.turns_left > 0) intervention_.turns_left--;

    apply_behavior_on_enemy_hit();
    maybe_patroclus_support();
}

// -------------------------------------------------------------
// Behaviors (unchanged from Step 3 — mirror / grief / shield_wall)
// -------------------------------------------------------------

void CombatSession::maybe_patroclus_support() {
    if (!pat_.active || !pat_.alive || pat_.used_support) return;
    if (player_.hp < player_.hp_max / 2) {
        player_.hp = std::min(player_.hp_max, player_.hp + 10);
        pat_.used_support = true;
        last_event_ += "  Patroclus steadies you (+10 HP).";
    } else if (enemy_hp_ < enemy_.hp_max / 2 && !enemy_.attacks.empty()) {
        enemy_.attacks[current_attack_index_ % enemy_.attacks.size()].coil_ms += 300;
        pat_.used_support = true;
        last_event_ += "  Patroclus calls out a warning.";
    }
}

void CombatSession::apply_behavior_on_player_action(PlayerAction a) {
    last_player_action_ = a;
    if (enemy_.behavior == "grief") {
        if (a != PlayerAction::Attack && a != PlayerAction::Reckless) {
            enemy_.behavior_state++;
            if (enemy_.behavior_state >= 3) {
                player_.hp -= 40;
                just_hit_ = true;
                last_event_ = "Deidamia's grief breaks over you (40 damage).";
                enemy_.behavior_state = 0;
                if (player_.hp <= 0) {
                    outcome_ = CombatOutcome::PlayerDied;
                }
            }
        }
    }
}

void CombatSession::apply_behavior_on_enemy_hit() {
    // Reserved for future per-behavior reactions.
}

// -------------------------------------------------------------
// Submit player action
// -------------------------------------------------------------

bool CombatSession::submit(PlayerAction action) {
    if (outcome_ != CombatOutcome::Ongoing) return false;
    if (action == PlayerAction::None)     return false;
    if (!action_unlocked(action))         return false;
    if (since_player_action_ms_ < player_action_cooldown_ms_) return false;
    since_player_action_ms_ = 0;

    switch (action) {
        case PlayerAction::Attack: {
            int dmg = 12 + std::rand() % 7;

            // Mirror behavior
            if (enemy_.behavior == "mirror" &&
                last_player_action_ == PlayerAction::Attack) {
                player_.hp -= 6;
                last_event_ = "The illusion mirrors your blow (6 back).";
                if (player_.hp <= 0) {
                    outcome_ = CombatOutcome::PlayerDied;
                    return true;
                }
                break;
            }

            // Shield wall
            if (enemy_.behavior == "shield_wall" && enemy_.behavior_state < 3) {
                enemy_.behavior_state++;
                last_event_ = "Shield absorbs the blow (" +
                              std::to_string(3 - enemy_.behavior_state) +
                              " more to break).";
                break;
            }

            // Combo — three consecutive attacks → Heroic Blow
            player_.combo++;
            if (player_.combo >= 3) {
                dmg = effective_player_damage(dmg) * 2;
                last_event_ = "HEROIC BLOW \u2014 " + std::to_string(dmg) + "!";
                player_.combo = 0;
                player_.rage = std::min(player_.rage_max, player_.rage + 10);
            } else {
                dmg = effective_player_damage(dmg);
                last_event_ = "You strike for " + std::to_string(dmg) + ".";
            }

            // Combo also raises heel-risk: reflected in stance multiplier during
            // enemy resolve. No extra state needed.
            enemy_hp_ -= dmg;
            just_hit_enemy_ = true;
            if (enemy_hp_ <= 0) {
                enemy_hp_ = 0;
                outcome_ = CombatOutcome::PlayerWon;
            }
            break;
        }

        case PlayerAction::Reckless: {
            // 20 HP cost, 3x damage, disables dodge next turn (heel exposed).
            if (player_.hp <= 20) {
                last_event_ = "Too weak to strike recklessly.";
                return false;
            }
            player_.hp -= 20;
            int dmg = 12 + std::rand() % 7;
            dmg = effective_player_damage(dmg) * 3;
            enemy_hp_ -= dmg;
            just_hit_enemy_ = true;
            last_event_ = "RECKLESS \u2014 " + std::to_string(dmg) +
                          " damage, but your heel is exposed.";
            // Penalise next turn's dodge by clearing dodge_pending_ at resolve.
            // Implemented by forcibly setting dodge_window_ms_ low.
            dodge_pending_ = false;
            player_.rage = std::min(player_.rage_max, player_.rage + 15);
            if (enemy_hp_ <= 0) {
                enemy_hp_ = 0;
                outcome_ = CombatOutcome::PlayerWon;
            }
            break;
        }

        case PlayerAction::Focus: {
            if (player_.stamina < 1) {
                last_event_ = "Not enough stamina to focus.";
                return false;
            }
            player_.stamina -= 1;
            player_.focused = true;
            player_.combo = 0;
            last_event_ = "You breathe. The next blow will be read.";
            break;
        }

        case PlayerAction::Dodge: {
            if (player_.stamina < 1) {
                last_event_ = "Not enough stamina to dodge.";
                return false;
            }
            player_.stamina -= 1;
            dodge_pending_ = true;
            dodge_window_ms_ = (player_.name.find("Athena") != std::string::npos)
                                ? 600 : 400;
            last_event_ = "You read the motion.";
            player_.combo = 0;
            break;
        }

        case PlayerAction::Heal: {
            if (player_.stamina < 1) {
                last_event_ = "Not enough stamina to heal.";
                return false;
            }
            player_.stamina -= 1;
            int before = player_.hp;
            player_.hp = std::min(player_.hp_max, player_.hp + 20);
            last_event_ = "You steady your breath (+" +
                          std::to_string(player_.hp - before) + ").";
            player_.combo = 0;
            break;
        }

        case PlayerAction::Stance: {
            switch (player_.stance) {
                case Stance::Balanced:   player_.stance = Stance::Aggressive; break;
                case Stance::Aggressive: player_.stance = Stance::Defensive;  break;
                case Stance::Defensive:  player_.stance = Stance::Balanced;   break;
            }
            last_event_ = std::string("Stance: ") + stance_name(player_.stance) + ".";
            break;
        }

        case PlayerAction::Flee: {
            outcome_ = CombatOutcome::PlayerFled;
            last_event_ = "You withdraw.";
            break;
        }

        default: break;
    }

    apply_behavior_on_player_action(action);
    return true;
}