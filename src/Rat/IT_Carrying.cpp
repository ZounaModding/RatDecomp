#include "Names.h"
#include "IT_Carrying.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_Carrying, Throwable_Z)
    ADD_BEHAVIOR(CheckRespawn)
    ADD_BEHAVIOR(EnterTeleportZone)
END_INIT_AGENT_CLASS

// clang-format on

void IT_Carrying::Init() {
    Throwable_Z::Init();
}

DynPtrArray_Z<IT_Carrying*> IT_Carrying::instances;

void IT_Carrying::ResetAll() {
}

// TODO: Finish matching
void IT_Carrying::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_Carrying::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

// clang-format off

BEHAVIOR(IT_Carrying,CheckRespawn)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_check_respawn_specific)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(IT_Carrying,EnterTeleportZone)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        result_FALSE /* TODO: message not found in ABC_Message_Engine.h / ABC_Message_Game.h cross-refs - check decomp_dump/Rat/*.cpp.ghidra.c (IT_Carrying::EnterTeleportZone) for `ABC_Agent::Message(this, N)` */
    ACTION
        ;
END_BEHAVIOR

// clang-format on

IT_Carrying::IT_Carrying() {
}
