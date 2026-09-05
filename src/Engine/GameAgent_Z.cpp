#include "Game_Z.h"
#include "ABC_Agent.h"
#include "ABC_ScriptManager.h"
#include "Agent_Z.h"
#include "AnimationManager_Z.h"
#include "CollisionVol_Z.h"
#include "Lod_Z.h"
#include "Node_Z.h"
#include "SystemDatas_Z.h"
#include "World_Z.h"

// TODO: Finish matching
void Game_Z::InitAgent(Node_Z* i_StartNode, Bool i_RecursiveBelow, Bool i_RecursiveNextTo) {
    if (gData.m_IsPlatformAgnostic) {
        return;
    }

    if (!m_InitAgentDepth) {
        m_NbAgent = m_GameAgentHdls.GetSize();
    }

    if (i_StartNode) {
        m_InitAgentDepth++;
        CreateAgentForNode(i_StartNode);

        if (i_RecursiveBelow) {
            InitAgent(i_StartNode->GetHeadSon(), TRUE, TRUE);
            if (i_RecursiveNextTo) {
                InitAgent(i_StartNode->GetNext(), TRUE, TRUE);
            }
        }

        m_InitAgentDepth--;
        if (!m_InitAgentDepth) {
            m_InitAgentRunning = TRUE;
            for (S32 i = m_NbAgent; i < m_GameAgentHdls.GetSize(); i++) {
                m_GameAgentHdls[i]->RegisterWithGame((Game_ZHdl&)GetHandle());
            }
            m_InitAgentRunning = FALSE;
        }
    }
}

Bool Game_Z::CreateAgentForNode(Node_Z* i_Node, Name_Z i_AgentClassName) {
    if (i_Node->IsFlagEnable(FL_NODE_NO_AGENT)) {
        return FALSE;
    }

    if (i_Node->GetAgent().IsValid()) {
        return FALSE;
    }

    Object_Z* l_Object = i_Node->GetObject();
    if (!l_Object) {
        return FALSE;
    }

    U16 l_GeometryType = l_Object->GetGeometryType();
    if (l_GeometryType != COLLISIONVOL_Z && l_GeometryType != PARTICLES_Z && (U16)(l_GeometryType - ROTSHAPE_Z) > 1) {
        return FALSE;
    }

    Agent_ZHdl l_AgentHdl;
    if (i_AgentClassName.GetID()) {
        l_AgentHdl = gData.ScriptMgr->NewAgent(i_AgentClassName);
    }

    if (!l_AgentHdl.IsValid()) {
        Name_Z l_AgentClassName = l_Object->GetName();

        if (l_GeometryType == LOD_Z) {
            Lod_Z* l_Lod = (Lod_Z*)l_Object;
            Name_Z l_AnimationName = gData.ClassMgr->GetHandleName(l_Object->GetHandle());

            if (l_Lod->GetNbObject() && l_Lod->IsFlag(FL_IS_LOD_SKIN)) {
                l_AgentHdl = gData.ScriptMgr->NewAgent(l_AgentClassName);
                if (!l_AgentHdl.IsValid()) {
                    if (!l_Lod->IsFlag(FL_IS_LOD_ANIMATED)) {
                        return FALSE;
                    }

                    if (i_Node->IsFlagEnable(FL_NODE_SEQUENCED)) {
                        l_AgentClassName = Name_Z::GetID("LodAgent_Z");
                    }
                    else {
                        S32 l_ClassId;
                        if (gData.AnimMgr->GetClassID(l_AgentClassName, l_AnimationName, l_ClassId, FALSE)) {
                            l_AgentClassName = Name_Z::GetID("AnimatedAgent_Z");
                        }
                    }
                }
            }
        }
        else if (l_GeometryType == COLLISIONVOL_Z) {
            CollisionVol_Z* l_CollisionVol = (CollisionVol_Z*)l_Object;
            if (!l_CollisionVol->HasAgentClass(l_AgentClassName)) {
                l_AgentClassName = Name_Z::GetID("VolumeAgent_Z");
            }

            l_AgentHdl = gData.ScriptMgr->NewAgent(l_AgentClassName);
            if (!l_AgentHdl.IsValid()) {
                l_AgentClassName = Name_Z::GetID("VolumeAgent_Z");
            }
        }
        else {
            l_AgentHdl = gData.ScriptMgr->NewAgent(l_AgentClassName);
            if (l_GeometryType == PARTICLES_Z && !l_AgentHdl.IsValid()) {
                l_AgentClassName = Name_Z::GetID("ParticlesAgent_Z");
            }
        }

        if (!l_AgentHdl.IsValid()) {
            l_AgentHdl = gData.ScriptMgr->NewAgent(l_AgentClassName);
            if (!l_AgentHdl.IsValid()) {
                return FALSE;
            }
        }
    }

    l_AgentHdl->UseNode((Node_ZHdl&)i_Node->GetHandle());
    m_GameAgentHdls.Add(l_AgentHdl);
    return TRUE;
}

