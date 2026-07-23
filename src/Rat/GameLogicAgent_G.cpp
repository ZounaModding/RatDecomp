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
}

void GameLogicAgent_G::Init() {
    LogicAgent_G::Init();
}

// clang-format off

BEHAVIOR(GameLogicAgent_G,ActivateGame)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_activated)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(GameLogicAgent_G,GameReseted)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_reseted)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(GameLogicAgent_G,StopPauseGameBhv)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_to_menu)
    ACTION
        ;
END_BEHAVIOR

// clang-format on
