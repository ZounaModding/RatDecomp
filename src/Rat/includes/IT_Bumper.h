#ifndef _IT_BUMPER_H_
#define _IT_BUMPER_H_
#include "GangAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Bumper, LightVolAnimatedAgent_Z, 17)
public:
    IT_Bumper();

    virtual ~IT_Bumper() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_Bumper*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_Bumper` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_BUMPER_H_
