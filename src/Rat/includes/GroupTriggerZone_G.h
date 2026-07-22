#ifndef _GROUPTRIGGERZONE_G_H_
#define _GROUPTRIGGERZONE_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(GroupWanderingZone_G, VolumeAgent_Z, 33)
public:
    GroupWanderingZone_G();

    virtual ~GroupWanderingZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct GroupWanderingZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(FleeCollisionVolumeAgent_G, VolumeAgent_Z, 42)
public:
    FleeCollisionVolumeAgent_G();

    virtual ~FleeCollisionVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct FleeCollisionVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(GroupTriggerZone_G, VolumeAgent_Z, 66)
public:
    GroupTriggerZone_G();

    virtual ~GroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct GroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(HoveringGroupTriggerZone_G, GroupTriggerZone_G, 124)
public:
    HoveringGroupTriggerZone_G();

    virtual ~HoveringGroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct HoveringGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(SwarmGroupTriggerZone_G, GroupTriggerZone_G, 137)
public:
    SwarmGroupTriggerZone_G();

    virtual ~SwarmGroupTriggerZone_G() {}
    virtual void Init();

    DECL_BHV(CheckPlayerDist);

private:
    // TODO: fields - see decomp_dump/types.h `struct SwarmGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(SwarmClamsGroupTriggerZone_G, SwarmGroupTriggerZone_G, 161)
public:
    SwarmClamsGroupTriggerZone_G();

    virtual ~SwarmClamsGroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct SwarmClamsGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(AntSwarmGroupTriggerZone_G, SwarmGroupTriggerZone_G, 169)
public:
    AntSwarmGroupTriggerZone_G();

    virtual ~AntSwarmGroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct AntSwarmGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(SwarmMeanGroupTriggerZone_G, SwarmGroupTriggerZone_G, 189)
public:
    SwarmMeanGroupTriggerZone_G();

    virtual ~SwarmMeanGroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct SwarmMeanGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(PigeonGroupTriggerZone_G, SwarmGroupTriggerZone_G, 215)
public:
    PigeonGroupTriggerZone_G();

    virtual ~PigeonGroupTriggerZone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct PigeonGroupTriggerZone_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _GROUPTRIGGERZONE_G_H_
