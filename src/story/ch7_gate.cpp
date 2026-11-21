#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_7_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — Dusk at the Scean Gate
    // ============================================================
    Scene arrival;
    arrival.id = "ch7_arrival";
    arrival.title = "THE SCEAN GATE \u2014 DUSK";
    arrival.lines = {
        {"", "The gate at dusk. Apollo's light on the walls. One archer."},
        {"", "He is not a warrior. He is a prince who was told a story about himself, and believed it."},
        {"Paris", "Achilles."},
        {"Achilles", "Paris."},
        {"Paris", "You killed my brother."},
        {"Achilles", "Yes."},
        {"Paris", "I am going to kill you."},
        {"Achilles", "Many have tried."},
        {"Paris", "I know. That is why I am not going to aim at you."},
        {"", "He draws the bow. The light bends toward the arrow. Apollo is with him."},
    };
    arrival.next_scene_id = "ch7_combat_1";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — Divine archers
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch7_combat_1";
    combat_1.title = "THE GATE \u2014 APOLLO'S LIGHT";
    combat_1.lines = {
        {"", "The archers on the wall do not need to aim. The light aims for them."},
        {"", "Every arrow is a question: which part of you is still mortal?"},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 3 — Choice: the ghost of Hector
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch7_mid_1";
    mid_1.title = "THE GATE \u2014 THE GHOST OF HECTOR";
    mid_1.lines = {
        {"", "Hector's ghost stands between you and the archer. He is not here to fight. He is here to be seen."},
        {"Hector", "You dragged me."},
        {"Achilles", "I know."},
        {"Hector", "My father begged."},
        {"Achilles", "I know."},
        {"Hector", "Why?"},
        {"", "You have no answer that is not a wound."},
    };
    mid_1.choices = {
        { "Give him back his name. Speak it.",       "ch7_name" },
        { "Walk through him. He is only a ghost.",   "ch7_through" },
    };
    scenes.push_back(std::move(mid_1));

    Scene name;
    name.id = "ch7_name";
    name.title = "THE GATE \u2014 THE NAME";
    name.lines = {
        {"Achilles", "Hector of Troy. Prince. Husband. Father."},
        {"", "The ghost looks at you. Something in it loosens."},
        {"Hector", "That is all I wanted."},
        {"Achilles", "I know. I am sorry."},
        {"", "He steps aside. The gate is a little less heavy."},
    };
    name.next_scene_id = "ch7_combat_2";
    scenes.push_back(std::move(name));

    Scene through;
    through.id = "ch7_through";
    through.title = "THE GATE \u2014 WALKING THROUGH";
    through.lines = {
        {"", "You walk through him. He is cold, and then he is behind you."},
        {"Hector", "You will remember me."},
        {"Achilles", "I always have."},
        {"", "It is not an apology. It is a fact. The ghost does not care for either."},
    };
    through.next_scene_id = "ch7_combat_2";
    scenes.push_back(std::move(through));

    // ============================================================
    // SCENE 4 — The gate guard
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch7_combat_2";
    combat_2.title = "THE GATE \u2014 THE LAST GUARD";
    combat_2.lines = {
        {"", "The last men on the wall are not soldiers anymore. They are the city's refusal to fall."},
        {"", "They fight like they have already lost. That is the most dangerous kind."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 5 — Paris speaks
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch7_mid_2";
    mid_2.title = "THE GATE \u2014 THE ARCHER'S CREED";
    mid_2.lines = {
        {"Paris", "Do you know why the gods love you?"},
        {"Achilles", "Because I win."},
        {"Paris", "Because you are easy to aim at. You never step back."},
        {"Achilles", "Stepping back is not in me."},
        {"Paris", "I know. Apollo knows. That is the whole of the plan."},
        {"", "The light behind him sharpens. The arrow is not in the bow yet, but it is already flying."},
    };
    mid_2.choices = {
        { "Stand tall. Let him see no fear.",        "ch7_stand" },
        { "Guard the heel. Let him see the sandal.",  "ch7_guard" },
    };
    scenes.push_back(std::move(mid_2));

    Scene stand;
    stand.id = "ch7_stand";
    stand.title = "THE GATE \u2014 NO FEAR";
    stand.lines = {
        {"", "You stand as you have always stood. Straight. Open. Certain."},
        {"", "The archers on the wall lower their bows, just a little, in something that is not respect but is close."},
        {"Paris", "There he is."},
    };
    stand.next_scene_id = "ch7_combat_3";
    scenes.push_back(std::move(stand));

    Scene guard;
    guard.id = "ch7_guard";
    guard.title = "THE GATE \u2014 THE SANDAL";
    guard.lines = {
        {"", "You shift your weight. The heel turns inward. It is small. It is enough."},
        {"Paris", "What are you doing?"},
        {"Achilles", "Wearing a sandal. An old horse told me to."},
        {"", "For a moment, Paris hesitates. It is the first honest thing either of you has done."},
    };
    guard.next_scene_id = "ch7_combat_3";
    scenes.push_back(std::move(guard));

    // ============================================================
    // SCENE 6 — Last of Apollo's archers
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch7_combat_3";
    combat_3.title = "THE GATE \u2014 THE DIVINE LINE";
    combat_3.lines = {
        {"", "They are not men. They are the light made into archers, and the light has been told your name."},
        {"", "Every arrow finds the shape of your mortality. You have to be faster than a god."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 7 — Boss pre-brief
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch7_pre_boss";
    pre_boss.title = "THE GATE \u2014 APOLLO'S CHOSEN";
    pre_boss.lines = {
        {"Apollo", "The heel, Paris. Always the heel."},
        {"Paris", "I know."},
        {"Achilles", "You could shoot me anywhere else. It would not matter."},
        {"Paris", "That is what makes it a story."},
        {"Apollo", "And stories must end."},
        {"", "The arrow comes to the string. The world narrows to the small soft place your mother could not reach."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 8 — The choice (fragments complete)
    // ============================================================
    Scene choice_true;
    choice_true.id = "ch7_choice_true";
    choice_true.title = "THE GATE \u2014 THE QUESTION";
    choice_true.lines = {
        {"", "The arrow waits. Paris waits. The Fates wait. For once, the whole weave is still."},
        {"Atropos", "You know how you die."},
        {"Achilles", "Yes."},
        {"Atropos", "You know it was never the heel."},
        {"Achilles", "Yes."},
        {"", "You can accept the story. Or you can speak a different truth and let the shears decide."},
        {"Clotho", "Choose, son of Thetis. Choose for yourself, this once."},
    };
    choice_true.choices = {
        { "Accept the arrow. Become the myth.",         "ch7_end_a" },
        { "Refuse Apollo's framing. Speak Patroclus.", "ch7_end_c" },
    };
    scenes.push_back(std::move(choice_true));

    // ============================================================
    // SCENE 9 — The choice (fragments incomplete)
    // ============================================================
    Scene choice_false;
    choice_false.id = "ch7_choice_false";
    choice_false.title = "THE GATE \u2014 THE UNFINISHED TRUTH";
    choice_false.lines = {
        {"", "The arrow waits. You do not yet know the whole of the story, but the story knows the whole of you."},
        {"Atropos", "Some doors only open once you have gathered every splinter."},
        {"Achilles", "Then I have missed something."},
        {"Clotho", "You have. Perhaps in another weaving."},
        {"", "The arrow will not wait for another weaving."},
    };
    choice_false.choices = {
        { "Accept the arrow. Become the myth.", "ch7_end_a" },
    };
    scenes.push_back(std::move(choice_false));

    // ============================================================
    // SCENE 10 — Ending A
    // ============================================================
    Scene end_a;
    end_a.id = "ch7_end_a";
    end_a.title = "ENDING \u2014 KLEOS";
    end_a.lines = {
        {"", "You do not step aside. You have never stepped aside."},
        {"", "The arrow finds the heel. It is quick. It is quiet. It is exactly what the story promised."},
        {"", "The Fates cut the thread."},
        {"Atropos", "Greatest of the Greeks."},
        {"Lachesis", "Short-lived."},
        {"Clotho", "Never forgotten."},
        {"", "Your name is carved into myth. The Loom re-spins. Somewhere, a boy is being dipped in a river by a mother who thinks she is saving him."},
        {"", "You chose glory. Glory chose you back."},
    };
    end_a.next_scene_id = "";
    scenes.push_back(std::move(end_a));

    // ============================================================
    // SCENE 11 — Ending C
    // ============================================================
    Scene end_c;
    end_c.id = "ch7_end_c";
    end_c.title = "ENDING \u2014 THE WEAVER";
    end_c.lines = {
        {"", "You lower your guard."},
        {"Paris", "...what are you doing?"},
        {"Achilles", "Patroclus."},
        {"", "The name is not a battle cry. It is a hand held out. It is the first thing you have given the world that was not a weapon."},
        {"Atropos", "Wait."},
        {"", "The shears do not close."},
        {"", "The arrow drops. The light goes out. Apollo's voice is a wind in the walls, and the wind does not know what to say."},
        {"Atropos", "He gave us a reason."},
        {"Clotho", "Then the thread is not cut."},
        {"Lachesis", "It is tied."},
        {"", "You become part of the Loom. Not a thread. A weaver."},
        {"Achilles", "You cannot avoid death. You can only make it yours."},
        {"", "The heel was never the weakness. The rage was."},
        {"", "For the first time, the loom is quiet, and the quiet is kind."},
    };
    end_c.next_scene_id = "";
    scenes.push_back(std::move(end_c));

    return scenes;
}

} // namespace story
