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
    begin_pre_coil();
}

CombatSession::CombatSession(Combatant p, Combatant e, Patroclus pat,
                             const std::string& god_ally)
    : player_(std::move(p)), enemy_(std::move(e)), pat_(pat) {
    enemy_hp_ = enemy_.hp;
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
    begin_pre_coil();
}

// -------------------------------------------------------------
// Action availability
// -------------------------------------------------------------

void CombatSession::set_unlocked_actions(int layer) { unlocked_layer_ = layer; }

bool CombatSession::action_unlocked(PlayerAction a) const {
    switch (a) {
        case PlayerAction::Attack:
        case PlayerAction::Dodge:
        case PlayerAction::Heal:
        case PlayerAction::Flee:
            return true;
        case PlayerAction::Focus:
        case PlayerAction::Stance:
            return unlocked_layer_ >= 2;
        case PlayerAction::Reckless:
            return unlocked_layer_ >= 3;
        default:
            return false;
    }
}

// -------------------------------------------------------------
// Pacing
// -------------------------------------------------------------

long long CombatSession::adjusted_coil_ms(int base_coil_ms) const {
    int penalty = consecutive_attacks_ * 150;
    long long result = base_coil_ms - penalty;
    return std::max(400LL, result);
}

long long CombatSession::current_dodge_window_ms() const {
    long long base = (player_.name.find("Athena") != std::string::npos) ? 600 : 400;
    long long penalty = (consecutive_attacks_ - 1) * 80;
    if (penalty < 0) penalty = 0;
    long long result = base - penalty;
    return std::max(150LL, result);
}

void CombatSession::register_player_attack() {
    consecutive_attacks_++;
}

void CombatSession::reset_consecutive_attacks() {
    consecutive_attacks_ = 0;
}

void CombatSession::begin_idle() {
    phase_ = Phase::Idle;
    phase_elapsed_ms_ = 0;
    attack_active_ = false;
}

void CombatSession::begin_pre_coil() {
    phase_ = Phase::PreCoil;
    phase_elapsed_ms_ = 0;
    attack_active_ = false;

    if (enemy_.attacks.empty()) {
        pending_coil_ms_ = 0;
        return;
    }
    const Attack& next = enemy_.attacks[current_attack_index_ % enemy_.attacks.size()];
    pending_coil_ms_ = (int)adjusted_coil_ms(next.coil_ms);
}

void CombatSession::begin_coil() {
    phase_ = Phase::Coil;
    phase_elapsed_ms_ = 0;
    attack_active_ = true;
    coil_elapsed_ms_ = 0;
}

// -------------------------------------------------------------
// Coil queries
// -------------------------------------------------------------

const Attack& CombatSession::CurrentAttack() const {
    static Attack none;
    if (enemy_.attacks.empty()) return none;
    return enemy_.attacks[current_attack_index_ % enemy_.attacks.size()];
}

bool CombatSession::AttackInProgress() const { return attack_active_; }

long long CombatSession::CoilRemainingMs() const {
    if (!attack_active_) return 0;
    return std::max(0LL, (long long)pending_coil_ms_ - coil_elapsed_ms_);
}

long long CombatSession::CoilTotalMs() const { return pending_coil_ms_; }

bool CombatSession::InIdlePhase() const { return phase_ == Phase::Idle; }
long long CombatSession::IdleRemainingMs() const {
    if (phase_ != Phase::Idle) return 0;
    return std::max(0LL, POST_ATTACK_IDLE_MS - phase_elapsed_ms_);
}
bool CombatSession::InPreCoilPhase() const { return phase_ == Phase::PreCoil; }
long long CombatSession::PreCoilRemainingMs() const {
    if (phase_ != Phase::PreCoil) return 0;
    return std::max(0LL, PRE_HEEL_COIL_MS - phase_elapsed_ms_);
}

