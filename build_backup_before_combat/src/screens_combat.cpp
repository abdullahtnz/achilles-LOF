#include "game_app.hpp"
#include "ascii_art.hpp"
#include "screens.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <string>

void GameApp::frame_combat() {
    if (!combat_) { state_ = AppState::Menu; return; }

    const int R = ui::rows();
    const int C = ui::cols();
    const auto& p = combat_->Player();
    const auto& e = combat_->Enemy();
    const int ehp = combat_->EnemyHP();

    int layer = ctx_.CurrentLayer();
    ui::Color accent = screens::layer_accent(layer);

    // ---------------- Top bar ----------------
    ui::draw(0, 0, std::string(C, ' '), {ui::DEFAULT});
    ui::draw(0, 2, "LAYER " + std::to_string(layer) + " \u00B7 " +
                    screens::layer_name(layer),
             {accent, true});
    ui::draw(0, C - 22, "kleos " + std::to_string(ctx_.Meta().kleos),
             {ui::ACCENT});

    // ---------------- Enemy block ----------------
    bool is_chiron = e.name.find("Chiron") != std::string::npos;
    auto enemy_art = is_chiron ? art::chiron() : art::phantom();
    int art_top = 3;

    bool coiling = combat_->AttackInProgress();
    long long remain = combat_->CoilRemainingMs();
    long long total  = combat_->CoilTotalMs();
    double prog = total > 0 ? 1.0 - (double)remain / (double)total : 0.0;

    ui::Color art_color = ui::DEFAULT;
    bool art_bold = false;
    if (coiling) {
        const auto& a = combat_->CurrentAttack();
        if (a.telegraphed && a.is_heel) {
            art_color = ui::DANGER;
            art_bold  = true;
        } else if (a.telegraphed) {
            art_color = ui::WARNING;
        }
    }
    art::draw_centered(enemy_art, art_top, art_color, art_bold);

    // Enemy name + HP bar
    int name_row = art_top + (int)enemy_art.size() + 1;
    ui::draw_center(name_row, e.name, {ui::DANGER, true});
    int bar_w = 30;
    int bx    = (C - bar_w) / 2 - 6;
    ui::draw(name_row + 1, bx, "HP", {ui::DIM});
    ui::draw_bar(name_row + 1, bx + 4, bar_w,
                 (double)ehp / e.hp_max,
                 {ui::DANGER, true}, {ui::DIM});
    ui::draw(name_row + 1, bx + 4 + bar_w + 2,
             std::to_string(ehp) + "/" + std::to_string(e.hp_max),
             {ui::DIM});

    // ---------------- Behavior hints ----------------
    int behavior_row = name_row + 2;
    if (e.behavior == "shield_wall") {
        int hits = e.behavior_state;
        std::string s = "SHIELD UP \u2014 " + std::to_string(3 - hits) +
                        " more hits to break";
        ui::draw_center(behavior_row, s, {ui::WARNING, true});
    } else if (e.behavior == "grief") {
        int charge = e.behavior_state;
                ui::Attr grief_attr = charge > 0
                              ? ui::Attr{ui::DANGER, true}
                              : ui::Attr{ui::DIM};
        ui::draw_center(behavior_row,
                        "GRIEF " + std::to_string(charge) + "/3",
                        grief_attr);
    } else if (e.behavior == "mirror") {
        ui::draw_center(behavior_row, "an illusion of you",
                        {ui::INFO});
    }

    // ---------------- Attack tell + coil meter ----------------
    int tell_row = name_row + 4;
    if (coiling) {
        const auto& a = combat_->CurrentAttack();
        if (a.telegraphed && a.is_heel) {
            ui::draw_center(tell_row, "\u25B6 HEEL STRIKE COILING \u25C0",
                            {ui::DANGER, true});
        } else if (a.telegraphed) {
            ui::draw_center(tell_row, "warning \u00B7 " + a.name,
                            {ui::WARNING});
        } else {
            ui::draw_center(tell_row, "movement \u00B7 unreadable",
                            {ui::DIM});
        }
        int meter_w = 40;
        ui::draw_bar(tell_row + 1, (C - meter_w) / 2, meter_w, prog,
                     {ui::DANGER, true}, {ui::DIM});
    } else {
        ui::draw_center(tell_row, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    }

    // ---------------- Last event line ----------------
    ui::draw_center(tell_row + 3, combat_->LastEvent(), {ui::INFO});

    // ---------------- Status bar ----------------
    int status_y = R - 5;
    ui::draw(status_y, 0, std::string(C, ' '), {ui::DEFAULT});

    int hb_w = 24;
    ui::draw(status_y, 2, "HP", {ui::DIM});
    ui::draw_bar(status_y, 5, hb_w,
                 (double)p.hp / p.hp_max,
                 {ui::SAFE, true}, {ui::DIM});
    ui::draw(status_y, 5 + hb_w + 2,
             std::to_string(p.hp) + "/" + std::to_string(p.hp_max),
             {ui::DIM});

    int sx = 5 + hb_w + 12;
    ui::draw(status_y, sx, "STA", {ui::DIM});
    for (int i = 0; i < p.stamina_max; ++i) {
        ui::draw(status_y, sx + 5 + i * 2,
                 i < p.stamina ? "\u25CF" : "\u25CB",
                 {ui::INFO, true});
    }

    // Patroclus line — only when present in this layer.
    if (combat_->Companion().active) {
        std::string patline = combat_->Companion().alive
                              ? "Patroclus at your shoulder."
                              : "Patroclus has fallen.";
                ui::Attr pat_attr = combat_->Companion().alive
                            ? ui::Attr{ui::SAFE}
                            : ui::Attr{ui::DANGER};
        ui::draw(status_y + 1, 2, patline, pat_attr);
    }

    // ---------------- Hotkey bar ----------------
    int hot_y = R - 2;
    ui::draw(hot_y, 0, std::string(C, ' '), {ui::DEFAULT});
    ui::draw(hot_y, 2, "[A]ttack    [D]odge    [H]eal    [F]lee",
             {ui::INFO, true});

    // ---------------- Resolve outcome ----------------
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
                    state_ = AppState::BoonChoice;
                } else if (new_layer == 4 && ctx_.Run().patroclus_alive) {
                    // Patroclus's first scripted appearance happens on the
                    // Layer 3 clear — he survives into Layer 4 by default.
                    state_ = ctx_.HasRun() ? AppState::LayerIntro : AppState::Menu;
                } else {
                    state_ = ctx_.HasRun() ? AppState::LayerIntro : AppState::Menu;
                }
            } else {
                ctx_.MutableRun().encounter += 1;
                ctx_.PersistRun();
                start_encounter_for_layer();
            }
            return;
        }

        // Death path (heel or HP)
        combat_.reset();
        ctx_.OnHeelHit();
        state_ = AppState::Death;
        return;
    }

    // ---------------- Player input ----------------
    int k = ui::get_key_ms(16);

    PlayerAction act = PlayerAction::None;
    if (k == 'a' || k == 'A') act = PlayerAction::Attack;
    if (k == 'd' || k == 'D') act = PlayerAction::Dodge;
    if (k == 'h' || k == 'H') act = PlayerAction::Heal;
    if (k == 'f' || k == 'F') act = PlayerAction::Flee;
    if (act != PlayerAction::None) combat_->submit(act);
}