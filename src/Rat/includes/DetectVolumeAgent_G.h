#ifndef _DETECTVOLUMEAGENT_G_H_
#define _DETECTVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(DetectVolumeAgent_G, VolumeAgent_Z, 10)
public:
    DetectVolumeAgent_G();

    virtual ~DetectVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct DetectVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _DETECTVOLUMEAGENT_G_H_