Agent_ZHdl Game_Z::CreateAgentForSystemObject(
    const Name_Z& i_ObjectName,
    Name_Z i_AgentClassName,
    Node_ZHdl i_ParentNodeHdl,
    U32 i_Flag,
    U32 i_NoFlag
) {
    Node_ZHdl l_NodeHdl = gData.SystemDatas->GetObjectByName(i_ObjectName, 0);
    if (!l_NodeHdl.IsValid()) {
        return HANDLE_NULL;
    }

    World_Z* l_World = m_WorldHdl;
    Node_ZHdl l_ClonedNodeHdl;
    l_NodeHdl->Clone(l_ClonedNodeHdl, TRUE);
    Node_Z* l_ClonedNode = l_ClonedNodeHdl;
    Node_Z* l_WorldRoot = l_World->GetRoot();
    Node_Z* l_ParentNode = m_SubRootNodeHdl;
    Node_Z* l_InputParentNode;
    if (i_ParentNodeHdl.IsValid()) {
        l_InputParentNode = i_ParentNodeHdl;
    }
    else {
        l_InputParentNode = l_ParentNode;
    }
    l_ParentNode = l_InputParentNode ? l_ParentNode : l_WorldRoot;

    l_ParentNode->AddSon(l_ClonedNodeHdl, FALSE, TRUE);
    l_ClonedNode->SetLight(l_ParentNode->GetLight());
    l_ClonedNode->SetHFog(l_ParentNode->GetHFog());
    l_ClonedNode->EnableFlag(i_Flag);
    l_ClonedNode->DisableFlag(i_NoFlag);
    l_ClonedNode->Changed();
    l_ClonedNode->Update();

    S32 l_OldAgentCount = m_GameAgentHdls.GetSize();
    if (!CreateAgentForNode(l_ClonedNodeHdl, i_AgentClassName)) {
        return HANDLE_NULL;
    }

    if (!m_InitAgentRunning) {
        Game_ZHdl l_GameHdl(GetHandle());
        m_GameAgentHdls[l_OldAgentCount]->RegisterWithGame(l_GameHdl);
    }

    return m_GameAgentHdls[l_OldAgentCount];
}

void Game_Z::DeclareOtherAgent(const Agent_ZHdl& i_AgentHdl) {
    S32 l_AgentNb = m_GameAgentHdls.GetSize();
    for (S32 i = 0; i < l_AgentNb; i++) {
        if (m_GameAgentHdls[i] == i_AgentHdl) {
            return;
        }
    }

    m_GameAgentHdls.Add(i_AgentHdl);
}

void Game_Z::SendMessage(U32 i_Target, abc_message i_Message, Float i_Param) {
    S32 i;

    if (i_Target & GAME_MESSAGE_TARGET_PLAYER_LOD_AGENTS) {
        for (i = 0; i < m_PlayerLodAgentHdls.GetSize(); i++) {
            if (m_PlayerLodAgentHdls[i].IsValid()) {
                Send(i_Message, (ABC_Agent*)m_PlayerLodAgentHdls[i], i_Param);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS) {
        for (i = 0; i < m_PlayerCamAgentHdls.GetSize(); i++) {
            if (m_PlayerCamAgentHdls[i].IsValid()) {
                Send(i_Message, (ABC_Agent*)m_PlayerCamAgentHdls[i], i_Param);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_RTC_AGENTS) {
        for (i = 0; i < m_RtcAgentHdls.GetSize(); i++) {
            if (IsRtcActive(i)) {
                Send(i_Message, (ABC_Agent*)m_RtcAgentHdls[i], i_Param);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_GAME_AGENTS) {
        for (i = 0; i < m_GameAgentHdls.GetSize(); i++) {
            if (m_GameAgentHdls[i].IsValid()) {
                Send(i_Message, (ABC_Agent*)m_GameAgentHdls[i], i_Param);
            }
        }
    }
}

void Game_Z::FlushMessage(U32 i_Target, abc_message i_Message) {
    S32 i;

    if (i_Target & GAME_MESSAGE_TARGET_PLAYER_LOD_AGENTS) {
        for (i = 0; i < m_PlayerLodAgentHdls.GetSize(); i++) {
            if (m_PlayerLodAgentHdls[i].IsValid()) {
                ((ABC_Agent*)m_PlayerLodAgentHdls[i])->FlushMessage(i_Message);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS) {
        for (i = 0; i < m_PlayerCamAgentHdls.GetSize(); i++) {
            if (m_PlayerCamAgentHdls[i].IsValid()) {
                ((ABC_Agent*)m_PlayerCamAgentHdls[i])->FlushMessage(i_Message);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_RTC_AGENTS) {
        for (i = 0; i < m_RtcAgentHdls.GetSize(); i++) {
            if (IsRtcActive(i)) {
                ((ABC_Agent*)m_RtcAgentHdls[i])->FlushMessage(i_Message);
            }
        }
    }

    if (i_Target & GAME_MESSAGE_TARGET_GAME_AGENTS) {
        for (i = 0; i < m_GameAgentHdls.GetSize(); i++) {
            if (m_GameAgentHdls[i].IsValid()) {
                ((ABC_Agent*)m_GameAgentHdls[i])->FlushMessage(i_Message);
            }
        }
    }
}
