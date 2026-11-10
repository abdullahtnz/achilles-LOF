#pragma once
#include "dialogue.hpp"

namespace story {

// Register all scene providers here. Each chapter file defines
// `std::vector<Scene> scenes()`. The registry assembles them.
const Scene* lookup(const std::string& id);

// Boot-time validation — returns number of scenes registered.
int scene_count();

} // namespace story