#ifndef _HUMANHIDEVOLUMEAGENT_G_H_
#define _HUMANHIDEVOLUMEAGENT_G_H_
#include "DetectVolumeAgent_G.h"
// clang-format off

BEGIN_AGENT_CLASS(HumanSeenVolumeAgent_G, DetectVolumeAgent_G, 13)
public:
    HumanSeenVolumeAgent_G();

    virtual ~HumanSeenVolumeAgent_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _HUMANHIDEVOLUMEAGENT_G_H_
