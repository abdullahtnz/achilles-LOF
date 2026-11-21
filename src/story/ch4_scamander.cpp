#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_4_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — The river plain
    // ============================================================
    Scene arrival;
    arrival.id = "ch4_arrival";
    arrival.title = "THE PLAIN OF SCAMANDER \u2014 THE WATER RISES";
    arrival.lines = {
        {"", "The plain is choked with the dead. The river moves slowly through them, and the water is not clean."},
        {"", "Every man you kill here makes the water rise a little higher."},
        {"Achilles", "Let it rise."},
        {"", "There is something in your chest that was not there before. It is not grief. It is not fear. It is a door, and it is open."},
        {"", "The rage is a road. You have only ever known how to walk forward."},
    };
    arrival.next_scene_id = "ch4_combat_1";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — First wave
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch4_combat_1";
    combat_1.title = "SCAMANDER \u2014 THE DROWNED LINE";
    combat_1.lines = {
        {"", "They rise out of the shallows with river-mud in their mouths. Some of them are still trying to breathe."},
        {"Achilles", "Stay down."},
        {"", "They do not stay down. The river will not let them."},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 3 — First choice: feed the rage or hold it
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch4_mid_1";
    mid_1.title = "SCAMANDER \u2014 THE DOOR IN THE CHEST";
    mid_1.lines = {
        {"", "The water is at your ankles now. It was at your feet a moment ago."},
        {"", "The rage is warm. It makes the killing easy. It makes everything easy."},
        {"Patroclus", "Achilles. Your hands."},
        {"Achilles", "What of them?"},
        {"Patroclus", "They are shaking."},
        {"", "You look down. They are. You had not noticed."},
    };
    mid_1.choices = {
        { "Open the door. Let the rage out.",       "ch4_rage" },
        { "Close it. Breathe. Hold the line.",      "ch4_hold" },
    };
    scenes.push_back(std::move(mid_1));

    Scene rage;
    rage.id = "ch4_rage";
    rage.title = "SCAMANDER \u2014 THE OPEN DOOR";
    rage.lines = {
        {"Achilles", "No. Let it come."},
        {"", "The warmth becomes heat. The plain narrows to a red line. You are faster. You are louder. You are less."},
        {"Patroclus", "Achilles \u2014"},
        {"Achilles", "Do not speak. Fight."},
        {"", "He fights. He does not speak again for a long time."},
    };
    rage.next_scene_id = "ch4_combat_2";
    scenes.push_back(std::move(rage));

    Scene hold;
    hold.id = "ch4_hold";
    hold.title = "SCAMANDER \u2014 THE HELD BREATH";
    hold.lines = {
        {"Achilles", "No."},
        {"", "You close the door. It costs you something. You do not know what, yet."},
        {"Patroclus", "There you are."},
        {"Achilles", "I never left."},
        {"Patroclus", "You did. For a moment. Long enough to be someone else."},
        {"", "The water stops rising. It does not fall. It waits."},
    };
    hold.next_scene_id = "ch4_combat_2";
    scenes.push_back(std::move(hold));

    // ============================================================
    // SCENE 4 — Second wave
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch4_combat_2";
    combat_2.title = "SCAMANDER \u2014 THE RIVER'S DAUGHTERS";
    combat_2.lines = {
        {"", "Nymphs rise from the current, beautiful and wrong, their hair full of drowned men."},
        {"", "They sing in a language older than Greek. It sounds like your mother's voice."},
        {"Achilles", "I have no time for songs."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 5 — The river speaks
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch4_mid_2";
    mid_2.title = "SCAMANDER \u2014 THE RIVER SPEAKS";
    mid_2.lines = {
        {"Scamander", "You choke me with your dead."},
        {"Achilles", "They came to my shore first."},
        {"Scamander", "They are my water now. Every man you send to me is a stone in my throat."},
        {"Achilles", "Then rise."},
        {"Scamander", "I will."},
        {"", "The plain is a lake. The corpses float. The corpses stand."},
    };
    mid_2.choices = {
        { "Turn the dead against the river.",     "ch4_turn_dead" },
        { "Wade on. Reach the far bank.",         "ch4_wade" },
    };
    scenes.push_back(std::move(mid_2));

    Scene turn_dead;
    turn_dead.id = "ch4_turn_dead";
    turn_dead.title = "SCAMANDER \u2014 THE CORPSE-PUPPETS";
    turn_dead.lines = {
        {"", "You take the dead and make them a wall. The river has to swallow its own."},
        {"Scamander", "You are a sickness."},
        {"Achilles", "I am what the war made."},
        {"", "The wall of bodies holds. For now."},
    };
    turn_dead.next_scene_id = "ch4_combat_3";
    scenes.push_back(std::move(turn_dead));

    Scene wade;
    wade.id = "ch4_wade";
    wade.title = "SCAMANDER \u2014 THE FAR BANK";
    wade.lines = {
        {"", "You do not look at the faces in the water. You look at the far bank."},
        {"", "It is closer than it was. It has always been closer than it was."},
        {"Patroclus", "The ground. There."},
        {"", "You reach the shallows. The river screams behind you and sends its last shape."},
    };
    wade.next_scene_id = "ch4_combat_3";
    scenes.push_back(std::move(wade));

    // ============================================================
    // SCENE 6 — Third wave
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch4_combat_3";
    combat_3.title = "SCAMANDER \u2014 THE LAST SHAPE";
    combat_3.lines = {
        {"", "It wears the shape of a man it has drowned. It moves like water and hates like a god."},
        {"", "Behind it, the river gathers itself into a fist."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 7 — Boss pre-brief
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch4_pre_boss";
    pre_boss.title = "SCAMANDER \u2014 THE RIVER GOD";
    pre_boss.lines = {
        {"", "The water rises to your chest. The god is the river. The river is the god."},
        {"Scamander", "You will not cross me."},
        {"Achilles", "I have crossed everything else."},
        {"Scamander", "Not this. This is where the wrath becomes flood."},
        {"", "You plant your feet. The water breaks around you."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 8 — Outro
    // ============================================================
    Scene outro;
    outro.id = "ch4_outro";
    outro.title = "SCAMANDER \u2014 AFTER THE FLOOD";
    outro.lines = {
        {"", "The river falls back. The plain is glass. The dead are still dead."},
        {"", "Your rage is quiet. Not gone. Quiet. That is worse."},
        {"Patroclus", "You could have drowned."},
        {"Achilles", "I could have."},
        {"Patroclus", "You did not."},
        {"Achilles", "Not this time."},
        {"", "Beyond the plain, the wall of Troy stands in the last light. Somewhere behind it, a prince is waiting."},
        {"", "The thread moves on."},
    };
    outro.next_scene_id = "";
    scenes.push_back(std::move(outro));

    return scenes;
}

} // namespace story
