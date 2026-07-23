#include "SaveVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(SaveVolumeAgent_G, VolumeDelayedStart_G)
END_INIT_AGENT_CLASS

// clang-format on

SaveVolumeAgent_G::SaveVolumeAgent_G() {
}

void SaveVolumeAgent_G::Init() {
    VolumeDelayedStart_G::Init();
}
