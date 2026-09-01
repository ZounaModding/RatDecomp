#include "GameManager_Z.h"
#include "MaterialManager_Z.h"

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
}

void GameManager_Z::AddGame(const World_ZHdl& i_WorldHdl, const Char* i_WorldName, S32 i_PlayerCount, Bool i_IsMono, Name_Z i_CameraAgentClass) { }

void GameManager_Z::ActivateGame(S32 i_GameId, S32 i_PlayerId) { }

S32 GameManager_Z::GetGameIdByWorld(const World_ZHdl& i_WorldHdl) {
    for (S32 i = 0; i < GetNbGame(); i++) {
        Game_Z* l_Game = m_GameHdls[i];

        if (l_Game && l_Game->m_WorldHdl == i_WorldHdl) {
            return i;
        }
    }

    return -1;
}
