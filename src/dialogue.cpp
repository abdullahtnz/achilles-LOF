#include "dialogue.hpp"
#include "story.hpp"
#include <algorithm>

const Scene* find_scene(const std::string& id) {
    return story::lookup(id);
}

bool advance_line(DialogueState& state, const Scene& scene) {
    if (state.line_index + 1 >= (int)scene.lines.size()) {
        state.pending_choice = !scene.choices.empty();
        return false;
    }
    state.line_index++;
    return true;
}

std::string apply_choice(DialogueState& state, const Scene& scene, int choice_index) {
    if (choice_index < 0 || choice_index >= (int)scene.choices.size()) return "";
    state.choice_path.push_back(choice_index);
    state.pending_choice = false;
    const std::string& next = scene.choices[choice_index].next_scene_id;
    if (next.empty()) return "";     // stay in current scene (edge case)
    state.scene_id = next;
    state.line_index = 0;
    return next;
}

bool scene_finished(const DialogueState& state, const Scene& scene) {
    return state.line_index >= (int)scene.lines.size() - 1 &&
           !state.pending_choice;
}