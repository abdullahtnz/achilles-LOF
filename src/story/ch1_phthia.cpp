#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_1_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — The Birth
    // Thetis at the water's edge with the newborn.
    // ============================================================
    Scene birth;
    birth.id = "ch1_birth";
    birth.title = "PHTHIA \u2014 THE BIRTH";
    birth.lines = {
        {"", "The sea is loud tonight."},
        {"", "A woman stands at the edge of the water, holding a newborn by one small foot. She has already wept. The salt on her face might be the sea, or might be grief."},
        {"Thetis", "The Fates have spoken."},
        {"Thetis", "He will be great, and short-lived. He will die beneath Troy \u2014 unless he forsakes glory and lives long in obscurity."},
        {"", "She lowers the child toward the river. The black water takes him."},
        {"Thetis", "All of you will be immortal. Every part of you."},
        {"", "She holds him by the heel. The water rises around his body and does not touch her hand."},
        {"Thetis", "Every part."},
        {"", "When she lifts him out, the boy is silent. Not crying. Watching her."},
        {"Thetis", "You will not remember this. But the river will."},
        {"", "She does not look at the heel."},
        {"", "She cannot."},
    };
    birth.next_scene_id = "ch1_training_intro";
    scenes.push_back(std::move(birth));

    // ============================================================
    // SCENE 2 — Chiron's cave (training intro)
    // Years later. Achilles a youth. Chiron appears.
    // ============================================================
    Scene training;
    training.id = "ch1_training_intro";
    training.title = "PHTHIA \u2014 THE CAVE OF CHIRON";
    training.lines = {
        {"", "Years pass. The boy becomes a youth. He is faster than the other boys and knows it."},
        {"", "One morning he wakes to find a centaur watching him from the mouth of a cave."},
        {"Chiron", "You are Thetis's son."},
        {"Achilles", "You are a horse."},
        {"Chiron", "I am a teacher."},
        {"", "Chiron steps into the light. He does not smile."},
        {"Chiron", "Your mother made you difficult to kill. She could not make you difficult to break."},
        {"Achilles", "Nothing can break me."},
        {"Chiron", "Then we begin."},
        {"", "He draws his bow, nocks an arrow, and looses it before Achilles can flinch."},
        {"", "The arrow is not aimed at his chest. It is aimed at his heel."},
        {"", "Achilles dodges."},
        {"Chiron", "Again."},
    };
    training.next_scene_id = "ch1_combat_1";
    scenes.push_back(std::move(training));

    // ============================================================
    // SCENE 3 — Combat encounter 1 (before)
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch1_combat_1";
    combat_1.title = "PHTHIA \u2014 THE FIRST LESSON";
    combat_1.lines = {
        {"Chiron", "A shadow will test you now. It is not a warrior \u2014 it is a shape."},
        {"Chiron", "It will not try to kill you. It will try to teach you."},
        {"Chiron", "Watch its body, not its weapon. When it coils, it tells you where it will strike."},
        {"Achilles", "And if I miss the tell?"},
        {"Chiron", "Then you learn."},
    };
    combat_1.next_scene_id = "";   // exits to combat via dialogue_exit()
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 4 — Between fights (mid-scene 1)
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch1_mid_1";
    mid_1.title = "PHTHIA \u2014 BETWEEN SHADOWS";
    mid_1.lines = {
        {"Chiron", "Better."},
        {"Achilles", "It was slow."},
        {"Chiron", "It was meant to be slow. The next one will not be."},
        {"Achilles", "Good."},
        {"Chiron", "You say that now."},
    };
    mid_1.next_scene_id = "ch1_combat_2";
    scenes.push_back(std::move(mid_1));

    // ============================================================
    // SCENE 5 — Combat encounter 2 (before)
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch1_combat_2";
    combat_2.title = "PHTHIA \u2014 THE SECOND SHADOW";
    combat_2.lines = {
        {"Chiron", "This one has learned from your mistakes."},
        {"Achilles", "I have not made any."},
        {"Chiron", "You have. You do not yet know what they are."},
        {"", "The shade steps forward. Its heel does not touch the ground."},
        {"Chiron", "Watch its feet. Not its blade."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 6 — Between fights (mid-scene 2)
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch1_mid_2";
    mid_2.title = "PHTHIA \u2014 BETWEEN SHADOWS";
    mid_2.lines = {
        {"Achilles", "It tried to strike my heel."},
        {"Chiron", "Yes."},
        {"Achilles", "Why?"},
        {"Chiron", "Because it wants to live."},
        {"Achilles", "Then it is a coward."},
        {"Chiron", "It is a survivor. There is a difference. You will learn it."},
    };
    mid_2.next_scene_id = "ch1_combat_3";
    scenes.push_back(std::move(mid_2));

    // ============================================================
    // SCENE 7 — Combat encounter 3 (before)
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch1_combat_3";
    combat_3.title = "PHTHIA \u2014 THE OLD TUTOR'S SHADE";
    combat_3.lines = {
        {"Chiron", "This one was your tutor, before me."},
        {"Achilles", "I do not remember a tutor."},
        {"Chiron", "You will."},
        {"", "The shade does not bow. It waits."},
        {"Chiron", "It will not telegraph. Not all enemies will."},
        {"Achilles", "Then how do I beat it?"},
        {"Chiron", "You stay ready when nothing is coming. That is the whole of it."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 8 — Between fights (pre-boss)
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch1_pre_boss";
    pre_boss.title = "PHTHIA \u2014 THE LAST LESSON";
    pre_boss.lines = {
        {"Chiron", "You have done well."},
        {"Achilles", "I know."},
        {"Chiron", "You do not."},
        {"", "The centaur strings his bow one last time."},
        {"Chiron", "If I were a Trojan, I would not aim for your chest."},
        {"Chiron", "I would aim for the small soft place your mother could not reach."},
        {"Achilles", "Then I will wear a sandal."},
        {"Chiron", "Then you will wear a sandal. And I will wait for you to forget."},
        {"Chiron", "Now. Face me."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 9 — Post-boss (outro)
    // ============================================================
    Scene outro;
    outro.id = "ch1_outro";
    outro.title = "PHTHIA \u2014 THE DEPARTURE";
    outro.lines = {
        {"", "Chiron lowers his bow. He is not bleeding. He is smiling."},
        {"Chiron", "Better than I was at your age."},
        {"Achilles", "That is not saying much."},
        {"Chiron", "It is saying enough."},
        {"", "He looks at the heel. He does not point."},
        {"Chiron", "You will leave this shore. You will go to Skyros, and then to Troy, and then to the dark."},
        {"Chiron", "And you will remember, on some night near the end, that an old horse told you not to forget the small things."},
        {"Achilles", "I will remember."},
        {"Chiron", "You will not. But the Fates are patient."},
        {"", "He walks into the cave. He does not look back."},
        {"", "The thread moves on."},
    };
    outro.next_scene_id = "";   // end of chapter → exits to LayerIntro for Layer 2
    scenes.push_back(std::move(outro));

    return scenes;
}

} // namespace story