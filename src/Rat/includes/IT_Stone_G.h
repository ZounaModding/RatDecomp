#ifndef _IT_STONE_G_H_
#define _IT_STONE_G_H_
#include "PhysicAgt_G.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Stone_G, IT_DYNBASE_G, 165)
public:
    IT_Stone_G();

    virtual ~IT_Stone_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct IT_Stone_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_STONE_G_H_
