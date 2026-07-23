#include "CameraAgent_Z.h"
#include "ABC_Message_Engine.h"
#include "Renderer_Z.h"

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
        if (l_Agent->HasFlag(FL_AGENT_CAMERA_NO_SUSPEND)) {
            return FALSE;
        }
        Suspend();
END_BEHAVIOR

// clang-format on

CameraAgent_Z::CameraAgent_Z() {
    m_Flags |= FL_AGENT_CAMERA;
    m_ViewportId = NONE_VIEWPORT;
}

void CameraAgent_Z::Init() {
}

void CameraAgent_Z::Reset() {
}

void CameraAgent_Z::Suspend() {
}

void CameraAgent_Z::Restore() {
}

Bool CameraAgent_Z::MarkHandles() {
    return FALSE;
}

void CameraAgent_Z::SetViewport(S32 m_ViewportId) {
}

void CameraAgent_Z::RegisterWithGame(Game_ZHdl& a1) {
}

void CameraAgent_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}
