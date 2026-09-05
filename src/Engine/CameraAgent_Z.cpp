#include "CameraAgent_Z.h"
#include "ABC_Message_Engine.h"
#include "Renderer_Z.h"
#include "CameraEngine_Z.h"
#include "ClassManager_Z.h"
#include "Game_Z.h"
#include "Node_Z.h"
#include "ParticlesAgent_Z.h"
#include "String_Z.h"
#include "SystemDatas_Z.h"
#include "World_Z.h"

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
    Agent_Z::Init();
    if (m_Flags & FL_AGENT_CAMERA) {
        m_DefaultCameraEngineHdl = gData.ClassMgr->NewObject("CameraEngineZone_Z");
    }
}

void CameraAgent_Z::Reset() {
}

void CameraAgent_Z::Suspend() {
    if (!(m_Flags & FL_AGENT_NO_SUSPEND)) {
        if (GetCameraManipulator(-1)) {
            GetCameraManipulator(-1)->Deactivate();
        }
        FxSuspend(TRUE);
        ABC_Agent::Deactivate();
    }
}

void CameraAgent_Z::FxSuspend(Bool i_Hide) {
    S32 l_Nb = m_ParticlesAgents.GetSize();
    for (S32 i = 0; i < l_Nb; i++) {
        ParticlesAgent_Z* l_Agent = (ParticlesAgent_Z*)GETPTR(m_ParticlesAgents[i]);
        if (l_Agent) {
            l_Agent->EnableFlag(FL_AGENT_NO_RESTORE);
            l_Agent->DisableFlag(FL_AGENT_NO_SUSPEND);
            l_Agent->Suspend();
            if (i_Hide) {
                l_Agent->GetNode()->Hide(FALSE);
            }
        }
    }
}

void CameraAgent_Z::Restore() {
    if (!(m_Flags & FL_AGENT_NO_RESTORE)) {
        if (GetCameraManipulator(-1)) {
            GetCameraManipulator(-1)->Activate();
        }
        S32 l_Nb = m_ParticlesAgents.GetSize();
        for (S32 i = 0; i < l_Nb; i++) {
            ParticlesAgent_Z* l_Agent = m_ParticlesAgents[i];
            if (l_Agent) {
                l_Agent->EnableFlag(FL_AGENT_NO_SUSPEND);
                l_Agent->DisableFlag(FL_AGENT_NO_RESTORE);
                l_Agent->Restore();
                l_Agent->Resume();
                Node_Z* l_Node = l_Agent->GetNode();
                ObjectDatas_Z* l_ObjectDatas = l_Node->GetObjectDatas();
                l_ObjectDatas->EnableFlag(FL_OBJECTDATAS_VP0_HIDE | FL_OBJECTDATAS_VP1_HIDE | FL_OBJECTDATAS_VP2_HIDE | FL_OBJECTDATAS_VP3_HIDE);
                l_ObjectDatas->DisableFlag(FL_OBJECTDATAS_VPX_HIDE(m_ViewportId));
                l_Node->UnHide(FALSE);
            }
        }
        ABC_Agent::Activate();
    }
}

Bool CameraAgent_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    S32 l_Nb = m_ParticlesAgents.GetSize();

    for (S32 i = 0; i < l_Nb; i++) {
        ParticlesAgent_Z* l_Agent = m_ParticlesAgents[i];

        if (l_Agent) {
            l_Agent->MarkHandles();
        }
    }

    return GetCameraManipulator(-1)->MarkHandles();
}

void CameraAgent_Z::SetViewport(S32 i_ViewportId) {
    m_ViewportId = i_ViewportId;
    GetCameraManipulator(-1)->SetViewport(i_ViewportId);
}

void CameraAgent_Z::RegisterWithGame(Game_ZHdl& i_GameHdl) {
    Agent_Z::RegisterWithGame(i_GameHdl);
    CameraEngine_ZHdl& l_CameraEngineHdl = GetCameraManipulator(-1);
    if (l_CameraEngineHdl) {
        l_CameraEngineHdl->SetGame(i_GameHdl);
        Game_Z* l_Game = i_GameHdl;
        GetNode();
        for (S32 i = 0; i < 4; i++) {
            String_Z<32> l_Name;
            l_Name.Sprintf("FX_VP_%d", i);
            Node_ZHdl l_ObjectHdl = gData.SystemDatas->GetObjectByName(Name_Z(Name_Z::GetID(l_Name)), 0x12);
            if (l_ObjectHdl.IsValid()) {
                ParticlesAgent_ZHdl l_AgentHdl = l_Game->CreateAgentForSystemObject(Name_Z(Name_Z::GetID(l_Name)), Name_Z(Name_Z::GetID("ParticlesAgent_Z")), HANDLE_NULL, FL_NODE_COLLIDE);
                ParticlesAgent_Z* l_Agent = l_AgentHdl;
                if (l_Agent) {
                    m_ParticlesAgents.Add(l_AgentHdl);
                    l_Agent->Suspend();
                    l_Agent->EnableFlag(FL_AGENT_NO_SUSPEND | FL_AGENT_NO_RESTORE);
                    l_Game->GetWorld()->NoFrustrumClipping(l_Agent->GetNodeHdl(), TRUE);
                    l_Agent->GetNode()->Hide(FALSE);
                }
            }
        }
    }
}

void CameraAgent_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}
