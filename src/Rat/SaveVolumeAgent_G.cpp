#include "SaveVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(SaveVolumeAgent_G, VolumeDelayedStart_G)
END_INIT_AGENT_CLASS

// clang-format on

SaveVolumeAgent_G::SaveVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (SaveVolumeAgent_G::SaveVolumeAgent_G)
}

void SaveVolumeAgent_G::Init() {
    VolumeDelayedStart_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (SaveVolumeAgent_G::Init)
}
