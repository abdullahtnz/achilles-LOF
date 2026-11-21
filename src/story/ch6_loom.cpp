#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_6_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — The Gate of Horn
    // ============================================================
    Scene arrival;
    arrival.id = "ch6_arrival";
    arrival.title = "THE GATE OF HORN \u2014 THE WEAVE";
    arrival.lines = {
        {"", "You step through the gate and the world becomes thread."},
        {"", "Above you, below you, through you \u2014 the Loom. Every life is a line. Every line is a life."},
        {"", "And hanging in the dark, in your shape, are all the men you have been and failed to be."},
        {"Clotho", "He has come to the middle of the weave."},
        {"Lachesis", "He is early. He is late. He is always both."},
        {"Atropos", "He is still holding the sword."},
    };
    arrival.next_scene_id = "ch6_combat_1";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — Past death
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch6_combat_1";
    combat_1.title = "THE WEAVE \u2014 A PAST DEATH";
    combat_1.lines = {
        {"", "One of the shapes steps down from its thread. It has your face. It has your wound."},
        {"Past Achilles", "You died here before."},
        {"Achilles", "I do not remember."},
        {"Past Achilles", "You never do."},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 3 — The Fates speak
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch6_mid_1";
    mid_1.title = "THE WEAVE \u2014 THE THREE SISTERS";
    mid_1.lines = {
        {"Clotho", "Every death you have had is a thread. We kept them. We did not burn them."},
        {"Lachesis", "We measure them. They are all short."},
        {"Atropos", "We cut them. That is the work."},
        {"Achilles", "Then cut mine and be done."},
        {"Clotho", "We have cut it. Many times. It comes back."},
        {"Lachesis", "You come back."},
        {"Atropos", "We are beginning to think it is your choice."},
    };
    mid_1.choices = {
        { "Demand the truth of your death.",  "ch6_demand" },
        { "Say nothing. Let them weave.",      "ch6_silence" },
    };
    scenes.push_back(std::move(mid_1));

    Scene demand;
    demand.id = "ch6_demand";
    demand.title = "THE WEAVE \u2014 THE DEMAND";
    demand.lines = {
        {"Achilles", "Tell me how I die."},
        {"Atropos", "No."},
        {"Achilles", "You owe me that."},
        {"Clotho", "We owe you nothing. We made you from what your mother gave us."},
        {"Lachesis", "But we will show you. Because it is the only way you will ever stop."},
        {"", "The threads part. A beach. A gate. An arrow."},
        {"", "And before all of it, a boy named Patroclus, waiting for you to say yes."},
    };
    demand.next_scene_id = "ch6_combat_2";
    scenes.push_back(std::move(demand));

    Scene silence;
    silence.id = "ch6_silence";
    silence.title = "THE WEAVE \u2014 THE SILENCE";
    silence.lines = {
        {"", "You do not speak. The sisters watch you for a long time."},
        {"Atropos", "He is learning."},
        {"Clotho", "He is quiet. That is not the same."},
        {"Lachesis", "It is the beginning of the same."},
        {"", "The thread tightens around your wrist like a hand."},
    };
    silence.next_scene_id = "ch6_combat_2";
    scenes.push_back(std::move(silence));

    // ============================================================
    // SCENE 4 — More phantoms
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch6_combat_2";
    combat_2.title = "THE WEAVE \u2014 THE NAMELESS THREADS";
    combat_2.lines = {
        {"", "They come without faces, because you never remembered them. They died at Troy too."},
        {"", "They died because you were winning, or because you were angry, or because the war needed bodies."},
        {"Achilles", "I did not know them."},
        {"", "That has never been a defense."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 5 — Patroclus's death
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch6_mid_2";
    mid_2.title = "THE WEAVE \u2014 THE ANCHOR";
    mid_2.lines = {
        {"", "The Loom shows you a beach you have never left."},
        {"", "Patroclus in your armor. Patroclus alone. Patroclus falling."},
        {"Patroclus", "Achilles."},
        {"Achilles", "No."},
        {"", "The scene does not stop. It has never stopped. It is always happening."},
        {"Atropos", "This is the anchor."},
        {"Lachesis", "Every thread you have pulled leads back to this."},
        {"Clotho", "You did not die of an arrow. The arrow was only the instrument."},
    };
    mid_2.choices = {
        { "Rage. Make them pay for showing you.",   "ch6_rage" },
        { "Grieve. Let the scene finish.",           "ch6_grieve" },
    };
    scenes.push_back(std::move(mid_2));

    Scene rage;
    rage.id = "ch6_rage";
    rage.title = "THE WEAVE \u2014 THE OLD ROAD";
    rage.lines = {
        {"Achilles", "You show me this to hurt me."},
        {"Atropos", "We show you this because it is true."},
        {"Achilles", "Then I will make it false."},
        {"", "You reach for the rage. It is always there. It is always warm."},
        {"", "The phantoms step forward. They are waiting for you to become one of them."},
    };
    rage.next_scene_id = "ch6_combat_3";
    scenes.push_back(std::move(rage));

    Scene grieve;
    grieve.id = "ch6_grieve";
    grieve.title = "THE WEAVE \u2014 THE FIRST TEAR";
    grieve.lines = {
        {"", "You do not move. You watch him fall. You let it be true."},
        {"", "Something in your chest gives way. Not the door. Under the door. A foundation stone."},
        {"Achilles", "I am sorry."},
        {"Patroclus", "I know."},
        {"", "The scene ends. It has never ended before."},
        {"Clotho", "Oh."},
        {"Lachesis", "He wept."},
        {"Atropos", "Do not put the shears away yet."},
    };
    grieve.next_scene_id = "ch6_combat_3";
    scenes.push_back(std::move(grieve));

    // ============================================================
    // SCENE 6 — The last phantom
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch6_combat_3";
    combat_3.title = "THE WEAVE \u2014 THE PHANTOM OF ACHILLES";
    combat_3.lines = {
        {"", "It wears every wound you have ever taken. It moves like all of your deaths at once."},
        {"Phantom Achilles", "Stay."},
        {"Achilles", "I cannot."},
        {"Phantom Achilles", "You never could."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 7 — Boss pre-brief
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch6_pre_boss";
    pre_boss.title = "THE WEAVE \u2014 THE MOIRAI";
    pre_boss.lines = {
        {"Clotho", "Spin the thread."},
        {"Lachesis", "Measure the life."},
        {"Atropos", "Cut \u2014 unless he gives us a reason not to."},
        {"Achilles", "What reason could I give you?"},
        {"Atropos", "That is the question, son of Thetis. It is the only question."},
        {"", "The shears open. The three sisters stand as one."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 8 — Outro
    // ============================================================
    Scene outro;
    outro.id = "ch6_outro";
    outro.title = "THE WEAVE \u2014 THE THREAD THAT TIES";
    outro.lines = {
        {"", "The shears close on nothing. The sisters look at you, and for the first time, they do not look like judges."},
        {"Atropos", "Go to the gate."},
        {"Achilles", "To die?"},
        {"Atropos", "To choose."},
        {"Lachesis", "There is one arrow left in the world with your name on it."},
        {"Clotho", "And one question you have never answered."},
        {"", "The threads part. The Scean Gate waits at dusk, and Paris waits behind the light."},
        {"", "You understand now that the heel was never the weakness."},
        {"", "The rage was."},
    };
    outro.next_scene_id = "";
    scenes.push_back(std::move(outro));

    return scenes;
}

} // namespace story
