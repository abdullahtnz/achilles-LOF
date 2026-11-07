#include "combat.hpp"
#include <algorithm>
#include <cstdlib>

CombatSession::CombatSession(Combatant p, Combatant e, Patroclus pat)
    : player_(std::move(p)), enemy_(std::move(e)), pat_(pat) {
    enemy_hp_ = enemy_.hp;
    if (!enemy_.attacks.empty()) next_enemy_attack();
}

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

void CombatSession::next_enemy_attack() {
    attack_active_ = true;
    coil_elapsed_ms_ = 0;
}

void CombatSession::tick(long long dt_ms) {
    just_hit_ = false;
    just_dodged_ = false;
    just_hit_enemy_ = false;

    if (outcome_ != CombatOutcome::Ongoing) return;

    since_player_action_ms_ += dt_ms;

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

    if (dodge_pending_) {
        just_dodged_ = true;
        last_event_ = "You slip aside.";
        dodge_pending_ = false;
        dodge_window_ms_ = 400;
    } else if (a.is_heel) {
        last_event_ = "The heel is struck.";
        outcome_ = CombatOutcome::PlayerHeelHit;
        attack_active_ = false;
        return;
    } else {
        player_.hp -= a.hp_damage;
        just_hit_ = true;
        last_event_ = enemy_.name + " hits you for " +
                      std::to_string(a.hp_damage) + ".";
        if (player_.hp <= 0) {
            outcome_ = CombatOutcome::PlayerDied;
            attack_active_ = false;
            return;
        }
    }

    attack_active_ = false;
    current_attack_index_++;
    player_.stamina = std::min(player_.stamina_max, player_.stamina + 1);

    apply_behavior_on_enemy_hit();
    maybe_patroclus_support();
}

void CombatSession::maybe_patroclus_support() {
    if (!pat_.active || !pat_.alive || pat_.used_support) return;
    // One support action per encounter: heals player 10 HP when they drop
    // below 50%, or slows the enemy (adds 300ms to next coil).
    if (player_.hp < player_.hp_max / 2) {
        player_.hp = std::min(player_.hp_max, player_.hp + 10);
        pat_.used_support = true;
        last_event_ += "  Patroclus steadies you (+10 HP).";
    } else if (enemy_hp_ < enemy_.hp_max / 2 && !enemy_.attacks.empty()) {
        // Slow the enemy's next coil.
        enemy_.attacks[current_attack_index_ % enemy_.attacks.size()].coil_ms += 300;
        pat_.used_support = true;
        last_event_ += "  Patroclus calls out a warning.";
    }
}

void CombatSession::apply_behavior_on_player_action(PlayerAction a) {
    last_player_action_ = a;
    if (enemy_.behavior == "grief") {
        // Deidamia: charges when player does NOT attack. If player skips
        // attacking, her grief grows. At 3, she releases a massive hit.
        if (a != PlayerAction::Attack) {
            enemy_.behavior_state++;
            if (enemy_.behavior_state >= 3) {
                // Release grief.
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
    if (enemy_.behavior == "shield_wall") {
        // Reset the shield counter when player hits (handled in submit()).
        // Nothing here — see submit Attack branch.
    }
    if (enemy_.behavior == "ranged") {
        // When the enemy lands a hit, the player is pushed back — closing
        // distance must be re-earned. This doesn't change the fight directly
        // (the ranged rule lives in submit Dodge), just a flavor hook.
    }
}

bool CombatSession::submit(PlayerAction action) {
    if (outcome_ != CombatOutcome::Ongoing) return false;
    if (action == PlayerAction::None) return false;
    if (since_player_action_ms_ < player_action_cooldown_ms_) return false;
    since_player_action_ms_ = 0;

    switch (action) {
        case PlayerAction::Attack: {
            int dmg = 12 + std::rand() % 7;

            // Mirror: if enemy is "mirror" and player attacked last turn,
            // the enemy reflects — player takes 6.
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

            // Shield wall: absorb damage until shield breaks (3 consecutive
            // attacks break it).
            if (enemy_.behavior == "shield_wall" && enemy_.behavior_state < 3) {
                enemy_.behavior_state++;
                last_event_ = "Shield absorbs the blow (" +
                              std::to_string(3 - enemy_.behavior_state) +
                              " more to break).";
                break;
            }

            enemy_hp_ -= dmg;
            just_hit_enemy_ = true;
            last_event_ = "You strike for " + std::to_string(dmg) + ".";
            if (enemy_hp_ <= 0) {
                enemy_hp_ = 0;
                outcome_ = CombatOutcome::PlayerWon;
            }
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
                                ? 600 : 400;   // Athena boon widens window
            last_event_ = "You read the motion.";
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