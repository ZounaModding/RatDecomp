#ifndef _NODEVOLUMEAGENT_G_H_
#define _NODEVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(NodeVolumeAgent_G, VolumeAgent_Z, 14)
public:
    NodeVolumeAgent_G();

    virtual ~NodeVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct NodeVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _NODEVOLUMEAGENT_G_H_
