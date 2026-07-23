#include "Names.h"
#include "GangAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(LightVolAnimatedAgent_Z, AnimatedAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

LightVolAnimatedAgent_Z::LightVolAnimatedAgent_Z() {
}

void LightVolAnimatedAgent_Z::Init() {
    AnimatedAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(GangAgent_G, LightVolAnimatedAgent_Z)
    ADD_BEHAVIOR(CheckPlayerDist)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<GangAgent_G*> GangAgent_G::instances;

void GangAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void GangAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}

GangAgent_G::GangAgent_G() {
}

void GangAgent_G::Init() {
    LightVolAnimatedAgent_Z::Init();
}

// clang-format off

BEHAVIOR(GangAgent_G,CheckPlayerDist)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_end_check_player)
    ACTION
        ;
END_BEHAVIOR

// clang-format on

// clang-format off

BEGIN_INIT_AGENT_CLASS(RepulseAnimatedAgent_G, LightVolAnimatedAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<RepulseAnimatedAgent_G*> RepulseAnimatedAgent_G::instances;

void RepulseAnimatedAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void RepulseAnimatedAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}

RepulseAnimatedAgent_G::RepulseAnimatedAgent_G() {
}

void RepulseAnimatedAgent_G::Init() {
    LightVolAnimatedAgent_Z::Init();
}
