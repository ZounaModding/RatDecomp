#include "Names.h"
#include "IT_Break.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_Break, Agent_Z)
END_INIT_AGENT_CLASS

// clang-format on

IT_Break::IT_Break() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Break::IT_Break)
}

void IT_Break::Init() {
    Agent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Break::Init)
}

DynPtrArray_Z<IT_Break*> IT_Break::instances;

// TODO: Finish matching
void IT_Break::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_Break::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}
