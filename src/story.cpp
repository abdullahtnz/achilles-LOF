#include "story.hpp"
#include <vector>

// Forward-declare chapter providers.
namespace story {
    std::vector<Scene> chapter_1_scenes();
    // std::vector<Scene> chapter_2_scenes();   // Step 4
    // std::vector<Scene> chapter_3_scenes();   // Step 4
}

namespace {

struct Registry {
    std::vector<Scene> all;
    Registry() {
        auto add = [&](std::vector<Scene> v) {
            for (auto& s : v) all.push_back(std::move(s));
        };
        add(story::chapter_1_scenes());
    }
};

Registry& reg() {
    static Registry r;
    return r;
}

} // namespace

namespace story {

const Scene* lookup(const std::string& id) {
    for (const auto& s : reg().all) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

int scene_count() {
    return (int)reg().all.size();
}

} // namespace story