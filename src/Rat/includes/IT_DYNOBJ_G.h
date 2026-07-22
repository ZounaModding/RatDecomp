#ifndef _IT_DYNOBJ_G_H_
#define _IT_DYNOBJ_G_H_
#include "PhysicAgt_G.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_DYNOBJ_G, IT_DYNBASE_G, 12)
public:
    IT_DYNOBJ_G();

    virtual ~IT_DYNOBJ_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct IT_DYNOBJ_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_DYNOBJ_G_H_
