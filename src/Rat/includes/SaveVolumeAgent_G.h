#ifndef _SAVEVOLUMEAGENT_G_H_
#define _SAVEVOLUMEAGENT_G_H_
#include "VolumeDelayedStart_G.h"
// clang-format off

BEGIN_AGENT_CLASS(SaveVolumeAgent_G, VolumeDelayedStart_G, 12)
public:
    SaveVolumeAgent_G();

    virtual ~SaveVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct SaveVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _SAVEVOLUMEAGENT_G_H_
