#include "story.hpp"
#include <vector>

namespace story {
    std::vector<Scene> chapter_1_scenes();
    std::vector<Scene> chapter_2_scenes();
    std::vector<Scene> chapter_3_scenes();
    std::vector<Scene> chapter_4_scenes();
    std::vector<Scene> chapter_5_scenes();
    std::vector<Scene> chapter_6_scenes();
    std::vector<Scene> chapter_7_scenes();
}

namespace {

struct Registry {
    std::vector<Scene> all;
    Registry() {
        auto add = [&](std::vector<Scene> v) {
            for (auto& s : v) all.push_back(std::move(s));
        };
        add(story::chapter_1_scenes());
        add(story::chapter_2_scenes());
        add(story::chapter_3_scenes());
        add(story::chapter_4_scenes());
        add(story::chapter_5_scenes());
        add(story::chapter_6_scenes());
        add(story::chapter_7_scenes());
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