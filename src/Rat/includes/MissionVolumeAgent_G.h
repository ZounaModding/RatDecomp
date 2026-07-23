#ifndef _MISSIONVOLUMEAGENT_G_H_
#define _MISSIONVOLUMEAGENT_G_H_
#include "VolumeDelayedStart_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(MissionVolumeAgent_G, VolumeDelayedStart_G, 59)
public:
    MissionVolumeAgent_G();

    virtual ~MissionVolumeAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();
    MissionVolumeAgent_G * CheckAnims();

private:
    static DynPtrArray_Z<MissionVolumeAgent_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _MISSIONVOLUMEAGENT_G_H_
