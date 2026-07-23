#include "HumanHideVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(HumanSeenVolumeAgent_G, DetectVolumeAgent_G)
END_INIT_AGENT_CLASS

// clang-format on

HumanSeenVolumeAgent_G::HumanSeenVolumeAgent_G() {
}

void HumanSeenVolumeAgent_G::Init() {
    DetectVolumeAgent_G::Init();
}
