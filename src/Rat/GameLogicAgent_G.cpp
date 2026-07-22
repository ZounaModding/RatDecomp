#include "GameLogicAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(GameLogicAgent_G, LogicAgent_G)
    ADD_BEHAVIOR(ActivateGame)
    ADD_BEHAVIOR(GameReseted)
    ADD_BEHAVIOR(StopPauseGameBhv)
END_INIT_AGENT_CLASS

// clang-format on

GameLogicAgent_G::GameLogicAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (GameLogicAgent_G::GameLogicAgent_G)
}

void GameLogicAgent_G::Init() {
    LogicAgent_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (GameLogicAgent_G::Init)
}

// clang-format off

BEHAVIOR(GameLogicAgent_G,ActivateGame)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_activated)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(GameLogicAgent_G::ActivateGame)
END_BEHAVIOR

BEHAVIOR(GameLogicAgent_G,GameReseted)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_reseted)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(GameLogicAgent_G::GameReseted)
END_BEHAVIOR

BEHAVIOR(GameLogicAgent_G,StopPauseGameBhv)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_to_menu)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(GameLogicAgent_G::StopPauseGameBhv)
END_BEHAVIOR

// clang-format on
