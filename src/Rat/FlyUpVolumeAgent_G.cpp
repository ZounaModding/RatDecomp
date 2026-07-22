#include "FlyUpVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(FlyUpVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

FlyUpVolumeAgent_G::FlyUpVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (FlyUpVolumeAgent_G::FlyUpVolumeAgent_G)
}

void FlyUpVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (FlyUpVolumeAgent_G::Init)
}

DynPtrArray_Z<FlyUpVolumeAgent_G*> FlyUpVolumeAgent_G::instances;

void FlyUpVolumeAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void FlyUpVolumeAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}
