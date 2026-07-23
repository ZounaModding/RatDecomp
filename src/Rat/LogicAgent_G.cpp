#include "LogicAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(LogicAgent_G, Agent_Z)
    ADD_BEHAVIOR(ActivateGame)
    ADD_BEHAVIOR(BlockedFaderBhv)
END_INIT_AGENT_CLASS

// clang-format on

LogicAgent_G::LogicAgent_G() {
}

void LogicAgent_G::Init() {
    Agent_Z::Init();
}

// clang-format off

BEHAVIOR(LogicAgent_G,ActivateGame)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        result_FALSE /* TODO: message not found in ABC_Message_Engine.h / ABC_Message_Game.h cross-refs - check decomp_dump/Rat/*.cpp.ghidra.c (LogicAgent_G::ActivateGame) for `ABC_Agent::Message(this, N)` */
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(LogicAgent_G,BlockedFaderBhv)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_block_fader)
    ACTION
        ;
END_BEHAVIOR

// clang-format on
