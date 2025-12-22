#include "CameraAgent_Z.h"
#include "ABC_Message_Engine.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(CameraAgent_Z, Agent_Z)
    ADD_BEHAVIOR(CameraAgentSuspend)
END_INIT_AGENT_CLASS

BEHAVIOR(CameraAgent_Z,CameraAgentSuspend)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_started)
    ACTION
        if((S32)m_MessageParameter!=GetViewport())
        {
            return FALSE;
        }
        ABC_Agent* l_Agent = (ABC_Agent*)GETPTR(m_Sender);
        if (l_Agent->HasFlag(0x1000000)) {
            return FALSE;
        }
        Suspend();
END_BEHAVIOR

// clang-format on
