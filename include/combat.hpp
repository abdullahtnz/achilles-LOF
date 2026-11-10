#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class PlayerAction {
    None,
    Attack,
    Reckless,
    Focus,
    Dodge,
    Heal,
    Stance,
    Flee,
};

enum class Stance {
    Aggressive,
    Balanced,
    Defensive,
};
const char* stance_name(Stance s);

struct Attack {
    std::string name;
    std::string verb;
    int         hp_damage   = 0;
    bool        is_heel     = false;
    bool        telegraphed = true;
    int         coil_ms     = 900;
    std::string speech;   // said before this attack (bosses only, filled in Step 3)
};

struct Combatant {
    std::string name;
    int         hp           = 100;
    int         hp_max       = 100;
    int         stamina      = 3;
    int         stamina_max  = 3;
    int         rage         = 0;
    int         rage_max     = 100;
    std::vector<Attack> attacks;

    std::string behavior;
    int         behavior_state = 0;

    Stance      stance       = Stance::Balanced;
    int         combo        = 0;
    int         heel_guards  = 0;
    bool        rage_active  = false;
    bool        focused      = false;
};

struct Patroclus {
    bool active       = false;
    bool alive        = true;
    bool used_support = false;
};

struct GodIntervention {
    std::string god;
    std::string message;
    int         turns_left  = 0;
    bool        triggered   = false;
};

enum class CombatOutcome {
    Ongoing,
    PlayerWon,
    PlayerDied,
    PlayerHeelHit,
    PlayerFled,
};

class CombatSession {
public:
    CombatSession(Combatant player, Combatant enemy, Patroclus pat = {});
    CombatSession(Combatant player, Combatant enemy, Patroclus pat,
                  const std::string& god_ally);

    void tick(long long dt_ms);
    bool submit(PlayerAction a);

    const Combatant& Player() const { return player_; }
    const Combatant& Enemy()  const { return enemy_; }
    const Patroclus& Companion() const { return pat_; }
    int              EnemyHP() const { return enemy_hp_; }
    CombatOutcome    Outcome() const { return outcome_; }

    const Attack&    CurrentAttack() const;
    bool             AttackInProgress() const;
    long long        CoilRemainingMs() const;
    long long        CoilTotalMs() const;

    // Pacing state — new in Step 1.5
    bool             InIdlePhase() const;          // between attacks
    long long        IdleRemainingMs() const;
    bool             InPreCoilPhase() const;       // stillness before a coil
    long long        PreCoilRemainingMs() const;
    int              ConsecutiveAttacks() const { return consecutive_attacks_; }

    bool             JustHitPlayer()  const { return just_hit_; }
    bool             JustDodged()     const { return just_dodged_; }
    bool             JustHitEnemy()   const { return just_hit_enemy_; }
    bool             JustGuardUsed()  const { return just_guard_used_; }

    std::string LastEvent() const { return last_event_; }
    const std::vector<std::string>& EventLog() const { return event_log_; }   // NEW
    const GodIntervention& Intervention() const { return intervention_; }
    std::string      EnemySpeech() const;    // current speech line ("" if none)

    void set_unlocked_actions(int layer);
    bool action_unlocked(PlayerAction a) const;

    // Exposed for UI: dodge window in ms (shrinks with spam)
    long long current_dodge_window_ms() const;

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

    // ---- Pacing state (Step 1.5) ----
    enum class Phase { Idle, PreCoil, Coil };
    Phase     phase_                = Phase::PreCoil;
    long long phase_elapsed_ms_     = 0;
    int       consecutive_attacks_  = 0;
    int       pending_coil_ms_      = 0;   // set when entering PreCoil

    static constexpr long long POST_ATTACK_IDLE_MS   = 1200;
    static constexpr long long PRE_HEEL_COIL_MS      = 500;

    bool      just_hit_        = false;
    bool      just_dodged_     = false;
    bool      just_hit_enemy_  = false;
    bool      just_guard_used_ = false;
    std::string last_event_;
        std::vector<std::string> event_log_;    // last 3 events, most recent last

    long long since_player_action_ms_ = 0;
    long long player_action_cooldown_ms_ = 150;

    PlayerAction last_player_action_ = PlayerAction::None;
    GodIntervention intervention_;

    int       unlocked_layer_ = 1;

    void begin_idle();
    void begin_pre_coil();
    void begin_coil();
    void resolve_enemy_attack();
    void advance_to_next_attack();

    void maybe_patroclus_support();
    void apply_behavior_on_player_action(PlayerAction a);
    void apply_behavior_on_enemy_hit();
    int  effective_player_damage(int base);
    double stance_damage_mult() const;
    double stance_heel_mult() const;

    // Pacing helpers
    long long adjusted_coil_ms(int base_coil_ms) const;
    void      register_player_attack();       // increments consecutive, applies penalty
    void      reset_consecutive_attacks();    // called on non-attack actions
};