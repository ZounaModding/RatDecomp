#include "IT_Fish_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_FISH_G, MovingAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

IT_FISH_G::IT_FISH_G() {
}

void IT_FISH_G::Init() {
    MovingAgent_Z::Init();
}

DynPtrArray_Z<IT_FISH_G*> IT_FISH_G::instances;

void IT_FISH_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_FISH_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}
