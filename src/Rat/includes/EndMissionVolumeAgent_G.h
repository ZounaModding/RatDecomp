#ifndef _ENDMISSIONVOLUMEAGENT_G_H_
#define _ENDMISSIONVOLUMEAGENT_G_H_
#include "VolumeDelayedStart_G.h"
// clang-format off

BEGIN_AGENT_CLASS(EndMissionVolumeAgent_G, VolumeDelayedStart_G, 12)
public:
    EndMissionVolumeAgent_G();

    virtual ~EndMissionVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct EndMissionVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _ENDMISSIONVOLUMEAGENT_G_H_
