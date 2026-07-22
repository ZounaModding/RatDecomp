#ifndef _SCRIPTMANAGER_G_H_
#define _SCRIPTMANAGER_G_H_

#include "ABC_ScriptManager.h"
#include "ActionHelper_GHdl.h"
#include "BaseInGameDatas_GHdl.h"
#include "BriefInfo.h"
#include "CharsData_G.h"
#include "DialogGroup_G.h"
#include "CreaturesManager_GHdl.h"
#include "EnemyGenerator_GHdl.h"
#include "FootPrints_GHdl.h"
#include "Game_ParticleMgr_GHdl.h"
#include "Game_SoundMgr_GHdl.h"
#include "GameMgr_G.h"
#include "InGameFX_G.h"
#include "InGameFX_GHdl.h"
#include "IT_ConditionHdl.h"
#include "ItemMgr_GHdl.h"
#include "LangDefine.h"
#include "LevelAgent_GHdl.h"
#include "LevelData_GHdl.h"
#include "LevelMenu_G.h"
#include "LiquidFlow_GHdl.h"
#include "LoadingDraw_GHdl.h"
#include "LogicAgent_GHdl.h"
#include "LogicLevelHdl.h"
#include "MemoryCardMgr_GHdl.h"
#include "MissionManager_GHdl.h"
#include "MusicManager_GHdl.h"
#include "Omni_ZHdl.h"
#include "PaddleChecker_GHdl.h"
#include "PermanentDrawHdl.h"
#include "PersoLight_GHdl.h"
#include "PhysicWorld_GHdl.h"
#include "PointJump_GHdl.h"
#include "Player_GHdl.h"
#include "ProGounrMgr_GHdl.h"
#include "Ropes_GHdl.h"
#include "Smell_GHdl.h"
#include "TextGameDraw_GHdl.h"
#include "UnLock_G.h"
#include "VibrationManager_GHdl.h"
#include "WanderingPath_GHdl.h"

class CreaturesMove_G;
class GameLogicAgent_G;
class MissionDef_G;
class Node_Z;
class Player_G;
class RtcScriptAgent_G;

#define PLAYER_DEFAULT_MAX_HEALTH 3
#define PLAYER_DEFAULT_HEALTH PLAYER_DEFAULT_MAX_HEALTH
#define PLAYER_DEFAULT_LIVES 5

#define SPECIAL_VISION_COLOR_1_R 0.4f
#define SPECIAL_VISION_COLOR_1_G 0.1f
#define SPECIAL_VISION_COLOR_1_B 0.5f
#define SPECIAL_VISION_COLOR_2_INTENSITY 0.4f
#define SPECIAL_VISION_HIGHLIGHT_R 1.0f
#define SPECIAL_VISION_HIGHLIGHT_G 1.0f
#define SPECIAL_VISION_HIGHLIGHT_B 0.0f

struct CloneClassTextInfo {
    Name_Z m_ClassName;
    Bool m_Triggered;
};

typedef DynArray_Z<CloneClassTextInfo, 2> CloneClassTextInfoDA;

class PlayerSaveStruct_G {
public:
    void Reset();
    void IncreaseHealth(S32 i_Amount);
    Bool IsPlayerFullHealth();
    void SetHealthMax(S32 i_MaxHealth);
    void IncreaseHealthMax(S32 i_Amount);
    void DecreaseHealth(S32 i_Amount);
    void DecreaseLife(S32 i_Amount);
    Bool IsPlayerDead();

private:
    S32 m_Health;
    S32 m_MaxHealth;
    S32 m_Lives;
};

