#include "ConditionVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(ConditionVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

ConditionVolumeAgent_G::ConditionVolumeAgent_G() {
}

void ConditionVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
}

DynPtrArray_Z<ConditionVolumeAgent_G*> ConditionVolumeAgent_G::STC_Instances;

void ConditionVolumeAgent_G::AddToStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            return;
        }
    }
    STC_Instances.Add(this);
}

void ConditionVolumeAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            STC_Instances.Remove(i);
            if (STC_Instances.GetSize() == 0)
                STC_Instances.Minimize();
            return;
        }
    }
}
