#include "GameManager_Z.h"
#include "MaterialManager_Z.h"
#include "World_Z.h"
#include "ABC_ScriptManager.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "Game_Z.h"
#include "Viewport_Z.h"

GameManager_Z::GameManager_Z() {
    for (S32 i = 0; i < MAX_VIEWPORT; i++) {
        m_CameraOccluderHdls[i] = gData.ClassMgr->NewObject("CameraOccluder_Z");
        m_CameraOccluderHdls[i]->SetViewportId(i);
        m_CameraOccluderHdls[i]->Activate();

        m_CameraStreamHdls[i] = gData.ClassMgr->NewObject("CameraStream_Z");
        m_CameraStreamHdls[i]->SetViewportId(i);
        m_CameraStreamHdls[i]->Activate();

        m_CameraUserBools[i] = FALSE;

        m_CameraUserHdls[i] = gData.ClassMgr->NewObject("CameraUser_Z");
        m_CameraUserHdls[i]->SetViewportId(i);
        m_CameraUserHdls[i]->SetInputNum(i);
    }

    m_IsPaused = FALSE;
}

void GameManager_Z::Update(Float i_DeltaTime) {
    if (m_IsPaused) {
        return;
    }

    gData.MaterialMgr->Update(i_DeltaTime);

    for (S32 i = 0; i < m_GameHdls.GetSize(); i++) {
        Game_Z* l_Game = m_GameHdls[i];
        if (l_Game) {
            l_Game->Update(i_DeltaTime);
        }
    }
}

void GameManager_Z::MarkHandles() {
    S32 i;

    for (i = 0; i < m_GameHdls.GetSize(); i++) {
        Game_Z* l_Game = m_GameHdls[i];

        if (l_Game) {
            l_Game->MarkHandles();
        }
    }

    for (i = 0; i < MAX_VIEWPORT; i++) {
        m_CameraUserHdls[i]->MarkHandles();
        m_CameraOccluderHdls[i]->MarkHandles();
        m_CameraStreamHdls[i]->MarkHandles();
    }
}

void GameManager_Z::AddGame(const World_ZHdl& i_WorldHdl, const Char* i_WorldName, S32 i_PlayerCount, Bool i_IsMono, Name_Z i_CameraAgentClass) {
    if (GetGameIdByWorld(i_WorldHdl) >= 0) {
        return;
    }

    Game_ZHdl l_GameHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Game_Z")), i_WorldHdl->GetName());
    m_GameHdls.Add(l_GameHdl);

    S32 l_GameId = m_GameHdls.GetSize() - 1;
    Game_Z* l_Game = l_GameHdl;

    l_Game->SetGameWorld(i_WorldHdl, i_WorldName);
    l_Game->SetGamePlayerNb(i_PlayerCount, i_IsMono, i_CameraAgentClass);

    gData.ScriptMgr->GameSet(m_GameHdls[l_GameId]);
    l_Game->InitAgent(i_WorldHdl->GetRoot(), TRUE, FALSE);
    gData.ScriptMgr->GameAgentSet(m_GameHdls[l_GameId]);
}

// TODO: Finish matching
void GameManager_Z::ActivateGame(S32 i_GameId, S32 i_PlayerId) {
    S32 i;
    S32 l_FreeVp = MAX_VIEWPORT;

    for (i = 0; i < m_GameHdls.GetSize(); i++) {
        Game_Z* l_Game = m_GameHdls[i];
        if (l_Game->GetFirstVp() >= 0) {
            l_FreeVp -= l_Game->GetNbVp();
        }
    }

    S32 l_UsedVp = 0;

    for (i = 0; i < m_GameHdls.GetSize(); i++) {
        Game_Z* l_Game = m_GameHdls[i];
        if (l_Game->GetFirstVp() >= 0) {
            l_UsedVp += l_Game->GetNbVp();
        }
    }

    S32 l_StartVp = l_UsedVp;
    Game_Z* l_Game = m_GameHdls[i_GameId];

    if (l_Game->GetFirstVp() >= 0) {
        l_FreeVp += l_Game->GetNbVp();
        l_StartVp = l_UsedVp - l_Game->GetNbVp();
    }

    if (i_PlayerId == -1) {
        i_PlayerId = l_Game->GetNbVp();
    }

    S32 l_FirstVp = MAX_VIEWPORT - l_FreeVp;

    gData.MainRdr->InitViewport(l_FirstVp + i_PlayerId);

    for (S32 j = 0; j < i_PlayerId; j++) {
        gData.MainRdr->GetViewport(l_StartVp + j).SetWorld(World_ZHdl(l_Game->GetWorld()));
    }

    gData.MainRdr->EnableEffectFlag(FL_EFFECT_CLEAR_SCREEN);

    l_Game->Activate(l_FirstVp, i_PlayerId);
}

S32 GameManager_Z::GetGameIdByWorld(const World_ZHdl& i_WorldHdl) {
    for (S32 i = 0; i < GetNbGame(); i++) {
        Game_Z* l_Game = m_GameHdls[i];

        if (l_Game && l_Game->m_WorldHdl == i_WorldHdl) {
            return i;
        }
    }

    return -1;
}