class ScriptManager_G : public ABC_ScriptManager {
public:
    ScriptManager_G();
    virtual ~ScriptManager_G();
    virtual void Init();
    virtual void Shut();
    virtual void MarkHandles();
    virtual void ReadEnumFromFiles();
    virtual void NoteTrackInterpMessage(StaticArray_Z<Param_Z, 16, 1, 1>& i_Params, Message_Z& i_Message);
    virtual U32 MateriaInterpColFlag(const Char* i_Flag);
    virtual U32 MateriaInterpObjFlag(const Char* i_Flag);
    virtual U32 MateriaRemoveColFlag(const Char* i_Flag);
    virtual void GameSet(const Game_ZHdl& i_GameHdl);
    virtual void GameAgentSet(const Game_ZHdl& i_GameHdl);
    virtual void RemoveGame(const Game_ZHdl& i_GameHdl);
    virtual void GameReseted(const Game_ZHdl& i_GameHdl);
    virtual void ActivateGame(const Game_ZHdl& i_GameHdl);
    virtual void UpdateIndependentResources(const World_ZHdl& i_WorldHdl);
    virtual void InterpKeyframeMsg(const RegMessage_Z& i_Message);
    virtual void StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);
    virtual void StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);
    virtual void StreamDone(const Game_ZHdl& i_GameHdl, S32 i_StreamId);
    virtual void Update(Float i_DeltaTime);
    virtual Bool Minimize();

    virtual void SetPlayer(S32 i_PlayerId, ABC_AgentHdl i_PlayerHdl) { }

    virtual ABC_AgentHdl GetPlayer(S32 i_PlayerId) { return ABC_AgentHdl(); }

    virtual void ResetGame(Game_ZHdl& i_GameHdl) { }

    // $SABE: Param names might be bs, done by codex
    void AddLogicAgent(const LogicAgent_GHdl& i_LogicAgentHdl, const Game_ZHdl& i_GameHdl);
    void RemoveLogicAgent(const Game_ZHdl& i_GameHdl);
    LogicAgent_GHdl GetLogicAgent(const Game_ZHdl& i_GameHdl);
    RtcScriptAgent_G* GetRtcScriptAgent(S32 i_Index);
    GameLogicAgent_G* GetGameLogicAgent();
    void PlayVibration(const Player_GHdl& i_PlayerHdl, S32 i_VibrationId, U8 i_Strength);
    void StopVibration(const Player_GHdl& i_PlayerHdl);
    void StopAllVibrations();
    void DeactivateVibrations();
    void ActivateVibrations();
    Bool AreVibrationsEnabled();
    MissionDef_G* GetMission(Char* i_LevelName, Char* i_MissionName);
    LevelData_GHdl AddLevelData();
    S32 GetLevelRtc(Char* i_RtcName, Char* i_LevelName);
    LevelData_GHdl GetMenuLevelData(S32 i_Index);
    LevelData_GHdl GetLevelDataForMission(MissionDef_G* i_Mission);
    S32 GetLevelDataIdForMission(MissionDef_G* i_Mission);
    S32 GetLevelDataId(LevelData_GHdl i_LevelDataHdl);
    LevelData_GHdl GetLevelData(Char* i_LevelName);
    void RemoveOldLevel();
    LogicLevel_GHdl GetLogicLevel(S32 i_Index);
    LogicLevel_GHdl GetNextLogicLevel(LogicLevel_GHdl i_LogicLevelHdl);
    LogicLevel_GHdl GetLastLogicLevel(LogicLevel_GHdl i_LogicLevelHdl);
    LogicLevel_GHdl AddLogicLevel();
    LogicLevel_GHdl GetLogicLevel(Char* i_LevelName);
    S32 GetLogicLevelId(LogicLevel_GHdl i_LogicLevelHdl);
    LogicLevel_GHdl GetCurrentLogicLevel();
    LogicLevel_GHdl GetLocalLogicLevel();
    LogicLevel_GHdl GetLogicLevelForLevelData(LevelData_GHdl i_LevelDataHdl);
    LogicLevel_GHdl GetLogicLevelForMission(MissionDef_G* i_Mission);
    S32 GetIdCharacter(Name_Z& i_Name);
    S32 GetIdCharacter(Char* i_Name);
    void RemoveCharacter(S32 i_CharacterId);
    void RemoveAllPlayableCharacters();
    void LoadCharacter(S32 i_CharacterId, Bool i_LoadRtc, Bool i_LoadObjectBank);
    void PlayLevel(Char* i_LevelName, S32 i_PlayerCount, S32* i_CharacterIds, Char* i_MissionName, Char* i_StartName);
    Bool IsCurrentPerso(Char* i_Name);
    void ChangePerso(Char* i_Name, Char* i_StartName);
    void MakeAllLevelsBF(Bool i_IncludeRtc, Bool i_SharedOnly, S32 i_CharacterId);
    void MakeRTCBF(Char* i_RtcName);
    void PushCommandToLoadLevel(LevelData_GHdl i_LevelDataHdl, S32 i_PlayerCount, S32* i_CharacterIds);
    void RemoveAllMaterialLib(const Name_Z& i_Name);
    void UnlockAll();
    void ResetAdvancement();
    Float GetLogicAdvancement();
    S32 GetNbPlayer();
    Player_G* GetMainPlayer(S32 i_PlayerId);
    void CheckUnlock(Bool i_Force);
    void CloneClassDone();
    void DisplayObjectTextInfo(const Name_Z& i_ClassName, S32 i_TextId, Float i_Duration, CloneClassTextInfoDA& io_TextInfos);
    void ResetObjectTextInfo(CloneClassTextInfoDA& io_TextInfos);
    BaseObject_ZHdl GetGusteauMove();
    void PlayMenuSound(S32 i_SoundId);
    void StopMenuSound(S32 i_SoundId);
    void PlayMenuSoundNoRepeat(S32 i_SoundId);
    void UpdateInterfaceToBeDrawn();
    void GetMemoryStats();
    Bool IsGusteauAvailable();
    MultiGame* GetMultiGame(Name_Z i_Name);
    Championship* GetChampionship(Name_Z i_Name);
    Bool IsMotPasJoli(Char* i_Text);
    void AddInGameDialog(Name_Z& i_Name, Float i_Param1, Float i_Param2, Float i_Param3, Float i_Param4, Bool i_Flag);
    void AddTTDialog(S32 i_DialogGroupId, S32 i_TextId);

    void ParseAIHierarchy(Node_Z* i_Node);
    void ParseHierarchy(const Node_ZHdl& i_NodeHdl, Bool i_Add);
    void GetAIDummies(const Game_ZHdl& i_GameHdl);
    void RemoveAIDummies(const Game_ZHdl& i_GameHdl);
    void CheckNodeOnAIDummies(const Node_ZHdl& i_NodeHdl);
    void CreateOmniForFX(const Game_ZHdl& i_GameHdl);
    void DeleteOmniForFX();
    void UpdateOmnis(Float i_DeltaTime);
    void UpdatePlatForms(Float i_DeltaTime);
    Node_ZHdl RetrieveNode(Name_Z i_Name);
    void SetLevelObjectsFromSave();

    CTFGameMgr_G& GetCTFGameMgr() { return m_ScriptGameMgr; }

    ArrayLang& GetArrayLang() { return m_ArrayLang; }

    BaseInGameDatas_GHdl& GetInGameDatas() { return m_InGameDatasHdl; }

    LevelData_GHdl GetCurrentLevelData() { return m_CurrentLevelHdl; }

    LevelAgent_GHdl& GetLevelAgent() { return m_LevelAgentHdl; }

    LoadingDraw_CHdl& GetLoadingDraw() { return m_LoadingDrawHdl; }

    MissionManager_GHdl& GetMissionManager() { return m_MissionManagerHdl; }

    MemoryCardManager_CHdl& GetMcMgr() { return m_MemoryCardMgrHdl; }

    MusicManager_GHdl& GetMusicMgr() { return m_MusicMgrHdl; }

    PaddleChecker_GHdl& GetPaddleMgr() { return m_PaddleCheckerHdl; }

    void IncreaseHealthMax(S32 i_Amount) { m_PlayerSaveStruct.IncreaseHealthMax(i_Amount); }

