#include "ScriptManager_G.h"

#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "LevelData_G.h"
#include "GameManager_Z.h"
#include "Game_Z.h"
#include "Memory_Z.h"
#include "MemoryCardMgr_G.h"
#include "MusicManager_G.h"
#include "PaddleChecker_G.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "TextGameDraw_G.h"
#include "LogicAgent_G.h"
#include "VibrationManager_G.h"
#include "MissionVolumeAgent_G.h"
#include "ConditionVolumeAgent_G.h"
#include "Mission_Global.h"
#include "LogicLevel.h"
#include "PersoLight_G.h"
#include "Console_Z.h"

ScriptManager_G::ScriptManager_G() {
    m_PlayerSaveStruct.Reset();
    m_UnkFloatZero_0x666c = 0.0f;
    m_MovieToBePlayed[0] = '\0';
    m_PackedMenuCommandBuffer = NULL;
    m_PackedMenuCommandBufferSize = 0;
    m_EnableDebugTools = FALSE;
}

ScriptManager_G::~ScriptManager_G() {
    if (m_PackedMenuCommandBuffer != NULL) {
        Free_Z(m_PackedMenuCommandBuffer);
    }
}

Bool ScriptManager_G::Minimize() {
    m_TextGameDrawMgrHdl->Minimize();
    return TRUE;
}

void ScriptManager_G::Init() {
    ABC_ScriptManager::Init();
    InitGameScriptClasses();

    m_LevelAgentHdl = gData.ScriptMgr->NewAgent(Name_Z::GetID("LevelAgent_G"));
    m_ScriptGameMgr.InitConfiguration();
    m_SynchedPlatformsTime = 0.0f;

    AnimationManager_Z* l_AnimMgr = gData.AnimMgr;
    l_AnimMgr->SetNodeId(Name_Z::GetID("SPINE"), BONE_SPINE);
    l_AnimMgr->SetNodeId(Name_Z::GetID("SPINE1"), BONE_SPINE1);
    l_AnimMgr->SetNodeId(Name_Z::GetID("SPINE2"), BONE_SPINE2);
    l_AnimMgr->SetNodeId(Name_Z::GetID("SPINE3"), BONE_SPINE3);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTSHOULDER"), BONE_LEFTSHOULDER);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTARM"), BONE_LEFTARM);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTFOREARM"), BONE_LEFTFOREARM);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTHAND"), BONE_LEFTHAND);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTSHOULDER"), BONE_RIGHTSHOULDER);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTARM"), BONE_RIGHTARM);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTFOREARM"), BONE_RIGHTFOREARM);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTHAND"), BONE_RIGHTHAND);
    l_AnimMgr->SetNodeId(Name_Z::GetID("NECK"), BONE_NECK);
    l_AnimMgr->SetNodeId(Name_Z::GetID("HEAD"), BONE_HEAD);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTUPLEG"), BONE_LEFTUPLEG);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTLEG"), BONE_LEFTLEG);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTFOOT"), BONE_LEFTFOOT);
    l_AnimMgr->SetNodeId(Name_Z::GetID("LEFTTOEBASE"), BONE_LEFTTOEBASE);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTUPLEG"), BONE_RIGHTUPLEG);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTLEG"), BONE_RIGHTLEG);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTFOOT"), BONE_RIGHTFOOT);
    l_AnimMgr->SetNodeId(Name_Z::GetID("RIGHTTOEBASE"), BONE_RIGHTTOEBASE);
    l_AnimMgr->SetNodeId(Name_Z::GetID("TONGUE"), BONE_TONGUE);
    l_AnimMgr->SetNodeId(Name_Z::GetID("HOOK"), BONE_HOOK);
    l_AnimMgr->SetNodeId(Name_Z::GetID("PELVIS"), BONE_PELVIS);
    l_AnimMgr->SetNodeId(Name_Z::GetID("TAIL5"), BONE_TAIL5);
    l_AnimMgr->SetNodeId(Name_Z::GetID("SCALE_CONTROL"), BONE_SCALE_CONTROL);

    l_AnimMgr->SetMaterialId(Name_Z::GetID("CLOTHE1"), MAT_CLOTHE1);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("CLOTHE2"), MAT_CLOTHE2);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("CLOTHE3"), MAT_CLOTHE3);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("CLOTHE4"), MAT_CLOTHE4);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("CLOTHE5"), MAT_CLOTHE5);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("BOMBE"), MAT_BOMBE);
    l_AnimMgr->SetMaterialId(Name_Z::GetID("BALLON"), MAT_BALLON);

    m_MemoryCardMgrHdl = gData.ClassMgr->NewObject("MemoryCardManager_C");
    m_MemoryCardMgrHdl->Activate();
    m_PaddleCheckerHdl = gData.ClassMgr->NewObject("PaddleChecker_G");
    m_PaddleCheckerHdl->Activate();
    m_VibrationMgrHdl = gData.ClassMgr->NewObject("VibrationManager_C");
    m_MusicMgrHdl = gData.ClassMgr->NewObject("MusicManager_G");
    m_MusicMgrHdl->Activate();

    m_LoadingDrawHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("LoadingDraw_C")), Name_Z(Name_Z::GetID("LoadingDraw_C")));
    m_PermanentDrawHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("PermanentDraw")), Name_Z(Name_Z::GetID("PermanentDraw")));

    m_TextGameDrawMgrHdl = gData.ClassMgr->NewObject("TextGameDraw_G");
    m_ActionHelperMgrHdl = gData.ClassMgr->NewObject("ActionHelper_G");
    m_FootPrintsMgrHdl = gData.ClassMgr->NewObject("FootPrints_G");
    m_RopesMgrHdl = gData.ClassMgr->NewObject("Ropes_G");
    m_SmellMgrHdl = gData.ClassMgr->NewObject("Smell_G");
    m_LiquidFlowMgrHdl = gData.ClassMgr->NewObject("LiquidFlow_G");
    m_ConditionMgrHdl = gData.ClassMgr->NewObject("IT_Condition");
    m_WanderingPathMgrHdl = gData.ClassMgr->NewObject("WanderingPath_G");
    m_PointJumpMgrHdl = gData.ClassMgr->NewObject("PointJump_G");
    m_MissionManagerHdl = gData.ClassMgr->NewObject("MissionManager_G");
    m_InGameFXMgrHdl = gData.ClassMgr->NewObject("InGameFX_G");
    m_PersoLightMgrHdl = gData.ClassMgr->NewObject("PersoLight_G");

    ResetAdvancement();

    Vec3f l_Color1(SPECIAL_VISION_COLOR_1_R, SPECIAL_VISION_COLOR_1_G, SPECIAL_VISION_COLOR_1_B);
    Vec3f l_Color2(SPECIAL_VISION_COLOR_2_INTENSITY, SPECIAL_VISION_COLOR_2_INTENSITY, SPECIAL_VISION_COLOR_2_INTENSITY);
    Vec3f l_Color3(SPECIAL_VISION_HIGHLIGHT_R, SPECIAL_VISION_HIGHLIGHT_G, SPECIAL_VISION_HIGHLIGHT_B);
    Vec3f l_Color4(SPECIAL_VISION_HIGHLIGHT_R, SPECIAL_VISION_HIGHLIGHT_G, SPECIAL_VISION_HIGHLIGHT_B);
    gData.MainRdr->SetSpecialVisionColor(l_Color1, l_Color2, l_Color3, l_Color4);
}

