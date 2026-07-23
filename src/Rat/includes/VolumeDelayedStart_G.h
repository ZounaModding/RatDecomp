#ifndef _VOLUMEDELAYEDSTART_G_H_
#define _VOLUMEDELAYEDSTART_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(VolumeDelayedStart_G, VolumeAgent_Z, 14)
public:
    VolumeDelayedStart_G();

    virtual ~VolumeDelayedStart_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _VOLUMEDELAYEDSTART_G_H_
