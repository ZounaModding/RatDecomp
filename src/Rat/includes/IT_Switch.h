#ifndef _IT_SWITCH_H_
#define _IT_SWITCH_H_
#include "AnimatedAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Switch, AnimatedAgent_Z, 11)
public:
    IT_Switch();

    virtual ~IT_Switch() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_Switch*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_Switch` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_SWITCH_H_
