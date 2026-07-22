#include "CameraAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(CameraAgent_G, CameraAgent_Z)
    ADD_BEHAVIOR(CameraAgentQuartier)
END_INIT_AGENT_CLASS

// clang-format on

CameraAgent_G::CameraAgent_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (CameraAgent_G::CameraAgent_G)
}

void CameraAgent_G::Init() {
    CameraAgent_Z::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (CameraAgent_G::Init)
}

// clang-format off

BEHAVIOR(CameraAgent_G,CameraAgentQuartier)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_framelink)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(CameraAgent_G::CameraAgentQuartier)
END_BEHAVIOR

// clang-format on