void ScriptManager_G::Shut() {
    ABC_ScriptManager::Shut();
    ShutGameScriptClasses();
}

void ScriptManager_G::MarkHandles() {
    ABC_ScriptManager::MarkHandles();

    S32 i;
    for (i = 0; i < m_LogicAgents.GetSize(); ++i) {
        MarkValidHandle_Z(m_LogicAgents[i]);
    }
    for (i = 0; i < m_DialogGroups.GetSize(); ++i) {
        MarkValidHandle_Z(m_DialogGroups[i]);
    }

    MarkValidHandle_Z(m_LevelAgentHdl);
    MarkValidHandle_Z(m_InGameDatasHdl);
    MarkHandle_Z(m_MemoryCardMgrHdl);
    MarkHandle_Z(m_PaddleCheckerHdl);
    MarkHandle_Z(m_VibrationMgrHdl);
    MarkHandle_Z(m_LoadingDrawHdl);
    MarkHandle_Z(m_PermanentDrawHdl);
    MarkHandle_Z(m_ActionHelperMgrHdl);
    MarkHandle_Z(m_FootPrintsMgrHdl);
    MarkHandle_Z(m_TextGameDrawMgrHdl);
    MarkHandle_Z(m_RopesMgrHdl);
    MarkHandle_Z(m_SmellMgrHdl);
    MarkHandle_Z(m_LiquidFlowMgrHdl);
    MarkHandle_Z(m_ConditionMgrHdl);
    MarkHandle_Z(m_WanderingPathMgrHdl);
    MarkHandle_Z(m_PointJumpMgrHdl);
    MarkHandle_Z(m_MissionManagerHdl);
    MarkHandle_Z(m_InGameFXMgrHdl);
    MarkHandle_Z(m_PersoLightMgrHdl);
    MarkHandle_Z(m_MusicMgrHdl);

    for (i = 0; i < m_Levels.GetSize(); ++i) {
        MarkValidHandle_Z(m_Levels[i]);
    }
    for (i = 0; i < m_LogicLevels.GetSize(); ++i) {
        MarkValidHandle_Z(m_LogicLevels[i]);
    }
    for (i = 0; i < m_Characters.GetSize(); ++i) {
        MarkValidHandle_Z(m_Characters[i]);
    }

    MarkValidHandle_Z(m_OmniHdlForFX);
    MarkValidHandle_Z(m_OmniNodeHdlForFX);
}

