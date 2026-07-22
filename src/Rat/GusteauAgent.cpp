#include "GusteauAgent.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(GusteauAgent, MovingAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

GusteauAgent::GusteauAgent() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (GusteauAgent::GusteauAgent)
}

void GusteauAgent::Init() {
    MovingAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (GusteauAgent::Init)
}
