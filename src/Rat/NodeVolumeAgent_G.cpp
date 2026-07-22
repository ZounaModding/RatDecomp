#include "NodeVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(NodeVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

NodeVolumeAgent_G::NodeVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (NodeVolumeAgent_G::NodeVolumeAgent_G)
}

void NodeVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (NodeVolumeAgent_G::Init)
}
