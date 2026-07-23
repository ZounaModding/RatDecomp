#ifndef _FLYUPVOLUMEAGENT_G_H_
#define _FLYUPVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(FlyUpVolumeAgent_G, VolumeAgent_Z, 12)
public:
    FlyUpVolumeAgent_G();

    virtual ~FlyUpVolumeAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<FlyUpVolumeAgent_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _FLYUPVOLUMEAGENT_G_H_
