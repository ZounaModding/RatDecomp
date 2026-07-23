#ifndef _CONDITIONVOLUMEAGENT_G_H_
#define _CONDITIONVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(ConditionVolumeAgent_G, VolumeAgent_Z, 13)
public:
    ConditionVolumeAgent_G();

    virtual ~ConditionVolumeAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<ConditionVolumeAgent_G*> STC_Instances;
END_AGENT_CLASS

// clang-format on
#endif // _CONDITIONVOLUMEAGENT_G_H_
