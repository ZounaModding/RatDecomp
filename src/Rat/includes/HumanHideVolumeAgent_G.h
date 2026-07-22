#ifndef _HUMANHIDEVOLUMEAGENT_G_H_
#define _HUMANHIDEVOLUMEAGENT_G_H_
#include "DetectVolumeAgent_G.h"
// clang-format off

BEGIN_AGENT_CLASS(HumanSeenVolumeAgent_G, DetectVolumeAgent_G, 13)
public:
    HumanSeenVolumeAgent_G();

    virtual ~HumanSeenVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct HumanSeenVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _HUMANHIDEVOLUMEAGENT_G_H_
