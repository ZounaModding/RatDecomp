#ifndef _LIGHTVOLUMEAGENT_G_H_
#define _LIGHTVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(LightVolumeAgent_G, VolumeAgent_Z, 13)
public:
    LightVolumeAgent_G();

    virtual ~LightVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct LightVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _LIGHTVOLUMEAGENT_G_H_
