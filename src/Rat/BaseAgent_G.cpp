#include "BaseAgent_G.h"
#include "ABC_Message_Game.h"
#include "ObjectMove_Z.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(BaseAgent_G, LodAgent_Z)
    ADD_BEHAVIOR(CheckReset)
END_INIT_AGENT_CLASS

// clang-format on

BaseAgent_G::BaseAgent_G() {
    m_TypeFlags |= FL_AGENT_TYPE_BASE;
}

void BaseAgent_G::Init() {
    LodAgent_Z::Init();
    ActivateBhv(CheckReset);
}

// clang-format off

BEHAVIOR(BaseAgent_G,CheckReset)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_reset_base_agent)
    ACTION
        Reset();
        m_ObjectMoveHdl->Reset();
END_BEHAVIOR

// clang-format on
