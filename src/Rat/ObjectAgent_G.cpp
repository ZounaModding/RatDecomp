#include "ObjectAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(ObjectAgent_G, BaseAgent_G)
END_INIT_AGENT_CLASS

// clang-format on

ObjectAgent_G::ObjectAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (ObjectAgent_G::ObjectAgent_G)
}

void ObjectAgent_G::Init() {
    BaseAgent_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (ObjectAgent_G::Init)
}

DynPtrArray_Z<IT_BREAKBASE_G*> IT_BREAKBASE_G::instances;

void IT_BREAKBASE_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

// TODO: Finish matching
void IT_BREAKBASE_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

IT_BREAKBASE_G::IT_BREAKBASE_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_BREAKBASE_G::IT_BREAKBASE_G)
}

void IT_BREAKBASE_G::Init() {
    Agent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_BREAKBASE_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_BREAKBASE_G, Agent_Z)
    ADD_BEHAVIOR(FireInside)
    ADD_BEHAVIOR(GenerateObject)
END_INIT_AGENT_CLASS

// clang-format on

// clang-format off

BEHAVIOR(IT_BREAKBASE_G,FireInside)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_break)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(IT_BREAKBASE_G::FireInside)
END_BEHAVIOR

BEHAVIOR(IT_BREAKBASE_G,GenerateObject)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_generate_object)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(IT_BREAKBASE_G::GenerateObject)
END_BEHAVIOR

// clang-format on

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_TIMED_PLATFORM_G, AnimatedAgent_Z)
    ADD_BEHAVIOR(CheckPlayer)
    ADD_BEHAVIOR(CheckRespawn)
END_INIT_AGENT_CLASS

// clang-format on

IT_TIMED_PLATFORM_G::IT_TIMED_PLATFORM_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_TIMED_PLATFORM_G::IT_TIMED_PLATFORM_G)
}

void IT_TIMED_PLATFORM_G::Init() {
    AnimatedAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_TIMED_PLATFORM_G::Init)
}

// clang-format off

BEHAVIOR(IT_TIMED_PLATFORM_G,CheckPlayer)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_collision_player_in)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(IT_TIMED_PLATFORM_G::CheckPlayer)
END_BEHAVIOR

BEHAVIOR(IT_TIMED_PLATFORM_G,CheckRespawn)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_start_check_respawn)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(IT_TIMED_PLATFORM_G::CheckRespawn)
END_BEHAVIOR

// clang-format on

// clang-format off

BEGIN_INIT_AGENT_CLASS(PLTF02_GROUP, AnimatedAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

PLTF02_GROUP::PLTF02_GROUP() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (PLTF02_GROUP::PLTF02_GROUP)
}

void PLTF02_GROUP::Init() {
    AnimatedAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (PLTF02_GROUP::Init)
}
