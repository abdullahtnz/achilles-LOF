#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_3_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — Arrival at Troy Shores
    // ============================================================
    Scene arrival;
    arrival.id = "ch3_arrival";
    arrival.title = "THE SHORES OF TROY \u2014 THE HOST GATHERS";
    arrival.lines = {
        {"", "The ships are black against the morning sun. A thousand of them. More."},
        {"", "The beach is mud and fire. The war has already begun without you. It will not end without you."},
        {"", "You step off the ship and the sand under you is warm. Not from the sun. From the dead who were here first."},
        {"Achilles", "How many have died?"},
        {"Soldier", "Enough, my lord."},
        {"Achilles", "It is not enough. It is never enough."},
        {"", "There is a young man waiting at the edge of the camp. He is watching you as though he has been watching you his whole life."},
    };
    arrival.next_scene_id = "ch3_patroclus_intro";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — Patroclus intro + choice
    // ============================================================
    Scene patroclus;
    patroclus.id = "ch3_patroclus_intro";
    patroclus.title = "THE SHORES \u2014 PATROCLUS";
    patroclus.lines = {
        {"Patroclus", "Achilles."},
        {"Achilles", "You know my name."},
        {"Patroclus", "Everyone knows your name. I know it differently. I have known it for a long time."},
        {"", "He is not afraid of you. That is uncommon."},
        {"Patroclus", "I came to fight beside you."},
        {"Achilles", "Why?"},
        {"Patroclus", "Because someone has to. Because you will die without someone. Because I would rather be with you than without."},
        {"", "There is no guile in him. There is only the plain truth."},
    };
    patroclus.choices = {
        { "Accept him. Let him fight at your side.", "ch3_accept_patroclus" },
        { "Refuse. He stays at camp.",               "ch3_refuse_patroclus" },
    };
    scenes.push_back(std::move(patroclus));

    Scene accept;
    accept.id = "ch3_accept_patroclus";
    accept.title = "THE SHORES \u2014 A COMPANION";
    accept.lines = {
        {"Achilles", "Then fight."},
        {"Patroclus", "I will."},
        {"", "He smiles. It is not the smile of a soldier. It is the smile of someone who has been waiting a long time to be told yes."},
        {"Patroclus", "You will not regret this."},
        {"Achilles", "I do not regret things."},
        {"Patroclus", "You will."},
        {"", "He says it without cruelty. He says it as a fact, and it is not about him."},
    };
    accept.next_scene_id = "ch3_combat_1";
    scenes.push_back(std::move(accept));

    Scene refuse;
    refuse.id = "ch3_refuse_patroclus";
    refuse.title = "THE SHORES \u2014 THE REFUSAL";
    refuse.lines = {
        {"Achilles", "No."},
        {"Patroclus", "No?"},
        {"Achilles", "You will stay at camp."},
        {"Patroclus", "I came across the sea."},
        {"Achilles", "You came across the sea for a man who has told you no. Stay at camp. Fight your own war. Not mine."},
        {"", "He does not argue. He bows. He walks away, and he does not look back."},
        {"", "You will see him again. You will wish you had said yes."},
    };
    refuse.next_scene_id = "ch3_combat_1";
    scenes.push_back(std::move(refuse));

    // ============================================================
    // SCENE 3 — Combat 1 pre-brief
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch3_combat_1";
    combat_1.title = "THE SHORES \u2014 FIRST BLOOD";
    combat_1.lines = {
        {"", "The Trojans are already breaking against the Greek line. You are the line."},
        {"", "The first Trojan you face is a boy. Fourteen, perhaps. He holds a spear he does not know how to use."},
        {"Achilles", "Put it down."},
        {"", "The boy does not put it down. He will die on this beach. He does not know that yet. You do."},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 4 — Mid 1 (Patroclus wounded choice)
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch3_mid_1";
    mid_1.title = "THE SHORES \u2014 THE WOUND";
    mid_1.lines = {
        {"", "An archer's arrow catches Patroclus in the shoulder. He does not fall. He cannot lift his sword."},
        {"Patroclus", "I can still fight."},
        {"Achilles", "Your arm is broken."},
        {"Patroclus", "I can still fight."},
        {"Achilles", "You cannot."},
        {"", "Behind him, the line is buckling. He is the left flank. You are the right."},
    };
    mid_1.choices = {
        { "Pull him back. He lives.",  "ch3_pull_back" },
        { "Let him fight. He stands.", "ch3_let_fight" },
    };
    scenes.push_back(std::move(mid_1));

    Scene pull;
    pull.id = "ch3_pull_back";
    pull.title = "THE SHORES \u2014 THE PULL";
    pull.lines = {
        {"", "You grab him by the shoulder and drag him back behind the shield wall."},
        {"Patroclus", "I could have held."},
        {"Achilles", "You could have died."},
        {"Patroclus", "I would have held."},
        {"", "His wound will heal. His pride will not, not entirely. But he lives."},
        {"", "You do not say what you are thinking. What you are thinking is: I do not want to lose you."},
    };
    pull.next_scene_id = "ch3_combat_2";
    scenes.push_back(std::move(pull));

    Scene let_fight;
    let_fight.id = "ch3_let_fight";
    let_fight.title = "THE SHORES \u2014 THE STAND";
    let_fight.lines = {
        {"Achilles", "Then hold."},
        {"", "He holds. He fights one-handed for twenty minutes. He kills three men. He does not fall."},
        {"", "When the line is safe, he comes back to you with his arm hanging, and he is smiling through the blood."},
        {"Patroclus", "I told you."},
        {"Achilles", "You told me."},
        {"", "You do not tell him the other thing. The other thing is: you did not need him to prove it. You only needed him to live."},
    };
    let_fight.next_scene_id = "ch3_combat_2";
    scenes.push_back(std::move(let_fight));

    // ============================================================
    // SCENE 5 — Combat 2 pre-brief
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch3_combat_2";
    combat_2.title = "THE SHORES \u2014 THE ARCHER'S LINE";
    combat_2.lines = {
        {"", "The Trojan archers have found their rhythm. Arrows come in waves."},
        {"Patroclus", "They are aiming at the gaps in the wall."},
        {"Achilles", "They are aiming at us."},
        {"Patroclus", "Then we give them nothing to aim at."},
        {"", "You have never seen him fight like this."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 6 — Mid 2 (Trojan prisoner choice)
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch3_mid_2";
    mid_2.title = "THE SHORES \u2014 THE PRISONER";
    mid_2.lines = {
        {"", "You have captured a Trojan scout. He is young. He is shaking. He has told you where Hector's vanguard is camped."},
        {"Scout", "Please. I have a sister."},
        {"Achilles", "Everyone has a sister."},
        {"Scout", "Please."},
        {"", "He looks at you. He is not the first prisoner you have taken. He is not the last."},
    };
    mid_2.choices = {
        { "Kill him. He has told you what you need.", "ch3_execute" },
        { "Let him go. He is not a weapon.",          "ch3_release" },
    };
    scenes.push_back(std::move(mid_2));

    Scene execute;
    execute.id = "ch3_execute";
    execute.title = "THE SHORES \u2014 THE BLADE";
    execute.lines = {
        {"", "You kill him. It takes one motion. He does not scream."},
        {"", "Patroclus says nothing. But he looks at you afterward, and his eyes are different. Not frightened. Not angry."},
        {"Patroclus", "You did not have to do that."},
        {"Achilles", "Yes I did."},
        {"Patroclus", "No. You wanted to."},
        {"", "You do not answer. The Ares in you does not need to."},
    };
    execute.next_scene_id = "ch3_combat_3";
    scenes.push_back(std::move(execute));

    Scene release;
    release.id = "ch3_release";
    release.title = "THE SHORES \u2014 THE MERCY";
    release.lines = {
        {"", "You cut his bonds and point at the beach. He runs. He does not look back."},
        {"Patroclus", "You let him go."},
        {"Achilles", "He was not a weapon."},
        {"Patroclus", "He could have been."},
        {"Achilles", "He was not."},
        {"", "Patroclus looks at you differently. Not with gratitude. With something rarer \u2014 recognition."},
        {"", "The Trojans will remember that you could have killed and did not. They will not thank you. But they will know."},
    };
    release.next_scene_id = "ch3_combat_3";
    scenes.push_back(std::move(release));

    // ============================================================
    // SCENE 7 — Combat 3 pre-brief (honor guard)
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch3_combat_3";
    combat_3.title = "THE SHORES \u2014 THE HONOR GUARD";
    combat_3.lines = {
        {"", "They come at dusk \u2014 the last line before the vanguard. They do not shout."},
        {"", "They have watched you all day. They know how you move."},
        {"Patroclus", "They are not afraid of you."},
        {"Achilles", "Good. Fear makes men clumsy."},
        {"", "The first one steps forward and raises his spear. He is not clumsy."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 8 — Boss pre-brief (Hector's Vanguard)
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch3_pre_boss";
    pre_boss.title = "THE SHORES \u2014 HECTOR'S VANGUARD";
    pre_boss.lines = {
        {"", "The vanguard is not a man. It is forty men. They hold a line that has not broken in three months."},
        {"Patroclus", "We cannot break that."},
        {"Achilles", "We do not have to. We only have to make them break first."},
        {"", "You walk toward the vanguard alone. Forty spears turn toward you."},
        {"", "You do not slow down."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 8 — Post-boss outro
    // ============================================================
    Scene outro;
    outro.id = "ch3_outro";
    outro.title = "THE SHORES \u2014 AFTER THE VANGUARD";
    outro.lines = {
        {"", "The vanguard breaks. Not because you are stronger. Because you are louder."},
        {"", "The Trojans fall back behind the wall. The wall is taller than anything on the beach. It is the wall of Troy."},
        {"Patroclus", "You did it."},
        {"Achilles", "We did it."},
        {"Patroclus", "I helped."},
        {"Achilles", "You helped."},
        {"", "He smiles. You have never seen him so tired. He is not afraid of the wall. He is not afraid of anything on the beach."},
        {"", "Neither are you. That is the problem."},
        {"", "Beyond the wall, the plain of Scamander waits. And beyond the river, the walls. And beyond the walls, Hector. And beyond Hector, the small soft place your mother could not reach."},
        {"", "The thread moves on."},
    };
    outro.next_scene_id = "";
    scenes.push_back(std::move(outro));

    return scenes;
}

} // namespace story