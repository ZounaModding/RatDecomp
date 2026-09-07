#include "GameMgr_G.h"

#include "Assert_Z.h"
#include "Language_Z.h"
#include "Main_Z.h"
#include "Renderer_Z.h"
#include "ScriptManager_G.h"
#include "SoundManager_Z.h"

CTFGameMgr_G::CTFGameMgr_G() {
}

Bool CTFGameMgr_G::Init() {
    m_Initialized = TRUE;
    m_ConfigStruct.m_UnkBool_0x29 = FALSE;
    m_ConfigStruct.m_UnkBool_0x2a = FALSE;
    m_ConfigStruct.m_UnkBool_0x2b = FALSE;
    m_ConfigStruct.m_UnkBool_0x2c = FALSE;
    m_FadeVolume = 1.0f;
    m_Initialized &= InitSaveStruct(TRUE);
    m_Initialized &= m_Shop.Init();
    m_AutoSaveEnabled = FALSE;
    m_FileIdToLoad = -1;
    m_DontSave = FALSE;
    return m_Initialized;
}

CTFGameMgr_G::~CTFGameMgr_G() {
}

void CTFGameMgr_G::InitConfiguration() {
    SetVideoMode(0);
    SetMusicVolume(8);
    SetSfxVolume(8);
    SetDialogVolume(8);
    m_ConfigStruct.m_CamSens = 8;
    SetScreenPosX(0);
    SetScreenPosY(0);
    m_ConfigStruct.m_UnkBool_0x25 = TRUE;
    m_ConfigStruct.m_UnkBool_0x26 = TRUE;
    m_ConfigStruct.m_UnkBool_0x27 = TRUE;
    m_ConfigStruct.m_IsNightmare = TRUE;
    m_ConfigStruct.m_InvertX = FALSE;
    m_ConfigStruct.m_InvertY = FALSE;
    m_ConfigStruct.m_Difficulty = 2;
    SetSubTitles(TRUE);
    m_UnkFloat_Zero_0x64f0 = 0.0f;
    m_UnkFloat_PointOne_0x64f4 = 0.1f;
}

void CTFGameMgr_G::SetCurrentLanguage(S32 i_Language) {
    langDefineDA& l_Defines = gScriptMgr->GetLangDefines();
    for (S32 i = 0; i < l_Defines.GetSize(); ++i) {
        langDefine& l_Define = l_Defines.GetArrayPtr()[i];
        if (i_Language == l_Define.m_TrTextId) {
            SetLanguage(l_Define.m_TrTextId, l_Define.m_DialogId, l_Define.m_MpegId);
            return;
        }
    }
    ASSERTL_Z(FALSE, "Language not found !", 0x131);
}

void CTFGameMgr_G::SetVideoMode(S32 i_VideoMode) {
    if (gData.MainRdr->GetScreenRatio() == RATIO_SCREEN_STANDARD) {
        if (i_VideoMode == screen_widescreen) {
            Renderer_Z::SwitchScreen(screen_widescreen);
        }
    }
    else if (i_VideoMode == screen_standard) {
        Renderer_Z::SwitchScreen(screen_standard);
    }
    m_ConfigStruct.m_VideoMode = i_VideoMode;
}

void CTFGameMgr_G::SetMusicVolume(U32 i_Volume) {
    m_ConfigStruct.m_MusicVolume = i_Volume;
    if (m_ConfigStruct.m_MusicVolume < 0) {
        m_ConfigStruct.m_MusicVolume = 0;
    }
    else if (m_ConfigStruct.m_MusicVolume > 16) {
        m_ConfigStruct.m_MusicVolume = 16;
    }
    gData.SoundMgr->SetMusicVol((Float)m_ConfigStruct.m_MusicVolume / 16.0f);
}

void CTFGameMgr_G::SetSfxVolume(U32 i_Volume) {
    m_ConfigStruct.m_SfxVolume = i_Volume;
    if (m_ConfigStruct.m_SfxVolume < 0) {
        m_ConfigStruct.m_SfxVolume = 0;
    }
    else if (m_ConfigStruct.m_SfxVolume > 16) {
        m_ConfigStruct.m_SfxVolume = 16;
    }
    gData.SoundMgr->SetSfxVol((Float)m_ConfigStruct.m_SfxVolume / 16.0f);
}

void CTFGameMgr_G::SetDialogVolume(U32 i_Volume) {
    m_ConfigStruct.m_DialogVolume = i_Volume;
    if (m_ConfigStruct.m_DialogVolume < 0) {
        m_ConfigStruct.m_DialogVolume = 0;
    }
    else if (m_ConfigStruct.m_DialogVolume > 16) {
        m_ConfigStruct.m_DialogVolume = 16;
    }
    gData.SoundMgr->SetDlgVol((Float)m_ConfigStruct.m_DialogVolume / 16.0f);
}

void CTFGameMgr_G::SetFadeVolume(Float i_Volume) {
    if (i_Volume != m_FadeVolume) {
        gData.SoundMgr->SetSfxVol(i_Volume * (Float)m_ConfigStruct.m_SfxVolume / 16.0f);
        m_FadeVolume = i_Volume;
    }
}

void CTFGameMgr_G::SetScreenPosX(S32 i_ScreenPosX) {
    gData.MainRdr->MoveScreenOrigin(i_ScreenPosX - m_ConfigStruct.m_ScreenPosX, 0);
    m_ConfigStruct.m_ScreenPosX = i_ScreenPosX;
}

void CTFGameMgr_G::SetScreenPosY(S32 i_ScreenPosY) {
    gData.MainRdr->MoveScreenOrigin(0, i_ScreenPosY - m_ConfigStruct.m_ScreenPosY);
    m_ConfigStruct.m_ScreenPosY = i_ScreenPosY;
}

void CTFGameMgr_G::SetSubTitles(Bool i_SubTitles) {
    m_ConfigStruct.m_Subtitles = i_SubTitles;
}

void CTFGameMgr_G::AddTime(Float i_DeltaTime) {
    m_SaveStruct.m_SecondsPlayed += i_DeltaTime;
    if (m_SaveStruct.m_SecondsPlayed >= 3600.0f) {
        m_SaveStruct.m_SecondsPlayed -= 3600.0f;
        m_SaveStruct.m_HoursPlayed += 1.0f;
    }
}

Bool CTFGameMgr_G::InitSaveStruct(Bool i_ResetActive) {
    m_SaveStruct.m_HoursPlayed = 0.0f;
    m_SaveStruct.m_SecondsPlayed = 0.0f;
    m_SaveStruct.m_LogicAdvancement = 0.0f;
    m_SaveStruct.m_LevelDataId = 1;
    if (i_ResetActive) {
        m_SaveStruct.m_Active = FALSE;
    }
    return TRUE;
}
