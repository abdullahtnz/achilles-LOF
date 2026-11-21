#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_2_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — Arrival at Skyros
    // ============================================================
    Scene arrival;
    arrival.id = "ch2_arrival";
    arrival.title = "SKYROS \u2014 THE COURT OF WOMEN";
    arrival.lines = {
        {"", "Skyros is white marble and cold light. The women of the court move like water through the halls."},
        {"", "You are dressed as one of them. Your mother's idea. The prophecy said Troy, so you have gone where Troy cannot reach."},
        {"", "But the Fates do not lose threads."},
        {"Thetis", "You will be safe here, my son. Live quietly. Grow old. That is the life I chose for you."},
        {"Achilles", "You chose the water. I did not choose the quiet."},
        {"", "Thetis does not answer. She has never answered that."},
        {"", "That night, a merchant comes to the court. His name is Odysseus, though he does not say so at first."},
        {"", "He lays out trinkets on a table. And among the trinkets \u2014 a sword."},
        {"", "You do not remember reaching for it. You only remember the weight of it in your hand."},
        {"", "The world tears open."},
    };
    arrival.next_scene_id = "ch2_deidamia_intro";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — Deidamia intro + first choice
    // ============================================================
    Scene deidamia;
    deidamia.id = "ch2_deidamia_intro";
    deidamia.title = "SKYROS \u2014 THE WOMAN LEFT BEHIND";
    deidamia.lines = {
        {"", "The court freezes. A woman steps forward. You know her name before she says it."},
        {"Deidamia", "You came back."},
        {"Achilles", "I never left."},
        {"Deidamia", "You left the morning after. You did not say goodbye. You did not say anything."},
        {"", "She is holding a spindle. Her hands are white around it."},
        {"Deidamia", "I bore a son for you. I named him after your father."},
        {"Achilles", "I did not know."},
        {"Deidamia", "You did not ask."},
        {"", "The grief in her is not a weapon. Not yet. But it could become one."},
    };
    deidamia.choices = {
        { "Speak to her softly.",        "ch2_deidamia_speak" },
        { "Draw your sword silently.",   "ch2_deidamia_silence" },
    };
    scenes.push_back(std::move(deidamia));

    // ============================================================
    // SCENE 2A — Speak softly (Athena-flavored)
    // ============================================================
    Scene speak;
    speak.id = "ch2_deidamia_speak";
    speak.title = "SKYROS \u2014 A SOFT ANSWER";
    speak.lines = {
        {"Achilles", "I was young. And afraid."},
        {"Deidamia", "You were never afraid."},
        {"Achilles", "I was afraid of staying."},
        {"", "She looks at you for a long time. Then she sets the spindle down."},
        {"Deidamia", "That is the first true thing you have said to me."},
        {"", "Her grief settles. It does not go. But it settles, and it does not become a weapon."},
        {"", "You will face her in the courtyard. She will not be merciful. But she will not be cruel."},
    };
    speak.next_scene_id = "ch2_combat_1";
    scenes.push_back(std::move(speak));

    // ============================================================
    // SCENE 2B — Silence (Ares-flavored)
    // ============================================================
    Scene silence;
    silence.id = "ch2_deidamia_silence";
    silence.title = "SKYROS \u2014 THE SWORD ANSWERS";
    silence.lines = {
        {"", "You do not speak. You draw."},
        {"", "Something in her eyes closes."},
        {"Deidamia", "Of course."},
        {"Deidamia", "You are what the stories said you were."},
        {"Achilles", "And you are what I left."},
        {"", "She does not weep. She does not flinch. She raises her spindle like a sword."},
        {"", "Her grief has become a weapon. She will wield it."},
    };
    silence.next_scene_id = "ch2_combat_1";
    scenes.push_back(std::move(silence));

    // ============================================================
    // SCENE 3 — Combat 1 pre-brief
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch2_combat_1";
    combat_1.title = "SKYROS \u2014 THE FIRST ILLUSION";
    combat_1.lines = {
        {"", "The courtyard is silver in the moonlight. An illusion of you stands at the far end."},
        {"Achilles", "What is this?"},
        {"Deidamia", "It is you. It is what you left behind."},
        {"Achilles", "It is not me."},
        {"Deidamia", "It has your stance. Your reach. Your impatience. Kill it and see."},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 4 — Mid 1 (Odysseus merchant choice)
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch2_mid_1";
    mid_1.title = "SKYROS \u2014 A MERCHANT'S GIFT";
    mid_1.lines = {
        {"", "Odysseus steps from the shadow of a column. He has been watching."},
        {"Odysseus", "You fight well."},
        {"Achilles", "I know."},
        {"Odysseus", "You fought an illusion of yourself and won. That is something few can say."},
        {"Achilles", "It was slower than me."},
        {"Odysseus", "It was slower because you have become faster. That is the trick."},
        {"", "He opens a palm. A small token is in it. Bronze, with a crest."},
        {"Odysseus", "For the heel. A gift."},
    };
    mid_1.choices = {
        { "Accept the token.",  "ch2_mid_1_accept" },
        { "Refuse. You need nothing.", "ch2_mid_1_refuse" },
    };
    scenes.push_back(std::move(mid_1));

    Scene accept;
    accept.id = "ch2_mid_1_accept";
    accept.title = "SKYROS \u2014 THE TOKEN";
    accept.lines = {
        {"Achilles", "What does it do?"},
        {"Odysseus", "It remembers a death you have not yet had. When the moment comes, it will not save you."},
        {"Achilles", "Then what use is it?"},
        {"Odysseus", "It will give you one more breath. Sometimes that is enough."},
        {"", "You take the token. It is warm."},
    };
    accept.next_scene_id = "ch2_combat_2";
    scenes.push_back(std::move(accept));

    Scene refuse;
    refuse.id = "ch2_mid_1_refuse";
    refuse.title = "SKYROS \u2014 THE REFUSAL";
    refuse.lines = {
        {"Achilles", "I do not need a token. I am the token."},
        {"Odysseus", "That is what you think now."},
        {"Achilles", "That is what I will think at Troy."},
        {"Odysseus", "Yes. It is."},
        {"", "He closes his hand. The bronze disappears."},
        {"", "You do not ask where it goes. You will not need to."},
    };
    refuse.next_scene_id = "ch2_combat_2";
    scenes.push_back(std::move(refuse));

    // ============================================================
    // SCENE 5 — Combat 2 pre-brief
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch2_combat_2";
    combat_2.title = "SKYROS \u2014 THE MIRRORED DUELIST";
    combat_2.lines = {
        {"", "Another shape steps from the dark. This one is not your reflection. It is your rival."},
        {"Deidamia", "This one is what you could have been, if you had stayed."},
        {"Achilles", "I would have been a farmer."},
        {"Deidamia", "You would have been a father."},
        {"", "The shape moves. It is very fast."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 6 — Mid 2 (final pre-boss)
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch2_mid_2";
    mid_2.title = "SKYROS \u2014 THE LAST ILLUSION";
    mid_2.lines = {
        {"", "Deidamia waits at the center of the courtyard. She is not an illusion. She is not a shade."},
        {"Deidamia", "One more."},
        {"Achilles", "And then?"},
        {"Deidamia", "And then you may leave. As you did before."},
        {"Achilles", "Deidamia \u2014"},
        {"Deidamia", "Fight me. Do not speak. You are only good at one of the two."},
    };
    mid_2.next_scene_id = "ch2_combat_3";
    scenes.push_back(std::move(mid_2));

    // ============================================================
    // SCENE 7 — Combat 3 pre-brief
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch2_combat_3";
    combat_3.title = "SKYROS \u2014 LYCOMEDES' GUARD";
    combat_3.lines = {
        {"", "The king's guards move to block the exits. Deidamia does not call them off."},
        {"Guard", "You will not leave this court alive."},
        {"Achilles", "I have already left it alive. You are the ones who came back."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 8 — Boss pre-brief (Deidamia)
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch2_pre_boss";
    pre_boss.title = "SKYROS \u2014 DEIDAMIA'S GRIEF";
    pre_boss.lines = {
        {"", "She stands at the edge of the courtyard. The moonlight does not touch her."},
        {"Deidamia", "I have been waiting for you for six years."},
        {"Achilles", "I did not know."},
        {"Deidamia", "You never know. That is what you are."},
        {"Deidamia", "Come. Let me show you what I became while you were choosing the sword."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 9 — Post-boss (Deidamia's parting)
    // ============================================================
    Scene post_boss;
    post_boss.id = "ch2_post_boss";
    post_boss.title = "SKYROS \u2014 THE PARTING";
    post_boss.lines = {
        {"", "She kneels. She does not weep. She is very still."},
        {"Deidamia", "You will die at Troy."},
        {"Achilles", "I know."},
        {"Deidamia", "You will die because you could not stop reaching."},
        {"Achilles", "I know."},
        {"Deidamia", "Then why are you still holding the sword?"},
        {"", "You look down. The blade is warm in your hand. You do not remember picking it up."},
        {"Deidamia", "Go, then. There is nothing here for you that you did not leave."},
        {"", "The Fates do not speak. They do not need to. The weave tightens."},
        {"", "There is a choice to be made before you leave Skyros. Two gods wait."},
    };
    post_boss.next_scene_id = "";
    scenes.push_back(std::move(post_boss));

    return scenes;
}

} // namespace story