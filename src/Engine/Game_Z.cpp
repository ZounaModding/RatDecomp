#include "Game_Z.h"
#include "Program_Z.h"
#include "ObjectsGame_Z.h"
#include "World_Z.h"
#include "LodMove_Z.h"
#include "CameraAgent_Z.h"
#include "LodAgent_Z.h"
#include "RtcAgent_Z.h"
#include "ABC_ScriptManager.h"
#include "Camera_ZHdl.h"
#include "SystemDatas_Z.h"
#include "ObjectsGameClip_ZHdl.h"
#include "Renderer_Z.h"

String_Z<ARRAY_CHAR_MAX> Game_Z::m_AddStartBaseName;

void Game_Z::Init() {
    m_NbPlayer = 0;
    m_InitAgentRunning = FALSE;
    m_InitAgentDepth = 0;
    m_ObjectsGameMgrHdl = gData.ClassMgr->NewObject("ObjectsGame_Z");
    ObjectsGameClip_ZHdl l_ObjectsGameClipHdl = gData.ClassMgr->NewObject("ObjectsGameClip_Z");
    DeclareObjectGame(l_ObjectsGameClipHdl);
    l_ObjectsGameClipHdl->SetObjectsGameHdl(m_ObjectsGameMgrHdl);
}

S32 Game_Z::GetFirstVp() const {
    return m_ObjectsGameMgrHdl->GetFirstVp();
}

U32 Game_Z::GetNbVp() const {
    return m_ObjectsGameMgrHdl->GetNbVp();
}

// TODO: Finish matching
void Game_Z::SetGameWorld(const World_ZHdl& i_WorldHdl, const Char* i_GameName) {
    m_WorldHdl = i_WorldHdl;
    m_GameName = i_GameName;
    // GetPtr is called by an inlined operator-> here
    m_ObjectsGameMgrHdl->SetGame(GetHandle());

    for (S32 i = 0; i < m_ObjectGameHdls.GetSize(); i++) {
        // GetPtr is called by an inlined operator-> here
        m_ObjectGameHdls[i]->SetGame(GetHandle());
        // the stack address of the return value of GetHandle()
        // should be loaded into r27 before the loop and then moved
        // into r4, instead it's being added directly into r4
    }

    // GetPtr is called by an inlined operator-> here
    m_SubRootNodeHdl = i_WorldHdl->GetNodeByName(Name_Z::GetID("SUB_ROOT"));
}

