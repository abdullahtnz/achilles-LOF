#pragma once
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

struct Attack {
    std::string name;
    int         hp_damage   = 0;
    bool        is_heel     = false;
    bool        telegraphed = true;
    int         coil_ms     = 500;
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

    // NEW — behavioral dispatch for layers 2-3
    std::string behavior;      // "" | "mirror" | "grief" | "ranged" | "chariot" | "shield_wall"
    int         behavior_state = 0;   // per-enemy scratch (shield hits, grief charge, etc.)
};

enum class PlayerAction { None, Attack, Dodge, Heal, Flee };

enum class CombatOutcome {
    Ongoing,
    PlayerWon,
    PlayerDied,
    PlayerHeelHit,
    PlayerFled,
};

// Patroclus companion — available from Layer 3 onward.
struct Patroclus {
    bool active       = false;   // in this fight
    bool alive        = true;
    bool used_support = false;   // one support action per encounter
};

class CombatSession {
public:
    CombatSession(Combatant player, Combatant enemy, Patroclus pat = {});

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

    bool             JustHitPlayer() const { return just_hit_; }
    bool             JustDodged()    const { return just_dodged_; }
    bool             JustHitEnemy()  const { return just_hit_enemy_; }

    std::string      LastEvent() const { return last_event_; }

private:
    Combatant player_;
    Combatant enemy_;
    Patroclus pat_;
    int       enemy_hp_ = 0;
    int       enemy_idx_ = 0;
    CombatOutcome outcome_ = CombatOutcome::Ongoing;

    bool      attack_active_ = false;
    long long coil_elapsed_ms_ = 0;
    int       current_attack_index_ = 0;

    bool      dodge_pending_ = false;
    long long dodge_window_ms_ = 400;

    bool      just_hit_ = false;
    bool      just_dodged_ = false;
    bool      just_hit_enemy_ = false;
    std::string last_event_;

    long long since_player_action_ms_ = 0;
    long long player_action_cooldown_ms_ = 150;

    PlayerAction last_player_action_ = PlayerAction::None;

    void resolve_enemy_attack();
    void next_enemy_attack();
    void maybe_patroclus_support();
    void apply_behavior_on_player_action(PlayerAction a);
    void apply_behavior_on_enemy_hit();
};