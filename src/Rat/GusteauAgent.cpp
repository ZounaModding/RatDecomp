#include "GusteauAgent.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(GusteauAgent, MovingAgent_Z)
END_INIT_AGENT_CLASS

// clang-format on

GusteauAgent::GusteauAgent() {
}

void GusteauAgent::Init() {
    MovingAgent_Z::Init();
}
