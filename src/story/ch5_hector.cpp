#include "dialogue.hpp"

namespace story {

std::vector<Scene> chapter_5_scenes() {
    std::vector<Scene> scenes;

    // ============================================================
    // SCENE 1 — The walls
    // ============================================================
    Scene arrival;
    arrival.id = "ch5_arrival";
    arrival.title = "THE WALLS OF TROY \u2014 THE SCREAN GATE";
    arrival.lines = {
        {"", "The wall is taller than anything on the beach. It has stood for a thousand years. It will not stand another day."},
        {"", "The gates are open. A single man waits in the shadow of them."},
        {"", "He is not running. He is not calling for help. He is waiting for you."},
        {"Achilles", "Hector."},
        {"Hector", "Achilles."},
        {"", "There is no hatred in his voice. That is the strangest thing of all."},
    };
    arrival.next_scene_id = "ch5_combat_1";
    scenes.push_back(std::move(arrival));

    // ============================================================
    // SCENE 2 — Wall defenders
    // ============================================================
    Scene combat_1;
    combat_1.id = "ch5_combat_1";
    combat_1.title = "THE WALLS \u2014 THE LAST DEFENDERS";
    combat_1.lines = {
        {"", "They hold the gate because their families are behind it. You hold your spear because it is all you know."},
        {"", "For a moment you are the same. Then the spears meet."},
    };
    combat_1.next_scene_id = "";
    scenes.push_back(std::move(combat_1));

    // ============================================================
    // SCENE 3 — Choice: the archers on the wall
    // ============================================================
    Scene mid_1;
    mid_1.id = "ch5_mid_1";
    mid_1.title = "THE WALLS \u2014 APOLLO'S ARCHERS";
    mid_1.lines = {
        {"", "Archers line the battlements. The sun is behind them. Their arrows are only visible when they are already close."},
        {"Patroclus", "They are aiming low. At your feet."},
        {"Achilles", "They are learning."},
        {"Patroclus", "Someone is teaching them."},
        {"", "A glint of gold on the wall. A god's light, perhaps. Or only a shield."},
    };
    mid_1.choices = {
        { "Charge the wall. Break the archers.",  "ch5_charge" },
        { "Take cover. Bring them down slowly.",  "ch5_cover" },
    };
    scenes.push_back(std::move(mid_1));

    Scene charge;
    charge.id = "ch5_charge";
    charge.title = "THE WALLS \u2014 THE CHARGE";
    charge.lines = {
        {"", "You run at the wall. Arrows fall around you like rain and miss, because you are already where they are not."},
        {"", "The archers do not expect a man to run toward the light. They were taught to fear it."},
    };
    charge.next_scene_id = "ch5_combat_2";
    scenes.push_back(std::move(charge));

    Scene cover;
    cover.id = "ch5_cover";
    cover.title = "THE WALLS \u2014 THE COVER";
    cover.lines = {
        {"", "You move from shield to corpse to broken cart. It is slower. It is safer. It is not glory."},
        {"Patroclus", "You are being careful."},
        {"Achilles", "Someone taught me."},
        {"", "He smiles, though he does not know why."},
    };
    cover.next_scene_id = "ch5_combat_2";
    scenes.push_back(std::move(cover));

    // ============================================================
    // SCENE 4 — Archers
    // ============================================================
    Scene combat_2;
    combat_2.id = "ch5_combat_2";
    combat_2.title = "THE WALLS \u2014 THE ARCHER'S LINE";
    combat_2.lines = {
        {"", "The arrows come in waves now. Between the waves, the archers stand still and watch you."},
        {"", "One of them is not aiming at your chest. He is aiming at the ground in front of your feet."},
        {"Achilles", "Clever."},
    };
    combat_2.next_scene_id = "";
    scenes.push_back(std::move(combat_2));

    // ============================================================
    // SCENE 5 — Hector's honor guard
    // ============================================================
    Scene mid_2;
    mid_2.id = "ch5_mid_2";
    mid_2.title = "THE WALLS \u2014 THE HONOR GUARD";
    mid_2.lines = {
        {"", "They come down from the gate in step. Twelve men. They do not look at you. They look at the ground where you will stand."},
        {"Hector", "Enough."},
        {"", "They stop. Hector walks between them and does not look at them either."},
        {"Hector", "This is between us. Let them go."},
        {"Achilles", "Why would I?"},
        {"Hector", "Because you want to be remembered for more than the number."},
        {"", "You do not answer. You are not sure he is wrong."},
    };
    mid_2.choices = {
        { "Let the guard go. Face Hector alone.", "ch5_let_go" },
        { "Cut them down. Leave nothing.",         "ch5_cut_down" },
    };
    scenes.push_back(std::move(mid_2));

    Scene let_go;
    let_go.id = "ch5_let_go";
    let_go.title = "THE WALLS \u2014 THE HONOR";
    let_go.lines = {
        {"Achilles", "Go."},
        {"", "They go. Hector watches them until they are inside the gate. Then he turns back to you."},
        {"Hector", "Thank you."},
        {"Achilles", "Do not thank me. I am still going to kill you."},
        {"Hector", "I know. That is why I thanked you."},
    };
    let_go.next_scene_id = "ch5_combat_3";
    scenes.push_back(std::move(let_go));

    Scene cut_down;
    cut_down.id = "ch5_cut_down";
    cut_down.title = "THE WALLS \u2014 NOTHING LEFT";
    cut_down.lines = {
        {"", "You do not let them go. It takes less time than it should."},
        {"Hector", "You did not have to."},
        {"Achilles", "Yes I did."},
        {"Hector", "No. You wanted to."},
        {"", "It is Patroclus's voice in your head, not Hector's. You hate that you hear it."},
    };
    cut_down.next_scene_id = "ch5_combat_3";
    scenes.push_back(std::move(cut_down));

    // ============================================================
    // SCENE 6 — The gate itself
    // ============================================================
    Scene combat_3;
    combat_3.id = "ch5_combat_3";
    combat_3.title = "THE WALLS \u2014 THE GATE OF HECTOR";
    combat_3.lines = {
        {"", "The gate is open behind him. Beyond it, the city. Inside the city, a wife, a child, a father who will beg for a body."},
        {"", "He does not look back at them. He looks at you."},
    };
    combat_3.next_scene_id = "";
    scenes.push_back(std::move(combat_3));

    // ============================================================
    // SCENE 7 — Boss pre-brief
    // ============================================================
    Scene pre_boss;
    pre_boss.id = "ch5_pre_boss";
    pre_boss.title = "THE WALLS \u2014 HECTOR, PRINCE OF TROY";
    pre_boss.lines = {
        {"Hector", "If I kill you, they will sing of me."},
        {"Achilles", "They will sing of me."},
        {"Hector", "I know."},
        {"Hector", "That is why I am not afraid."},
        {"", "He raises his spear. The Scean Gate closes behind him. There is no one left to come between you."},
    };
    pre_boss.next_scene_id = "";
    scenes.push_back(std::move(pre_boss));

    // ============================================================
    // SCENE 8 — Outro
    // ============================================================
    Scene outro;
    outro.id = "ch5_outro";
    outro.title = "THE WALLS \u2014 THE BODY";
    outro.lines = {
        {"", "He falls. The wall does not."},
        {"", "You drag the body behind your chariot, and the Trojan ghosts claw at you from the gate, and the city screams."},
        {"", "You do not stop. A part of you wants to stop. That part is small and it is getting smaller."},
        {"Hector", "...my body..."},
        {"", "He asks, though he is already dead. You do not answer, because the answer is no."},
        {"", "You wear his armor. It fits badly. You do not take it off."},
        {"", "The Fates watch from the loom. One of them is crying. One of them is not."},
        {"", "The thread moves on."},
    };
    outro.next_scene_id = "";
    scenes.push_back(std::move(outro));

    return scenes;
}

} // namespace story