private:
    BriefData m_BriefData;
    CTFGameMgr_G m_ScriptGameMgr;
    LogicAgent_GHdlDA m_LogicAgents;
    LevelAgent_GHdl m_LevelAgentHdl;
    MemoryCardManager_CHdl m_MemoryCardMgrHdl;
    CloneClassTextInfoDA m_CollectTutorialTextInfos;
    CloneClassTextInfoDA m_CarryTutorialTextInfos;
    PaddleChecker_GHdl m_PaddleCheckerHdl;
    VibrationManager_CHdl m_VibrationMgrHdl;
    MusicManager_GHdl m_MusicMgrHdl;
    ArrayLang m_ArrayLang;
    LoadingDraw_CHdl m_LoadingDrawHdl;
    PermanentDrawHdl m_PermanentDrawHdl;
    EnemyGenerator_GHdl m_EnemyGeneratorMgrHdl;
    TextGameDraw_GHdl m_TextGameDrawMgrHdl;
    ActionHelper_GHdl m_ActionHelperMgrHdl;
    FootPrints_GHdl m_FootPrintsMgrHdl;
    Ropes_GHdl m_RopesMgrHdl;
    Smell_GHdl m_SmellMgrHdl;
    LiquidFlow_GHdl m_LiquidFlowMgrHdl;
    IT_ConditionHdl m_ConditionMgrHdl;
    WanderingPath_GHdl m_WanderingPathMgrHdl;
    PointJump_GHdl m_PointJumpMgrHdl;
    InGameFX_GHdl m_InGameFXMgrHdl;
    PersoLight_GHdl m_PersoLightMgrHdl;
    BaseObject_ZHdl m_UnkHdl_0x663c;
    Omni_ZHdl m_OmniHdlForFX;
    Node_ZHdl m_OmniNodeHdlForFX;
    FXOmniDA m_FXOmnis;
    Node_ZHdlDA m_AIDummies_0x6650;
    Node_ZHdlDA m_AIDummies_0x6658;
    Float m_SynchedPlatformsTime;
    MissionManager_GHdl m_MissionManagerHdl;
    S8 m_PreloadedRtcNum;
    Float m_UnkFloatZero_0x666c;
    Bool m_EnableDebugTools;
    Char m_MovieToBePlayed[16];
    BaseInGameDatas_GHdl m_InGameDatasHdl;
    ProGroundMgr_GHdl m_ProGroundMgrHdl;
    Game_SoundMgrHdl m_GameSoundMgrHdl;
    GameParticleMgr_GHdl m_GameParticleMgrHdl;
    ItemMgr_GHdl m_ItemMgrHdl;
    PhysicWorld_GHdl m_PhysicWorldHdl;
    CreaturesMgr_GHdl m_CreaturesMgrHdl;
    DialogGroup_GHdlDA m_DialogGroups;
    UnLockEvents_G m_UnlockEvents;
    LevelData_GHdlDA m_Levels;
    LogicLevel_GHdlDA m_LogicLevels;
    CharsData_GHdlDA m_Characters;
    LevelMenu_GDA m_LevelSelectData;
    LevelDemoMenu_GDA m_DemoLevelSelectData;
    MenuMpegTextDA m_MpegTexts;
    MultiGameDA m_MultiplayerMinigames;
    ChampionshipDA m_Championships;
    LevelData_GHdl m_CurrentLevelHdl;
    PlayerSaveStruct_G m_PlayerSaveStruct;
    S32 m_AbilityFlag;
    S32 m_CookBfStartId;
    void* m_PackedMenuCommandBuffer;
    S32 m_PackedMenuCommandBufferSize;
};

#define gScriptMgr ((ScriptManager_G*)gData.ScriptMgr)

#endif // _SCRIPTMANAGER_G_H_
