#ifndef _GROUPAGENT_G_H_
#define _GROUPAGENT_G_H_
#include "BaseAgent_G.h"
// clang-format off

BEGIN_AGENT_CLASS(GroupAgent_G, BaseAgent_G, 16)
public:
    GroupAgent_G();

    virtual ~GroupAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct GroupAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _GROUPAGENT_G_H_
