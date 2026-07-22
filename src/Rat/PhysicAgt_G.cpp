#include "PhysicAgt_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_DYNBASE_G, BaseAgent_G)
END_INIT_AGENT_CLASS

// clang-format on

IT_DYNBASE_G::IT_DYNBASE_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_DYNBASE_G::IT_DYNBASE_G)
}

DynPtrArray_Z<IT_DYNBASE_G*> IT_DYNBASE_G::instances;

void IT_DYNBASE_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

// TODO: Finish matching
void IT_DYNBASE_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}
