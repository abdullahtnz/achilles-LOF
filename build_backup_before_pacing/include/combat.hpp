#pragma once
#include <cstdint>
#include <string>
#include <vector>

// ============================================================
// Actions available to the player — progressive unlock by layer
// ============================================================
enum class PlayerAction {
    None,
    Attack,
    Reckless,   // Layer 3+
    Focus,      // Layer 2+
    Dodge,
    Heal,
    Stance,     // Layer 2+
    Flee,
};

// ============================================================
// Stances — Layer 2+
// ============================================================
enum class Stance {
    Aggressive,
    Balanced,
    Defensive,
};
const char* stance_name(Stance s);

// ============================================================
// Attack — one enemy action
// ============================================================
struct Attack {
    std::string name;
    std::string verb;         // "coiling", "loosing an arrow", ...
    int         hp_damage   = 0;
    bool        is_heel     = false;
    bool        telegraphed = true;
    int         coil_ms     = 900;
};

// ============================================================
// Combatant
// ============================================================
struct Combatant {
    std::string name;
    int         hp           = 100;
    int         hp_max       = 100;
    int         stamina      = 3;
    int         stamina_max  = 3;
    int         rage         = 0;
    int         rage_max     = 100;
    std::vector<Attack> attacks;

    // Behavioral dispatch (Layer 2-3 enemies)
    std::string behavior;
    int         behavior_state = 0;

    // Combat depth additions
    Stance      stance       = Stance::Balanced;
    int         combo        = 0;
    int         heel_guards  = 0;
    bool        rage_active  = false;
    bool        focused      = false;   // pre-committed dodge
};

// ============================================================
// Companion
// ============================================================
struct Patroclus {
    bool active       = false;
    bool alive        = true;
    bool used_support = false;
};

// ============================================================
// God intervention — one fire per encounter, path-driven
// ============================================================
struct GodIntervention {
    std::string god;             // "ares" | "athena" | ""
    std::string message;         // shown when triggered
    int         turns_left  = 0; // duration
    bool        triggered   = false;
};

// ============================================================
// Outcomes
// ============================================================
enum class CombatOutcome {
    Ongoing,
    PlayerWon,
    PlayerDied,
    PlayerHeelHit,
    PlayerFled,
};

// ============================================================
// CombatSession
// ============================================================
class CombatSession {
public:
    CombatSession(Combatant player, Combatant enemy, Patroclus pat = {});
    CombatSession(Combatant player, Combatant enemy, Patroclus pat,
                  const std::string& god_ally); // "ares" | "athena" | ""

    void tick(long long dt_ms);
    bool submit(PlayerAction a);

    // Read-only view
    const Combatant& Player() const { return player_; }
    const Combatant& Enemy()  const { return enemy_; }
    const Patroclus& Companion() const { return pat_; }
    int              EnemyHP() const { return enemy_hp_; }
    CombatOutcome    Outcome() const { return outcome_; }

    const Attack&    CurrentAttack() const;
    bool             AttackInProgress() const;
    long long        CoilRemainingMs() const;
    long long        CoilTotalMs() const;

    // One-frame event flags
    bool             JustHitPlayer()  const { return just_hit_; }
    bool             JustDodged()     const { return just_dodged_; }
    bool             JustHitEnemy()   const { return just_hit_enemy_; }
    bool             JustGuardUsed()  const { return just_guard_used_; }

    std::string      LastEvent() const { return last_event_; }
    const GodIntervention& Intervention() const { return intervention_; }

    // Which actions are unlocked for the current layer (set by game_app)
    void set_unlocked_actions(int layer);

    // Query: is this action available?
    bool action_unlocked(PlayerAction a) const;

private:
    Combatant player_;
    Combatant enemy_;
    Patroclus pat_;
    int       enemy_hp_     = 0;
    int       enemy_idx_    = 0;
    CombatOutcome outcome_  = CombatOutcome::Ongoing;

    bool      attack_active_ = false;
    long long coil_elapsed_ms_ = 0;
    int       current_attack_index_ = 0;

    bool      dodge_pending_ = false;
    long long dodge_window_ms_ = 400;

    // One-frame event flags
    bool      just_hit_        = false;
    bool      just_dodged_     = false;
    bool      just_hit_enemy_  = false;
    bool      just_guard_used_ = false;
    std::string last_event_;

    long long since_player_action_ms_ = 0;
    long long player_action_cooldown_ms_ = 150;

    PlayerAction last_player_action_ = PlayerAction::None;
    GodIntervention intervention_;

    // Layer gate
    int       unlocked_layer_ = 1;

    // Internals
    void resolve_enemy_attack();
    void next_enemy_attack();
    void maybe_patroclus_support();
    void apply_behavior_on_player_action(PlayerAction a);
    void apply_behavior_on_enemy_hit();
    void maybe_trigger_intervention();
    int  effective_player_damage(int base);
    double stance_damage_mult() const;
    double stance_heel_mult() const;
};