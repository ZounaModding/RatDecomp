#include "ImpactVolumeAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(ImpactVolumeAgent_G, VolumeAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

DynPtrArray_Z<ImpactVolumeAgent_G*> ImpactVolumeAgent_G::instances;

void ImpactVolumeAgent_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void ImpactVolumeAgent_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            if (instances.GetSize() == 0)
                instances.Minimize();
            return;
        }
    }
}

ImpactVolumeAgent_G::ImpactVolumeAgent_G() {
}

void ImpactVolumeAgent_G::Init() {
    VolumeAgent_Z::Init();
}
