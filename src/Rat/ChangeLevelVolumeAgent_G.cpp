#include "ChangeLevelVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(ChangeLevelVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

ChangeLevelVolumeAgent_G::ChangeLevelVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (ChangeLevelVolumeAgent_G::ChangeLevelVolumeAgent_G)
}

void ChangeLevelVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (ChangeLevelVolumeAgent_G::Init)
}

DynPtrArray_Z<ChangeLevelVolumeAgent_G*> ChangeLevelVolumeAgent_G::instances;

void ChangeLevelVolumeAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void ChangeLevelVolumeAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}
