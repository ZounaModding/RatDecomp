#include "IT_BallWalk_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_BallWalk_G, MovingAgent_Z)
    ADD_BEHAVIOR(CheckRespawn)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<IT_BallWalk_G*> IT_BallWalk_G::instances;

void IT_BallWalk_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_BallWalk_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
}

IT_BallWalk_G::~IT_BallWalk_G() {
    RemoveFromStaticList();
}

IT_BallWalk_G::IT_BallWalk_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_BallWalk_G::IT_BallWalk_G)
}

void IT_BallWalk_G::Init() {
    MovingAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_BallWalk_G::Init)
}

// clang-format off

BEHAVIOR(IT_BallWalk_G,CheckRespawn)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_respawn)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(IT_BallWalk_G::CheckRespawn)
END_BEHAVIOR

// clang-format on
