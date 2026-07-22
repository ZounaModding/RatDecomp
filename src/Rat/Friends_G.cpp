#include "Friends_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(Friends_G, Creatures_G)
    ADD_BEHAVIOR(CheckContextualSound)
END_INIT_AGENT_CLASS

// clang-format on

Friends_G::Friends_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (Friends_G::Friends_G)
}

void Friends_G::Init() {
    Creatures_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (Friends_G::Init)
}

// clang-format off

BEHAVIOR(Friends_G,CheckContextualSound)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_contextual_sound_1)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Friends_G::CheckContextualSound)
END_BEHAVIOR

// clang-format on

DynPtrArray_Z<Friends_G*> Friends_G::instances;

void Friends_G::AddToStaticList() {
    Creatures_G::AddToStaticList();
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void Friends_G::RemoveFromStaticList() {
    Creatures_G::RemoveFromStaticList();
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            instances.Minimize();
            return;
        }
    }
}
