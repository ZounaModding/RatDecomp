#ifndef _GUARDAGENT_G_H_
#define _GUARDAGENT_G_H_
#include "AnimatedAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(GuardAgent_G, AnimatedAgent_Z, 14)
public:
    GuardAgent_G();

    virtual ~GuardAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckSight);
    DECL_BHV(CheckBlind);

private:
    static DynPtrArray_Z<GuardAgent_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct GuardAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _GUARDAGENT_G_H_
