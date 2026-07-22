#include "CheckpointVolAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(CheckpointVolAgent_G, VolumeDelayedStart_G)
END_INIT_AGENT_CLASS

// clang-format on

CheckpointVolAgent_G::CheckpointVolAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (CheckpointVolAgent_G::CheckpointVolAgent_G)
}

void CheckpointVolAgent_G::Init() {
    VolumeDelayedStart_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (CheckpointVolAgent_G::Init)
}

DynPtrArray_Z<CheckpointVolAgent_G*> CheckpointVolAgent_G::STC_Instances;

void CheckpointVolAgent_G::AddToStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            return;
        }
    }
    STC_Instances.Add(this);
}

void CheckpointVolAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            STC_Instances.Remove(i);
            if (STC_Instances.GetSize() == 0)
                STC_Instances.Minimize();
            return;
        }
    }
}
