#include "Names.h"
#include "IT_Switch.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_Switch, AnimatedAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<IT_Switch*> IT_Switch::instances;

void IT_Switch::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_Switch::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

IT_Switch::IT_Switch() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Switch::IT_Switch)
}

void IT_Switch::Init() {
    AnimatedAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Switch::Init)
}
