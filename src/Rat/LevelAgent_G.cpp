#include "LevelAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(LevelAgent_G, Agent_Z)
    ADD_BEHAVIOR(BhvToLevel)
    ADD_BEHAVIOR(BhvChangeLevel)
    ADD_BEHAVIOR(BhvToMenu)
END_INIT_AGENT_CLASS

// clang-format on

LevelAgent_G::LevelAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (LevelAgent_G::LevelAgent_G)
}

void LevelAgent_G::Init() {
    Agent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (LevelAgent_G::Init)
}

// clang-format off

BEHAVIOR(LevelAgent_G,BhvToLevel)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_to_level)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(LevelAgent_G::BhvToLevel)
END_BEHAVIOR

BEHAVIOR(LevelAgent_G,BhvChangeLevel)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_change_level)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(LevelAgent_G::BhvChangeLevel)
END_BEHAVIOR

BEHAVIOR(LevelAgent_G,BhvToMenu)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_to_menu)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(LevelAgent_G::BhvToMenu)
END_BEHAVIOR

// clang-format on
