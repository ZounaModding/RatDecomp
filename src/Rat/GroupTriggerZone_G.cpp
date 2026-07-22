#include "GroupTriggerZone_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(GroupWanderingZone_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

GroupWanderingZone_G::GroupWanderingZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (GroupWanderingZone_G::GroupWanderingZone_G)
}

void GroupWanderingZone_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (GroupWanderingZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(FleeCollisionVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

FleeCollisionVolumeAgent_G::FleeCollisionVolumeAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (FleeCollisionVolumeAgent_G::FleeCollisionVolumeAgent_G)
}

void FleeCollisionVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (FleeCollisionVolumeAgent_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(GroupTriggerZone_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

GroupTriggerZone_G::GroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (GroupTriggerZone_G::GroupTriggerZone_G)
}

void GroupTriggerZone_G::Init() {
    VolumeAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (GroupTriggerZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(HoveringGroupTriggerZone_G, GroupTriggerZone_G)
END_INIT_AGENT_CLASS

// clang-format on

HoveringGroupTriggerZone_G::HoveringGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (HoveringGroupTriggerZone_G::HoveringGroupTriggerZone_G)
}

void HoveringGroupTriggerZone_G::Init() {
    GroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (HoveringGroupTriggerZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(SwarmGroupTriggerZone_G, GroupTriggerZone_G)
    ADD_BEHAVIOR(CheckPlayerDist)
END_INIT_AGENT_CLASS

// clang-format on

SwarmGroupTriggerZone_G::SwarmGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmGroupTriggerZone_G::SwarmGroupTriggerZone_G)
}

void SwarmGroupTriggerZone_G::Init() {
    GroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmGroupTriggerZone_G::Init)
}

// clang-format off

BEHAVIOR(SwarmGroupTriggerZone_G,CheckPlayerDist)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_end_check_player)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(SwarmGroupTriggerZone_G::CheckPlayerDist)
END_BEHAVIOR

// clang-format on

// clang-format off

BEGIN_INIT_AGENT_CLASS(SwarmClamsGroupTriggerZone_G, SwarmGroupTriggerZone_G)
END_INIT_AGENT_CLASS

// clang-format on

SwarmClamsGroupTriggerZone_G::SwarmClamsGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmClamsGroupTriggerZone_G::SwarmClamsGroupTriggerZone_G)
}

void SwarmClamsGroupTriggerZone_G::Init() {
    SwarmGroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmClamsGroupTriggerZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(AntSwarmGroupTriggerZone_G, SwarmGroupTriggerZone_G)
END_INIT_AGENT_CLASS

// clang-format on

AntSwarmGroupTriggerZone_G::AntSwarmGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (AntSwarmGroupTriggerZone_G::AntSwarmGroupTriggerZone_G)
}

void AntSwarmGroupTriggerZone_G::Init() {
    SwarmGroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (AntSwarmGroupTriggerZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(SwarmMeanGroupTriggerZone_G, SwarmGroupTriggerZone_G)
END_INIT_AGENT_CLASS

// clang-format on

SwarmMeanGroupTriggerZone_G::SwarmMeanGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmMeanGroupTriggerZone_G::SwarmMeanGroupTriggerZone_G)
}

void SwarmMeanGroupTriggerZone_G::Init() {
    SwarmGroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (SwarmMeanGroupTriggerZone_G::Init)
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(PigeonGroupTriggerZone_G, SwarmGroupTriggerZone_G)
END_INIT_AGENT_CLASS

// clang-format on

PigeonGroupTriggerZone_G::PigeonGroupTriggerZone_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (PigeonGroupTriggerZone_G::PigeonGroupTriggerZone_G)
}

void PigeonGroupTriggerZone_G::Init() {
    SwarmGroupTriggerZone_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (PigeonGroupTriggerZone_G::Init)
}
