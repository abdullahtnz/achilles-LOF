#pragma once
#include <string>
#include <vector>

namespace tutorial {

// One "page" of the tutorial. Player reads it, presses ENTER to advance.
struct Page {
    std::string title;
    std::vector<std::string> paragraphs;
    bool show_keystroke_prompt;  // e.g. "press D to dodge"
    char expected_key;           // '\0' for ENTER, 'D'/'A' for action demos
};

// The tutorial script — returns one Page per screen.
std::vector<Page> script();

// Hint keys stored in meta.fates_flags.
namespace hint {
    constexpr const char* HEEL_FIRST       = "hint_heel_first";
    constexpr const char* SPAM_FIRST       = "hint_spam_first";
    constexpr const char* COMBO_FIRST      = "hint_combo_first";
    constexpr const char* STAMINA_ZERO     = "hint_stamina_zero";
    constexpr const char* PATROCLUS_FIRST  = "hint_patroclus_first";
    constexpr const char* RAGE_FIRST       = "hint_rage_first";
    constexpr const char* STANCE_FIRST     = "hint_stance_first";
    constexpr const char* DEATH_FIRST      = "hint_death_first";
    constexpr const char* BOSS_FIRST       = "hint_boss_first";
}

} // namespace tutorial