#ifndef _CHECKPOINTVOLAGENT_G_H_
#define _CHECKPOINTVOLAGENT_G_H_
#include "VolumeDelayedStart_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(CheckpointVolAgent_G, VolumeDelayedStart_G, 14)
public:
    CheckpointVolAgent_G();

    virtual ~CheckpointVolAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<CheckpointVolAgent_G*> STC_Instances;
END_AGENT_CLASS

// clang-format on
#endif // _CHECKPOINTVOLAGENT_G_H_
