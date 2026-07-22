#include "LightVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(LightVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

LightVolumeAgent_G::LightVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (LightVolumeAgent_G::LightVolumeAgent_G)
}

void LightVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (LightVolumeAgent_G::Init)
}