void ScriptManager_G::Update(Float i_DeltaTime) {
    ABC_ScriptManager::Update(i_DeltaTime);
    UpdateOmnis(i_DeltaTime);
    UpdatePlatForms(i_DeltaTime);
    if (!IsPaused()) {
        m_ScriptGameMgr.AddTime(i_DeltaTime);
    }
}

void ScriptManager_G::RemoveLogicAgent(const Game_ZHdl& i_GameHdl) {
}

void ScriptManager_G::StopAllVibrations() {
    m_VibrationMgrHdl->StopAllVibrations();
}

LevelData_GHdl ScriptManager_G::AddLevelData() {
    LevelData_GHdl l_LevelHdl = gData.ClassMgr->NewObject("LevelData_G");
    m_Levels.Add(l_LevelHdl);
    return m_Levels[m_Levels.GetSize() - 1];
}

S32 ScriptManager_G::GetLevelDataId(LevelData_GHdl i_Hdl) {
    for (S32 l_Index = 0; l_Index < m_Levels.GetSize(); ++l_Index) {
        if (m_Levels[l_Index] == i_Hdl) {
            return l_Index;
        }
    }
    return -1;
}

void ScriptManager_G::RemoveOldLevel() {
    S32 l_Removed = FALSE;
    for (S32 l_Index = 0; l_Index < gData.GameMgr->GetNbGame(); ++l_Index) {
        if (gData.GameMgr->GetGame(l_Index)) {
            gData.GameMgr->DeactivateGame(l_Index);
            gData.GameMgr->RemoveGame(l_Index, FALSE);
            l_Removed = TRUE;
        }
    }
    if (l_Removed) {
        gScriptMgr->m_PersoLightMgrHdl->GameRemoved();
        gData.Cons->PushCommand("CheckHandles", TRUE);
    }
}

LogicLevel_GHdl ScriptManager_G::GetLogicLevel(S32 i_Index) {
    return m_LogicLevels[i_Index];
}

LogicLevel_GHdl ScriptManager_G::AddLogicLevel() {
    LogicLevel_GHdl l_LevelHdl = gData.ClassMgr->NewObject("LogicLevel_G");
    m_LogicLevels.Add(l_LevelHdl);
    return m_LogicLevels[m_LogicLevels.GetSize() - 1];
}

S32 ScriptManager_G::GetLogicLevelId(LogicLevel_GHdl i_Hdl) {
    for (S32 l_Index = 0; l_Index < m_LogicLevels.GetSize(); ++l_Index) {
        if (m_LogicLevels[l_Index] == i_Hdl) {
            return l_Index;
        }
    }
    return -1;
}

LogicLevel_GHdl ScriptManager_G::GetCurrentLogicLevel() {
    for (S32 l_Index = 0; l_Index < m_LogicLevels.GetSize(); ++l_Index) {
        if (m_LogicLevels[l_Index]->IsOpened()) {
            LogicLevel_GHdl l_NextLevelHdl = GetNextLogicLevel(m_LogicLevels[l_Index]);
            if (!l_NextLevelHdl) {
                return m_LogicLevels[l_Index];
            }
            if (!l_NextLevelHdl->IsOpened()) {
                return m_LogicLevels[l_Index];
            }
        }
    }
}

S32 ScriptManager_G::GetIdCharacter(Char* i_Name) {
    return 0;
}

void ScriptManager_G::PlayLevel(Char* i_LevelName, S32 i_PlayerCount, S32* i_CharacterIds, Char* i_MissionName, Char* i_StartName) {
}

void ScriptManager_G::RemoveAllMaterialLib(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < m_Levels.GetSize(); ++l_Index) {
        LevelData_GHdl l_LevelHdl = LevelData_GHdl(m_Levels[l_Index]);
        MaterialLib* l_MaterialLib = l_LevelHdl->GetMaterialLibLoaded(i_Name);
        if (l_MaterialLib) {
            l_LevelHdl->FreeMaterialLib(l_MaterialLib, FALSE);
        }
    }
}

