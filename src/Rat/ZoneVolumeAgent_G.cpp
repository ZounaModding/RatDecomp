#include "ZoneVolumeAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(CameraFxVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

CameraFxVolumeAgent_G::CameraFxVolumeAgent_G() {
}

void CameraFxVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(WakeUpDynVolAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

WakeUpDynVolAgent_G::WakeUpDynVolAgent_G() {
}

void WakeUpDynVolAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(FallVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

FallVolumeAgent_G::FallVolumeAgent_G() {
}

void FallVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(SASVolumeAgent_G, VolumeAgent_Z)
    ADD_BEHAVIOR(CheckSwitch)
END_INIT_AGENT_CLASS

// clang-format on

SASVolumeAgent_G::SASVolumeAgent_G() {
}

void SASVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEHAVIOR(SASVolumeAgent_G,CheckSwitch)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_change)
    ACTION
        ;
END_BEHAVIOR

// clang-format on

// clang-format off

BEGIN_INIT_AGENT_CLASS(RaftMoveJumpAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

RaftMoveJumpAgent_G::RaftMoveJumpAgent_G() {
}

void RaftMoveJumpAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(RaftImmobileAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

RaftImmobileAgent_G::RaftImmobileAgent_G() {
}

void RaftImmobileAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(PlatformCameraAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

PlatformCameraAgent_G::PlatformCameraAgent_G() {
}

void PlatformCameraAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(BehindCameraAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

BehindCameraAgent_G::BehindCameraAgent_G() {
}

void BehindCameraAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(RaftHideAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

RaftHideAgent_G::RaftHideAgent_G() {
}

void RaftHideAgent_G::Init() {
    VolumeAgent_Z::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(RaftTourbillonAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

RaftTourbillonAgent_G::RaftTourbillonAgent_G() {
}

void RaftTourbillonAgent_G::Init() {
    VolumeAgent_Z::Init();
}
