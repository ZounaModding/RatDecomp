#include "EndMissionVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(EndMissionVolumeAgent_G, VolumeDelayedStart_G)
END_INIT_AGENT_CLASS

// clang-format on

EndMissionVolumeAgent_G::EndMissionVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (EndMissionVolumeAgent_G::EndMissionVolumeAgent_G)
}

void EndMissionVolumeAgent_G::Init() {
    VolumeDelayedStart_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (EndMissionVolumeAgent_G::Init)
}