Bool Game_Z::MarkHandles() {
    S32 i;

    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    for (i = 0; i < m_NbPlayer; i++) {
        if (m_PlayerLodMoveHdls[i].IsValid()) {
            m_PlayerLodMoveHdls[i]->MarkHandles();
        }
        if (m_PlayerCamAgentHdls[i].IsValid()) {
            m_PlayerCamAgentHdls[i]->MarkHandles();
        }
        if (m_PlayerLodAgentHdls[i].IsValid()) {
            m_PlayerLodAgentHdls[i]->MarkHandles();
        }
        if (m_PlayerCamNodeHdls[i].IsValid()) {
            m_PlayerCamNodeHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_GameAgentHdls.GetSize(); i++) {
        if (m_GameAgentHdls[i].IsValid()) {
            m_GameAgentHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_RtcAgentHdls.GetSize(); i++) {
        if (m_RtcAgentHdls[i].IsValid()) {
            m_RtcAgentHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_ObjectGameHdls.GetSize(); i++) {
        if (m_ObjectGameHdls[i].IsValid()) {
            m_ObjectGameHdls[i]->MarkHandles();
        }
    }

    m_ObjectsGameMgrHdl->MarkHandles();

    return TRUE;
}

void Game_Z::Update(Float i_DeltaTime) {
    if (m_ObjectsGameMgrHdl->GetFirstVp() >= 0) {
        m_WorldHdl->Update(i_DeltaTime);
    }
    else {
        if (m_StreamNotifyAgent.IsValid()) {
            Stream(VEC3F_NULL, HANDLE_NULL, msg_no_message);
        }
    }
}

void Game_Z::SetGamePlayerNb(S32 i_Nb, Bool i_IsMono, const Name_Z& i_CameraAgentClass) {
    if (m_NbPlayer) {
        return;
    }

    m_NbPlayer = i_Nb;
    m_PlayerLodMoveHdls.SetSize(m_NbPlayer);
    m_PlayerLodAgentHdls.SetSize(m_NbPlayer);
    m_PlayerCamNodeHdls.SetSize(m_NbPlayer);
    m_PlayerCamAgentHdls.SetSize(m_NbPlayer);
    m_ObjectsGameMgrHdl->SetNbVp(i_IsMono ? 1 : m_NbPlayer);

    String_Z<ARRAY_CHAR_MAX> l_CameraNameStr;

    World_Z* l_World = m_WorldHdl;
    Node_ZHdl l_StartBaseHdl = l_World->GetNodeByName(Name_Z::GetID("Start_Base"));
    Vec2f l_Min;
    Vec2f l_Max;
    l_World->GetSeadDisplay().GetSize(l_Min, l_Max);
    l_Min += l_Max;
    l_Min /= 2.0f;
    Vec3f l_StartPos(l_Min.x, 0.0f, l_Min.y);
    if (l_StartBaseHdl.IsValid()) {
        l_StartPos = l_StartBaseHdl->GetWorldTranslation();
    }

    for (S32 i = 0; i < m_NbPlayer; i++) {
        CameraAgent_ZHdl l_CamAgentHdl = gData.ScriptMgr->NewAgent(i_CameraAgentClass, "CameraAgent");
        if (!l_CamAgentHdl.IsValid()) {
            l_CamAgentHdl = gData.ScriptMgr->NewAgent(Name_Z::GetID("CameraAgent_Z"), "CameraAgent");
        }
        l_CameraNameStr.Sprintf("%s_CAMERA_VP_%d", (const Char*)m_GameName, i);
        m_PlayerCamAgentHdls[i] = l_CamAgentHdl;
        CameraAgent_Z* l_CamAgent = l_CamAgentHdl;

        m_PlayerCamNodeHdls[i] = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Node_Z")), Name_Z::GetID(l_CameraNameStr));
        Node_Z* l_CamNode = m_PlayerCamNodeHdls[i];
        l_CamNode->SetTranslation(l_StartPos);
        l_CamNode->SetLight(gData.SystemDatas->GetDefaultLight());

        l_CamNode->SetObject((Camera_ZHdl&)gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Camera_Z")), Name_Z::GetID(l_CameraNameStr)));
        l_CamNode->Changed();
        l_CamNode->Update();
        l_World->GetRoot()->AddSon(m_PlayerCamNodeHdls[i], TRUE);

        l_CamAgent->UseNode((Node_ZHdl&)m_PlayerCamNodeHdls[i]);
        l_CamAgent->RegisterWithGame((Game_ZHdl&)GetHandle());
    }
}

// TODO: Finish matching
void Game_Z::Activate(S32 i_FirstVp, S32 i_NbVp) {
    Bool l_WasActive;
    S32 i;

    TryToSuspend();

    l_WasActive = m_ObjectsGameMgrHdl->GetFirstVp() >= 0;

    for (i = 0; i < m_NbPlayer; i++) {
        if (m_PlayerLodAgentHdls[i].IsValid()) {
            m_PlayerLodAgentHdls[i]->SetViewportId(i_FirstVp + i % i_NbVp);
        }
        m_PlayerCamAgentHdls[i]->Suspend();
        m_PlayerCamAgentHdls[i]->SetViewport(i_FirstVp + i % i_NbVp);
    }

    ObjectsGame_Z* l_ObjectsGameMgr = m_ObjectsGameMgrHdl;
    l_ObjectsGameMgr->SetNbVp(i_NbVp);
    l_ObjectsGameMgr->SetFirstVp(i_FirstVp);

    for (i = 0; i < m_ObjectGameHdls.GetSize(); i++) {
        ObjectGame_Z* l_ObjectGame = m_ObjectGameHdls[i];
        l_ObjectGame->SetNbVp(i_NbVp);
        l_ObjectGame->SetFirstVp(i_FirstVp);
    }

    m_WorldHdl->SetNbVp(i_NbVp);
    m_WorldHdl->SetFirstVp(i_FirstVp);

    Restore();
    FlushMessage(GAME_MESSAGE_TARGET_PLAYER_LOD_AGENTS | GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS | GAME_MESSAGE_TARGET_RTC_AGENTS | GAME_MESSAGE_TARGET_GAME_AGENTS, msg_game_deactivated);

    if (!l_WasActive) {
        SendMessage(GAME_MESSAGE_TARGET_PLAYER_LOD_AGENTS | GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS | GAME_MESSAGE_TARGET_RTC_AGENTS | GAME_MESSAGE_TARGET_GAME_AGENTS, msg_game_activated, UNDEFINED_FVALUE);
        gData.ScriptMgr->Update(0.0f);
        gData.ScriptMgr->ActivateGame(GetHandle());
    }

    gData.ScriptMgr->ViewportDone(GetHandle());
}

void Game_Z::Restore() {
    S32 i;
    S32 l_FirstVp = m_ObjectsGameMgrHdl->GetFirstVp();
    S32 l_NbVp = m_ObjectsGameMgrHdl->GetNbVp();

    for (i = 0; i < m_NbPlayer; i++) {
        if (m_PlayerLodAgentHdls[i].IsValid()) {
            m_PlayerLodAgentHdls[i]->Restore();
        }
        if (i < l_NbVp) {
            RestoreVpCamera(l_FirstVp + i % l_NbVp);
        }
        else {
            m_PlayerCamAgentHdls[i]->Suspend();
        }
    }

    for (i = 0; i < m_ObjectGameHdls.GetSize(); i++) {
        m_ObjectGameHdls[i]->Activate();
    }

    m_ObjectsGameMgrHdl->Activate();
}

void Game_Z::RestoreVpCamera(S32 i_Vp) {
    S32 l_FirstVp = m_ObjectsGameMgrHdl->GetFirstVp();

    m_ObjectsGameMgrHdl->GetFirstVp();

    S32 l_CamId = i_Vp - l_FirstVp;

    gData.MainRdr->GetViewport(i_Vp).SetCamera(m_PlayerCamNodeHdls[l_CamId]);
    gData.MainRdr->GetViewport(i_Vp).SetWorld(World_ZHdl(m_WorldHdl));
    m_PlayerCamAgentHdls[l_CamId]->Restore();
}