void ScriptManager_G::ResetAdvancement() {
    m_AbilityFlag = 0;
    m_PlayerSaveStruct.Reset();

    for (S32 i = 0; i < m_Levels.GetSize(); ++i) {
        m_Levels[i]->ResetAdvancement();
    }

    ResetObjectTextInfo(m_CollectTutorialTextInfos);
    ResetObjectTextInfo(m_CarryTutorialTextInfos);
    CheckUnlock(TRUE);
}

S32 ScriptManager_G::GetNbPlayer() {
    if (!gData.GameMgr->GetNbGame()) {
        return 0;
    }
    return gData.GameMgr->GetGame(0)->GetNbPlayer();
}

Player_G* ScriptManager_G::GetMainPlayer(S32 i_PlayerId) {
    if (!gData.GameMgr) {
        return NULL;
    }
    if (i_PlayerId >= GetNbPlayer()) {
        return NULL;
    }
    for (S32 l_GameId = 0; l_GameId < gData.GameMgr->GetNbGame(); ++l_GameId) {
        Player_G* l_Player = (Player_G*)gData.GameMgr->GetGame(l_GameId)->GetPlayerAgent(i_PlayerId);
        if (l_Player) {
            return l_Player;
        }
    }
    return NULL;
}

void ScriptManager_G::CheckUnlock(Bool i_Force) {
    m_UnlockEvents.CheckUnlock(i_Force);
    MissionVolumeAgent_G::CheckAnims();
    ConditionVolumeAgent_G::CheckAllConditions();
    Mission_Global::UpdateValues();
}

void ScriptManager_G::CloneClassDone() {
    DynArray_Z<Name_Z, 32, FALSE, TRUE> l_Classes;
    gData.ClassMgr->GetCloneClass(Name_Z(Name_Z::GetID("IT_COLLECT_G")), l_Classes);
    S32 l_Index = 0;
    S32 l_Count = l_Classes.GetSize();
    for (; l_Index < l_Count; ++l_Index) {
        CloneClassTextInfo l_Info;
        l_Info.m_ClassName = l_Classes[l_Index];
        l_Info.m_Triggered = FALSE;
        m_CollectTutorialTextInfos.Add(l_Info);
    }
    gData.ClassMgr->GetCloneClass(Name_Z(Name_Z::GetID("IT_Carrying")), l_Classes);
    S32 l_CarryCount = l_Classes.GetSize();
    S32 l_CarryIndex = 0;
    for (; l_CarryIndex < l_CarryCount; ++l_CarryIndex) {
        const Name_Z& l_ClassName = l_Classes[l_CarryIndex];
        CloneClassTextInfo l_Info;
        l_Info.m_ClassName = l_ClassName;
        l_Info.m_Triggered = FALSE;
        m_CarryTutorialTextInfos.Add(l_Info);
    }
}

void ScriptManager_G::ResetObjectTextInfo(CloneClassTextInfoDA& io_TextInfos) {
    S32 l_Index = 0;
    S32 l_Count = io_TextInfos.GetSize();
    for (; l_Count > 0; --l_Count, ++l_Index) {
        io_TextInfos[l_Index].m_Triggered = FALSE;
    }
}

// $SABE: Never thought I'd commit an array of slurs to a repo, let's hope this is not against GitHub's TOS.
Char* arrayMotsPasJoli[] = {
    "ALA", "GAY", "SEX", "ANL", "GOD", "SHT", "ASS", "HOE", "SPC", "BCH", "HOR", "SPK", "BUM", "JAP", "SUC", "BUT", "JEW", "SUK", "CNT", "JIZ", "SUX", "COC", "KOC", "THC", "COX", "KOK", "TIT", "COK", "KKK", "VAG", "CUM", "KNT", "WHR", "DAM", "KYK", "WOP", "DIC", "LEZ", "XXX", "DIK", "LSD", "DIX", "NGR", "DYK", "NIG", "FAG", "NIP", "FOB", "NUT", "FUC", "PCP", "FUK", "PEE", "FUQ", "POO", "FUX", "PUS", "END"
};

void PlayerSaveStruct_G::Reset() {
    m_MaxHealth = PLAYER_DEFAULT_MAX_HEALTH;
    m_Health = PLAYER_DEFAULT_HEALTH;
    m_Lives = PLAYER_DEFAULT_LIVES;
}

Bool ScriptManager_G::IsMotPasJoli(Char* i_Text) {
    Char** l_Word = &arrayMotsPasJoli[0];
    while (stricmp(*l_Word, "END") != 0) {
        if (stricmp(i_Text, *l_Word) == 0) {
            return TRUE;
        }
        ++l_Word;
    }
    return FALSE;
}
