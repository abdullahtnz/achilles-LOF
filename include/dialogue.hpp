#pragma once
#include <string>
#include <vector>

// A single line of prose. Empty `speaker` means narration.
struct Line {
    std::string speaker;   // "" for narration, or a name like "Thetis"
    std::string text;
};

// One choice offered at the end of a scene. Choosing a line index
// advances to a different scene (or the same scene, later line).
struct Choice {
    std::string label;
    std::string next_scene_id;   // "" = continue current scene
};

// A scene — a sequence of lines, optionally ending in a choice.
struct Scene {
    std::string       id;         // e.g. "ch1_intro_thetis"
    std::string       title;      // shown at top, e.g. "PHTHIA — THE BIRTH"
    std::vector<Line> lines;
    std::vector<Choice> choices;  // empty = no choice, auto-advance to next_scene_id
    std::string       next_scene_id; // used when there are no choices
};

// Serializable dialogue progress — persisted in RunState.
struct DialogueState {
    std::string      scene_id;           // current scene, e.g. "ch1_intro_thetis"
    int              line_index = 0;     // which line the player is on
    std::vector<int> choice_path;        // indices of past choices, in order
    bool             pending_choice = false; // has the scene reached its choice point?
};

// Load the scene with the given ID. nullptr if not found (validation fallback).
const Scene* find_scene(const std::string& id);

// Advance to the next line. Returns true if the scene advanced.
// Returns false if the scene is complete (choices pending or auto-advance).
bool advance_line(DialogueState& state, const Scene& scene);

// Apply a choice at the current scene. Returns the next scene id (or "" if
// the scene does not end with a choice).
std::string apply_choice(DialogueState& state, const Scene& scene, int choice_index);

// Is the scene finished (all lines shown)?
bool scene_finished(const DialogueState& state, const Scene& scene);