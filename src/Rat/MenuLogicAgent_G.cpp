#include "MenuLogicAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(MenuLogicAgent_G, LogicAgent_G)
    ADD_BEHAVIOR(ActivateGame)
    ADD_BEHAVIOR(ReallyStartMenu)
END_INIT_AGENT_CLASS

// clang-format on

MenuLogicAgent_G::MenuLogicAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (MenuLogicAgent_G::MenuLogicAgent_G)
}

void MenuLogicAgent_G::Init() {
    LogicAgent_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (MenuLogicAgent_G::Init)
}

// clang-format off

BEHAVIOR(MenuLogicAgent_G,ActivateGame)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_game_activated)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(MenuLogicAgent_G::ActivateGame)
END_BEHAVIOR

BEHAVIOR(MenuLogicAgent_G,ReallyStartMenu)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_start_menu)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(MenuLogicAgent_G::ReallyStartMenu)
END_BEHAVIOR

// clang-format on
