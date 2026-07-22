#ifndef _IT_BALL_G_H_
#define _IT_BALL_G_H_
#include "PhysicAgt_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_BALL, IT_DYNBASE_G, 16)
public:
    IT_BALL();

    virtual ~IT_BALL() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_BALL*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_BALL` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_BALL_G_H_
