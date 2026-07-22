#include "Names.h"
#include "Player_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(Player_G, Friends_G)
    ADD_BEHAVIOR(CheckWarpColor)
    ADD_BEHAVIOR(CheckSoundClothe)
    ADD_BEHAVIOR(CheckAnimEvent)
    ADD_BEHAVIOR(CheckHit)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<Player_G*> Player_G::instances;

void Player_G::AddToStaticList() {
    Friends_G::AddToStaticList();
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void Player_G::RemoveFromStaticList() {
    Friends_G::RemoveFromStaticList();
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

Player_G::Player_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (Player_G::Player_G)
}

void Player_G::Init() {
    Friends_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (Player_G::Init)
}

// clang-format off

BEHAVIOR(Player_G,CheckWarpColor)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_warp_color_check)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Player_G::CheckWarpColor)
END_BEHAVIOR

BEHAVIOR(Player_G,CheckSoundClothe)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_sound_clothe)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Player_G::CheckSoundClothe)
END_BEHAVIOR

BEHAVIOR(Player_G,CheckAnimEvent)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_event)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Player_G::CheckAnimEvent)
END_BEHAVIOR

BEHAVIOR(Player_G,CheckHit)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_hit_skel_collide)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Player_G::CheckHit)
END_BEHAVIOR

// clang-format on
