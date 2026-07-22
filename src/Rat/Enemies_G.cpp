#include "Names.h"
#include "Enemies_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(Enemies_G, Friends_G)
    ADD_BEHAVIOR(CheckEvent)
END_INIT_AGENT_CLASS

// clang-format on

Enemies_G::Enemies_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (Enemies_G::Enemies_G)
}

void Enemies_G::Init() {
    Friends_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (Enemies_G::Init)
}

// clang-format off

BEHAVIOR(Enemies_G,CheckEvent)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_event)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Enemies_G::CheckEvent)
END_BEHAVIOR

// clang-format on
