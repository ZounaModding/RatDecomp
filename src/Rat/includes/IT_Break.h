#ifndef _IT_BREAK_H_
#define _IT_BREAK_H_
#include "Agent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Break, Agent_Z, 35)
public:
    IT_Break();

    virtual ~IT_Break() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();
    static void ResetAll();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_Break*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_Break` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_BREAK_H_
