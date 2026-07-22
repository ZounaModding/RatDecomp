#include "IT_Bumper.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_Bumper, LightVolAnimatedAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<IT_Bumper*> IT_Bumper::instances;

void IT_Bumper::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_Bumper::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

IT_Bumper::IT_Bumper() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Bumper::IT_Bumper)
}

void IT_Bumper::Init() {
    LightVolAnimatedAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_Bumper::Init)
}
