#include "Names.h"
#include "GuardAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(GuardAgent_G, AnimatedAgent_Z)
    ADD_BEHAVIOR(CheckSight)
    ADD_BEHAVIOR(CheckBlind)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<GuardAgent_G*> GuardAgent_G::instances;

void GuardAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void GuardAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}

GuardAgent_G::GuardAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (GuardAgent_G::GuardAgent_G)
}

void GuardAgent_G::Init() {
    AnimatedAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (GuardAgent_G::Init)
}

// clang-format off

BEHAVIOR(GuardAgent_G,CheckSight)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_end_check_player)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(GuardAgent_G::CheckSight)
END_BEHAVIOR

BEHAVIOR(GuardAgent_G,CheckBlind)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_start_blind)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(GuardAgent_G::CheckBlind)
END_BEHAVIOR

// clang-format on
