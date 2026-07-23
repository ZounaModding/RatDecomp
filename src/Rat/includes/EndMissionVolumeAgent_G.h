#ifndef _ENDMISSIONVOLUMEAGENT_G_H_
#define _ENDMISSIONVOLUMEAGENT_G_H_
#include "VolumeDelayedStart_G.h"
// clang-format off

BEGIN_AGENT_CLASS(EndMissionVolumeAgent_G, VolumeDelayedStart_G, 12)
public:
    EndMissionVolumeAgent_G();

    virtual ~EndMissionVolumeAgent_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _ENDMISSIONVOLUMEAGENT_G_H_
