#include "MissionVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(MissionVolumeAgent_G, VolumeDelayedStart_G)
END_INIT_AGENT_CLASS

// clang-format on

MissionVolumeAgent_G::MissionVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (MissionVolumeAgent_G::MissionVolumeAgent_G)
}

void MissionVolumeAgent_G::Init() {
    VolumeDelayedStart_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (MissionVolumeAgent_G::Init)
}

DynPtrArray_Z<MissionVolumeAgent_G*> MissionVolumeAgent_G::instances;

void MissionVolumeAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void MissionVolumeAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}

MissionVolumeAgent_G* MissionVolumeAgent_G::CheckAnims() {
    ; // TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(MissionVolumeAgent_G::CheckAnims)
    return NULL;
}
