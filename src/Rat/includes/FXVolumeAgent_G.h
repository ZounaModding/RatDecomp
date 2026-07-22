#ifndef _FXVOLUMEAGENT_G_H_
#define _FXVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(FXVolumeAgent_G, VolumeAgent_Z, 16)
public:
    FXVolumeAgent_G();

    virtual ~FXVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct FXVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _FXVOLUMEAGENT_G_H_
