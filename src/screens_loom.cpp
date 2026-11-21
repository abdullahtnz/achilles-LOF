#include "game_app.hpp"
#include "ascii_art.hpp"
#include "ui.hpp"
#include <ncurses.h>
#include <algorithm>
#include <string>
#include <vector>

namespace {

struct WeaponOffer {
    std::string name;
    uint32_t    cost;
};

WeaponOffer next_weapon_offer(const MetaState& m) {
    static const std::vector<std::string> order = {
        "spear", "sword", "shield", "chariot", "bow"
    };
    for (const auto& w : order) {
        bool owned = false;
        for (const auto& u : m.unlocked_weapons) {
            if (u == w) { owned = true; break; }
        }
        if (!owned) {
            uint32_t cost = 40u * (uint32_t)m.unlocked_weapons.size();
            return { w, cost };
        }
    }
    return { "", 0 };
}

const char* fragment_name(const std::string& id) {
    if (id == "frag_chiron")    return "Chiron's Lesson";
    if (id == "frag_deidamia")  return "Deidamia's Grief";
    if (id == "frag_patroclus") return "Patroclus's Truth";
    if (id == "frag_scamander") return "The River's Dead";
    if (id == "frag_hector")    return "Hector's Body";
    if (id == "frag_moirai")    return "The Weaver's Truth";
    return id.c_str();
}

} // namespace

void GameApp::frame_loom() {
    const int R = ui::rows();
    const int C = ui::cols();

    static auto opened_at = ui::now();
    static bool first = true;
    if (first) { opened_at = ui::now(); first = false; }
    long long elapsed = ui::ms_since(opened_at);

    auto loom_art = art::loom();
    int art_y = 1;
    if (elapsed < 500) {
        ui::draw_center(art_y + 4, "\u00B7 \u00B7 \u00B7", {ui::DIM});
    } else {
        art::draw_centered(loom_art, art_y, ui::HIGHLIGHT, true);
    }

    const auto& meta = ctx_.Meta();

    int title_y = art_y + (int)loom_art.size() + 1;
    ui::draw_center(title_y, "THE LOOM", {ui::HIGHLIGHT, true});

    std::string fates;
    if (meta.total_deaths == 0) {
        fates = "A new thread. We have not yet cut it.";
    } else if (meta.total_deaths < 3) {
        fates = "You have been cut. The weave remembers.";
    } else if (meta.total_deaths < 6) {
        fates = "The loom is heavy with your names.";
    } else {
        fates = "We know every way you fall now. Ask us for a new way.";
    }
    ui::draw_center(title_y + 1, fates, {ui::DIM});

    int info_y = title_y + 3;
    ui::draw_center(info_y,
        "kleos " + std::to_string(meta.kleos) +
        "    deaths " + std::to_string(meta.total_deaths) +
        "    runs " + std::to_string(meta.total_runs),
        {ui::ACCENT, true});

    // Memory fragments
    int frag_y = info_y + 2;
    ui::draw_center(frag_y,
        "MEMORY FRAGMENTS  " +
        std::to_string((int)meta.fragments_collected.size()) + " / " +
        std::to_string(FRAGMENT_TOTAL),
        {ui::HIGHLIGHT, true});

    static const char* frag_ids[] = {
        "frag_chiron", "frag_deidamia", "frag_patroclus",
        "frag_scamander", "frag_hector", "frag_moirai"
    };
    for (int i = 0; i < FRAGMENT_TOTAL; ++i) {
        bool got = meta.fragments_collected.count(frag_ids[i]) > 0;
        std::string line = std::string(got ? "\u2726 " : "\u00B7 ") +
                           (got ? fragment_name(frag_ids[i]) : "unwoven");
        int col = (i % 2 == 0) ? C / 2 - 30 : C / 2 + 2;
        ui::draw(frag_y + 1 + i / 2, col, line,
                 got ? ui::Attr{ui::ACCENT, true} : ui::Attr{ui::DIM});
    }

    // Shop
    int shop_y = frag_y + 5;
    WeaponOffer offer = next_weapon_offer(meta);
    std::string weapon_line;
    if (!offer.name.empty()) {
        weapon_line = "[W]  Offer " + std::to_string(offer.cost) +
                      " kleos \u2014 unlock the " + offer.name;
    } else {
        weapon_line = "Every weapon is yours. Even the bow.";
    }
    ui::draw_center(shop_y, weapon_line, {ui::INFO, true});

    int tokens = 0;
    auto tok = meta.fates_flags.find("thetis_tokens");
    if (tok != meta.fates_flags.end()) tokens = tok->second;
    ui::draw_center(shop_y + 1,
        "[T]  Offer 30 kleos \u2014 Thetis grants a heel guard next run (" +
        std::to_string(tokens) + " held)",
        {ui::SAFE, true});

    if (meta.fates_flags.count("hard_mode_unlocked") > 0) {
        ui::draw_center(shop_y + 2,
            "THE NAMELESS THREAD is unlocked in the menu.",
            {ui::DANGER, true});
    }
    if (!meta.endings_seen.empty()) {
        std::string seen = "endings witnessed: ";
        for (size_t i = 0; i < meta.endings_seen.size(); ++i) {
            if (i) seen += "  ";
            seen += meta.endings_seen[i];
        }
        ui::draw_center(shop_y + 3, seen, {ui::DIM});
    }

    int bottom_y = std::min(R - 3, shop_y + 5);
    ui::draw_center(bottom_y,
        "[M] manual    [ENTER] step back from the loom",
        {ui::DIM, true});

    int k = ui::get_key_ms(200);
    if (k == ERR) return;

    if (k == '\n' || k == '\r' || k == KEY_ENTER) {
        first = true;
        transition_to(AppState::Menu);
        return;
    }
    if (k == 'm' || k == 'M') {
        return_to_ = AppState::Loom;
        transition_to(AppState::Manual);
        return;
    }
    if ((k == 'w' || k == 'W') && !offer.name.empty()) {
        if (ctx_.SpendKleos(offer.cost)) {
            ctx_.UnlockWeapon(offer.name);
        } else {
            ui::beep_error();
        }
        return;
    }
    if (k == 't' || k == 'T') {
        if (tokens >= 3) {
            ui::beep_error();
        } else if (ctx_.SpendKleos(30)) {
            ctx_.AddThetisToken(1);
        } else {
            ui::beep_error();
        }
        return;
    }
}