std::string CombatSession::EnemySpeech() const {
    if (phase_ != Phase::Coil) return "";
    const Attack& a = CurrentAttack();
    return a.speech;
}

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
    if (intervention_.god == "ares" && intervention_.turns_left > 0)
        dmg *= 1.5;
    return (int)dmg;
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

    if (dodge_pending_) {
        dodge_window_ms_ -= dt_ms;
        if (dodge_window_ms_ <= 0) {
            dodge_pending_ = false;
        }
    }

    phase_elapsed_ms_ += dt_ms;

    switch (phase_) {
        case Phase::Idle:
            if (phase_elapsed_ms_ >= POST_ATTACK_IDLE_MS) {
                begin_pre_coil();
            }
            break;

        case Phase::PreCoil:
            if (phase_elapsed_ms_ >= PRE_HEEL_COIL_MS) {
                begin_coil();
            }
            break;

        case Phase::Coil:
            coil_elapsed_ms_ += dt_ms;
            if (coil_elapsed_ms_ >= pending_coil_ms_) {
                resolve_enemy_attack();
            }
            break;
    }
}

// -------------------------------------------------------------
// Resolution
// -------------------------------------------------------------

void CombatSession::resolve_enemy_attack() {
    const Attack& a = CurrentAttack();

    bool dodge_disabled = (intervention_.god == "ares" && intervention_.turns_left > 0);

    bool negate = false;

    if (a.is_heel) {
        if (player_.stance == Stance::Defensive) {
            negate = true;
            last_event_ = "You brace \u2014 the heel is safe.";
        } else if (player_.focused) {
            negate = true;
            player_.focused = false;
            last_event_ = "You focus \u2014 the blow slips past.";
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
        if (dodge_pending_ && !dodge_disabled) {
            just_dodged_ = true;
            last_event_ = "You slip aside.";
            negate = true;
        } else if (player_.focused) {
            player_.focused = false;
            last_event_ = "You focus \u2014 the blow is read.";
            negate = true;
        }
    }

    if (!negate) {
        if (a.is_heel) {
            last_event_ = "The heel is struck.";
            outcome_ = CombatOutcome::PlayerHeelHit;
            attack_active_ = false;

            // Log the heel hit
            if (!last_event_.empty()) {
                event_log_.push_back(last_event_);
                if (event_log_.size() > 3) event_log_.erase(event_log_.begin());
            }
            return;
        }
        int dmg = a.hp_damage;
        if (player_.stance == Stance::Aggressive) dmg = (int)(dmg * 1.2);
        if (player_.stance == Stance::Defensive)  dmg = (int)(dmg * 0.7);
        if (intervention_.god == "ares" && intervention_.turns_left > 0)
            dmg = (int)(dmg * 1.5);
        if (player_.rage > 40) dmg += 2;

        player_.hp -= dmg;
        just_hit_ = true;
        last_event_ = enemy_.name + " hits you for " + std::to_string(dmg) + ".";
        if (player_.hp <= 0) {
            outcome_ = CombatOutcome::PlayerDied;
            attack_active_ = false;
            if (!last_event_.empty()) {
                event_log_.push_back(last_event_);
                if (event_log_.size() > 3) event_log_.erase(event_log_.begin());
            }
            return;
        }
    }

    dodge_pending_ = false;
    player_.stamina = std::min(player_.stamina_max, player_.stamina + 1);

    if (intervention_.turns_left > 0) intervention_.turns_left--;

    apply_behavior_on_enemy_hit();
    maybe_patroclus_support();

    // Log the event
    if (!last_event_.empty()) {
        if (event_log_.empty() || event_log_.back() != last_event_) {
            event_log_.push_back(last_event_);
            if (event_log_.size() > 3) event_log_.erase(event_log_.begin());
        }
    }

    advance_to_next_attack();
}

void CombatSession::advance_to_next_attack() {
    attack_active_ = false;
    current_attack_index_++;

    if (last_player_action_ != PlayerAction::Attack &&
        last_player_action_ != PlayerAction::Reckless) {
        reset_consecutive_attacks();
    }

    begin_idle();
}

// -------------------------------------------------------------
// Behaviors
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
                if (player_.hp <= 0) outcome_ = CombatOutcome::PlayerDied;
            }
        }
    }
}

