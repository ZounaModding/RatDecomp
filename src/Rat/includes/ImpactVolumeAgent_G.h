#ifndef _IMPACTVOLUMEAGENT_G_H_
#define _IMPACTVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(ImpactVolumeAgent_G, VolumeAgent_Z, 12)
public:
    ImpactVolumeAgent_G();

    virtual ~ImpactVolumeAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<ImpactVolumeAgent_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct ImpactVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IMPACTVOLUMEAGENT_G_H_
