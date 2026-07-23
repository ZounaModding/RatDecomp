#include "VolumeDelayedStart_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(VolumeDelayedStart_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

VolumeDelayedStart_G::VolumeDelayedStart_G() {
}

void VolumeDelayedStart_G::Init() {
    VolumeAgent_Z::Init();
}
