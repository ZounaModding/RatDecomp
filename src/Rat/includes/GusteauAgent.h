#ifndef _GUSTEAUAGENT_H_
#define _GUSTEAUAGENT_H_
#include "MovingAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(GusteauAgent, MovingAgent_Z, 8)
public:
    GusteauAgent();

    virtual ~GusteauAgent() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct GusteauAgent` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _GUSTEAUAGENT_H_