void CombatSession::apply_behavior_on_enemy_hit() {}

// -------------------------------------------------------------
// Submit
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

            if (enemy_.behavior == "mirror" &&
                last_player_action_ == PlayerAction::Attack) {
                player_.hp -= 6;
                last_event_ = "The illusion mirrors your blow (6 back).";
                register_player_attack();
                if (player_.hp <= 0) { outcome_ = CombatOutcome::PlayerDied; return true; }
                break;
            }

            if (enemy_.behavior == "shield_wall" && enemy_.behavior_state < 3) {
                enemy_.behavior_state++;
                last_event_ = "Shield absorbs the blow (" +
                              std::to_string(3 - enemy_.behavior_state) +
                              " more to break).";
                register_player_attack();
                break;
            }

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

            register_player_attack();
            enemy_hp_ -= dmg;
            just_hit_enemy_ = true;
            if (enemy_hp_ <= 0) { enemy_hp_ = 0; outcome_ = CombatOutcome::PlayerWon; }
            break;
        }

        case PlayerAction::Reckless: {
            if (player_.hp <= 20) { last_event_ = "Too weak to strike recklessly."; return false; }
            player_.hp -= 20;
            int dmg = 12 + std::rand() % 7;
            dmg = effective_player_damage(dmg) * 3;
            enemy_hp_ -= dmg;
            just_hit_enemy_ = true;
            last_event_ = "RECKLESS \u2014 " + std::to_string(dmg) +
                          " damage, but your heel is exposed.";
            dodge_pending_ = false;
            player_.rage = std::min(player_.rage_max, player_.rage + 15);
            register_player_attack();
            if (enemy_hp_ <= 0) { enemy_hp_ = 0; outcome_ = CombatOutcome::PlayerWon; }
            break;
        }

        case PlayerAction::Focus: {
            if (player_.stamina < 1) { last_event_ = "Not enough stamina to focus."; return false; }
            player_.stamina -= 1;
            player_.focused = true;
            player_.combo = 0;
            last_event_ = "You breathe. The next blow will be read.";
            reset_consecutive_attacks();
            break;
        }

        case PlayerAction::Dodge: {
            if (player_.stamina < 1) { last_event_ = "Not enough stamina to dodge."; return false; }
            player_.stamina -= 1;
            dodge_pending_ = true;
            dodge_window_ms_ = current_dodge_window_ms();
            last_event_ = "You read the motion.";
            player_.combo = 0;
            reset_consecutive_attacks();
            break;
        }

        case PlayerAction::Heal: {
            if (player_.stamina < 1) { last_event_ = "Not enough stamina to heal."; return false; }
            player_.stamina -= 1;
            int before = player_.hp;
            player_.hp = std::min(player_.hp_max, player_.hp + 20);
            last_event_ = "You steady your breath (+" +
                          std::to_string(player_.hp - before) + ").";
            player_.combo = 0;
            reset_consecutive_attacks();
            break;
        }

        case PlayerAction::Stance: {
            switch (player_.stance) {
                case Stance::Balanced:   player_.stance = Stance::Aggressive; break;
                case Stance::Aggressive: player_.stance = Stance::Defensive;  break;
                case Stance::Defensive:  player_.stance = Stance::Balanced;   break;
            }
            last_event_ = std::string("Stance: ") + stance_name(player_.stance) + ".";
            reset_consecutive_attacks();
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

    // Log the event
    if (!last_event_.empty()) {
        if (event_log_.empty() || event_log_.back() != last_event_) {
            event_log_.push_back(last_event_);
            if (event_log_.size() > 3) event_log_.erase(event_log_.begin());
        }
    }

    return true;
}