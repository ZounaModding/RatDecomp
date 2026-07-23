#include "MenuLogicAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(MenuLogicAgent_G, LogicAgent_G)
    ADD_BEHAVIOR(ActivateGame)
    ADD_BEHAVIOR(ReallyStartMenu)
END_INIT_AGENT_CLASS

// clang-format on

MenuLogicAgent_G::MenuLogicAgent_G() {
}

void MenuLogicAgent_G::Init() {
    LogicAgent_G::Init();
}

// clang-format off

BEHAVIOR(MenuLogicAgent_G,ActivateGame)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_activated)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(MenuLogicAgent_G,ReallyStartMenu)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_start_menu)
    ACTION
        ;
END_BEHAVIOR

// clang-format on
