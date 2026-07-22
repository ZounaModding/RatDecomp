#ifndef _COLLECTAGENT_G_H_
#define _COLLECTAGENT_G_H_
#include "ObjectAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_COLLECT_G, ObjectAgent_G, 15)
public:
    IT_COLLECT_G();

    virtual ~IT_COLLECT_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_COLLECT_G*> STC_Instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_COLLECT_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _COLLECTAGENT_G_H_
