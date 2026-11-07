#include "game_app.hpp"
#include "ascii_art.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

static ui::Attr attr_color(ui::Color c, bool bold = false) {
    ui::Attr a; a.color = c; a.bold = bold; return a;
}

void GameApp::frame_combat() {
    if (!combat_) { transition_to(AppState::Menu); return; }

    const int R = ui::rows();
    const int C = ui::cols();
    const auto& p = combat_->Player();
    const auto& e = combat_->Enemy();
    const int ehp = combat_->EnemyHP();

    int layer = ctx_.CurrentLayer();
    ui::Color accent = screens::layer_accent(layer);

    bool coiling   = combat_->AttackInProgress();
    long long remain = combat_->CoilRemainingMs();
    long long total  = combat_->CoilTotalMs();
    double prog = total > 0 ? 1.0 - (double)remain / (double)total : 0.0;

    // ============================================================
    // TOP BAR
    // ============================================================
    ui::draw(0, 0, std::string(C, ' '), {ui::DEFAULT});
    ui::draw(0, 2,
             "LAYER " + std::to_string(layer) + " \u00B7 " +
             screens::layer_name(layer),
             attr_color(accent, true));
    ui::draw(0, C - 28,
             "kleos " + std::to_string(ctx_.Meta().kleos) +
             "    deaths " + std::to_string(ctx_.Meta().total_deaths),
             attr_color(ui::ACCENT));
    // \u2500 = ─ = 3 UTF-8 bytes. Use a UTF-8 string, not a char literal.
std::string rule;
for (int i = 0; i < C; ++i) rule += "\u2500";
ui::draw(1, 0, rule, attr_color(ui::BORDER));

    // ============================================================
    // ENEMY ZONE
    // ============================================================
    int enemy_top    = 3;
    int enemy_bottom = R - 12;
    int enemy_center = (enemy_top + enemy_bottom) / 2;

    // Border glow when heel is coiling
    ui::Attr border_attr = attr_color(ui::BORDER);
    if (coiling && combat_->CurrentAttack().telegraphed &&
        combat_->CurrentAttack().is_heel) {
        border_attr = attr_color(ui::DANGER, true);
    }

    int box_y = enemy_top;
    int box_h = enemy_bottom - enemy_top;
    int box_x = 2;
    int box_w = C - 4;
    ui::draw_box(box_y, box_x, box_h, box_w, "", border_attr);

    // Enemy art
    bool is_chiron = e.name.find("Chiron") != std::string::npos;
    auto enemy_art = is_chiron ? art::chiron() : art::phantom();
    int art_y = enemy_center - (int)enemy_art.size() / 2 - 4;

    ui::Color art_color = ui::DEFAULT;
    bool art_bold = false;
    if (coiling) {
        const auto& a = combat_->CurrentAttack();
        if (a.telegraphed && a.is_heel) { art_color = ui::DANGER;  art_bold = true; }
        else if (a.telegraphed)         { art_color = ui::WARNING; }
    }
    art::draw_centered(enemy_art, art_y, art_color, art_bold);

    // Enemy name + verb
    int name_y = art_y + (int)enemy_art.size() + 1;
    ui::draw_center(name_y, e.name, attr_color(ui::DANGER, true));

    if (coiling) {
        const auto& a = combat_->CurrentAttack();
        std::string verb = a.verb.empty() ? "moving" : a.verb;
        if (a.telegraphed && a.is_heel) {
            ui::draw_center(name_y + 1,
                "\u26A0  HEEL STRIKE COILING  \u26A0",
                attr_color(ui::DANGER, true));
        } else if (a.telegraphed) {
            ui::draw_center(name_y + 1, "\u00B7 " + verb + " \u00B7",
                            attr_color(ui::WARNING));
        } else {
            ui::draw_center(name_y + 1, "\u00B7 movement \u00B7 unreadable",
                            attr_color(ui::DIM));
        }
    } else {
        ui::draw_center(name_y + 1, "\u00B7 \u00B7 \u00B7",
                        attr_color(ui::DIM));
    }

    // Big HP bar
    int hp_w = std::min(44, box_w - 12);
    int hp_x = (C - hp_w) / 2;
    ui::draw_big_bar(name_y + 2, hp_x, hp_w,
                     (double)ehp / e.hp_max,
                     attr_color(ui::DANGER, true),
                     attr_color(ui::DIM));
    ui::draw_center(name_y + 2,
                    std::to_string(ehp) + " / " + std::to_string(e.hp_max),
                    attr_color(ui::DANGER, true));

    // Coil meter
    if (coiling) {
        int meter_w = std::min(50, box_w - 6);
        int meter_x = (C - meter_w) / 2;
        ui::draw_bar(name_y + 4, meter_x, meter_w, prog,
                     attr_color(ui::DANGER, true), attr_color(ui::DIM));
    }

    // Behavior hint
    int bhy = name_y + 6;
    if (e.behavior == "shield_wall") {
        ui::draw_center(bhy,
            "SHIELD UP \u2014 " + std::to_string(3 - e.behavior_state) +
            " more hits to break", attr_color(ui::WARNING, true));
    } else if (e.behavior == "grief") {
        ui::Attr g = e.behavior_state > 0 ? attr_color(ui::DANGER, true)
                                          : attr_color(ui::DIM);
        ui::draw_center(bhy,
            "GRIEF " + std::to_string(e.behavior_state) + "/3", g);
    } else if (e.behavior == "mirror") {
        ui::draw_center(bhy, "an illusion of you", attr_color(ui::INFO));
    }

    // God intervention banner
    if (combat_->Intervention().triggered &&
        combat_->Intervention().turns_left > 0) {
        ui::draw_center(R - 11, combat_->Intervention().message,
                        attr_color(ui::HIGHLIGHT, true));
    }

    // Event log (last 3 events from combat)
    ui::draw_center(R - 9, combat_->LastEvent(), attr_color(ui::INFO));
    if (combat_->Companion().active) {
        std::string patline = combat_->Companion().alive
                              ? "Patroclus at your shoulder."
                              : "Patroclus has fallen.";
        ui::draw_center(R - 8, patline,
                        combat_->Companion().alive
                            ? attr_color(ui::SAFE)
                            : attr_color(ui::DANGER));
    }

    // ============================================================
    // PLAYER HUD
    // ============================================================
    int status_y = R - 6;
    ui::draw(status_y, 0, std::string(C, ' '), {ui::DEFAULT});

    // HP
    int hb_w = 22;
    ui::draw(status_y, 2, "HP", attr_color(ui::DIM));
    ui::draw_big_bar(status_y, 5, hb_w,
                     (double)p.hp / p.hp_max,
                     attr_color(ui::SAFE, true), attr_color(ui::DIM));
    ui::draw(status_y, 5 + hb_w + 2,
             std::to_string(p.hp) + "/" + std::to_string(p.hp_max),
             attr_color(ui::DIM));

    // Stamina
    int sx = 5 + hb_w + 12;
    ui::draw(status_y, sx, "STA", attr_color(ui::DIM));
    for (int i = 0; i < p.stamina_max; ++i) {
        ui::draw(status_y, sx + 5 + i * 2,
                 i < p.stamina ? "\u25CF" : "\u25CB",
                 attr_color(ui::INFO, true));
    }

    // Stance
    int stance_x = sx + 5 + p.stamina_max * 2 + 3;
    ui::Attr stance_attr =
        p.stance == Stance::Aggressive ? attr_color(ui::DANGER, true)
      : p.stance == Stance::Defensive  ? attr_color(ui::SAFE, true)
                                       : attr_color(ui::INFO, true);
    ui::draw(status_y, stance_x,
             std::string("STANCE: ") + stance_name(p.stance),
             stance_attr);

    // Combo
    int combo_x = C - 24;
    ui::draw(status_y, combo_x, "COMBO", attr_color(ui::DIM));
    for (int i = 0; i < 3; ++i) {
        ui::draw(status_y, combo_x + 7 + i * 2,
                 i < p.combo ? "\u25CF" : "\u25CB",
                 i < p.combo ? attr_color(ui::ACCENT, true)
                             : attr_color(ui::DIM));
    }

    // Heel Guards
    if (p.heel_guards > 0) {
        ui::draw(status_y + 1, 2,
                 "HEEL GUARD: " + std::to_string(p.heel_guards) + " token" +
                 (p.heel_guards == 1 ? "" : "s"),
                 attr_color(ui::ACCENT, true));
    }

    // Rage bar (only if rage gained)
    if (p.rage > 0) {
        int rage_w = 20;
        ui::draw(status_y + 1, C - rage_w - 8, "RAGE", attr_color(ui::DIM));
        ui::draw_bar(status_y + 1, C - rage_w - 3, rage_w,
                     (double)p.rage / p.rage_max,
                     attr_color(ui::DANGER, true), attr_color(ui::DIM));
    }

    // ============================================================
    // HOTKEY BAR
    // ============================================================
    int hot_y = R - 2;
    ui::draw(hot_y, 0, std::string(C, ' '), {ui::DEFAULT});
    int hx = 2;
    auto hotkey = [&](const std::string& label, bool unlocked,
                      ui::Color c) {
        ui::Attr a = unlocked ? attr_color(c, true) : attr_color(ui::DIM);
        ui::draw(hot_y, hx, label, a);
        hx += (int)label.size() + 3;
    };
    hotkey("[A]ttack",   true, ui::INFO);
    hotkey("[R]eckless", combat_->action_unlocked(PlayerAction::Reckless), ui::DANGER);
    hotkey("[F]ocus",    combat_->action_unlocked(PlayerAction::Focus),   ui::SAFE);
    hotkey("[D]odge",    true, ui::INFO);
    hotkey("[H]eal",     true, ui::WARNING);
    hotkey("[S]tance",   combat_->action_unlocked(PlayerAction::Stance),  ui::HIGHLIGHT);
    hotkey("[X] Flee",   true, ui::DIM);

    // ============================================================
    // RESOLVE OUTCOME
    // ============================================================
    if (combat_->Outcome() != CombatOutcome::Ongoing) {
        auto outcome = combat_->Outcome();

        uint32_t earned = 0;
        switch (outcome) {
            case CombatOutcome::PlayerWon:
                earned = 15 + ctx_.Run().encounter * 5;
                break;
            case CombatOutcome::PlayerHeelHit:
            case CombatOutcome::PlayerDied:
                earned = 15;
                break;
            default: break;
        }
        auto& run = ctx_.MutableRun();
        run.run_kleos += earned;
        ctx_.PersistRun();

        if (outcome == CombatOutcome::PlayerWon) {
            bool was_boss =
                e.name.find("Chiron")   != std::string::npos ||
                e.name.find("Deidamia") != std::string::npos ||
                e.name.find("Vanguard") != std::string::npos;
            combat_.reset();
            if (was_boss) {
                ctx_.AdvanceLayer();
                int new_layer = ctx_.CurrentLayer();
                if (new_layer == 3 && !ctx_.Run().boon_chosen) {
                    boon_pending_ = true;
                    transition_to(AppState::BoonChoice);
                } else {
                    transition_to(ctx_.HasRun() ? AppState::LayerIntro
                                                : AppState::Menu);
                }
            } else {
                ctx_.MutableRun().encounter += 1;
                ctx_.PersistRun();
                start_encounter_for_layer();
            }
            return;
        }

        combat_.reset();
        ctx_.OnHeelHit();
        transition_to(AppState::Death);
        return;
    }

    // ============================================================
    // INPUT
    // ============================================================
    int k = ui::get_key_ms(16);

    PlayerAction act = PlayerAction::None;
    if (k == 'a' || k == 'A') act = PlayerAction::Attack;
    if (k == 'r' || k == 'R') act = PlayerAction::Reckless;
    if (k == 'f' || k == 'F') act = PlayerAction::Focus;
    if (k == 'd' || k == 'D') act = PlayerAction::Dodge;
    if (k == 'h' || k == 'H') act = PlayerAction::Heal;
    if (k == 's' || k == 'S') act = PlayerAction::Stance;
    if (k == 'x' || k == 'X') act = PlayerAction::Flee;

    // Confirm flee when at good HP
    if (act == PlayerAction::Flee && p.hp > p.hp_max / 2) {
        ui::draw_center(R - 4, "Flee at full strength? [F] confirm / [ESC] cancel",
                        attr_color(ui::WARNING, true));
        ui::refresh();
        int k2 = ui::get_key_ms(2000);
        if (k2 != 'f' && k2 != 'F') act = PlayerAction::None;
    }

    if (act != PlayerAction::None) combat_->submit(act);

    if (k == '?') {
        return_to_ = AppState::Combat;
        transition_to(AppState::Help);
    }
    if (k == 'p' || k == 'P') paused_ = !paused_;
}