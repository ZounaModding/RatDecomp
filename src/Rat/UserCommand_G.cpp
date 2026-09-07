#include "UserCommand_G.h"

#include "Console_Z.h"
#include "BaseInGameDatas_G.h"
#include "ClassManager_Z.h"
#include "GameMgr_G.h"
#include "GameManager_Z.h"
#include "Language_Z.h"
#include "LevelData_G.h"
#include "LoadingDraw_G.h"
#include "MemoryCardMgr_G.h"
#include "MenuParser.h"
#include "Program_Z.h"
#include "ScriptManager_G.h"
#include "UnLock_G.h"
#include <stdlib.h>
#include <string.h>

void RegisterUserCommand() {
    REGISTERCOMMAND("SetGameLogicAgent", SetGameLogicAgent);
    REGISTERCOMMAND("CAMDebug", CAM_Debug);
    REGISTERCOMMAND("DebugMC", DebugMC);
    REGISTERCOMMAND("ActiveTeleport", ActiveTeleport);
    REGISTERCOMMAND("CameraMouseControl", CameraMouseControl);
    REGISTERCOMMAND("GotoDummyTeleport", GotoDummyTeleport);
    REGISTERCOMMAND("GotoDummyName", GotoDummyName);
    REGISTERCOMMAND("ConvertToQuat", ConvertToQuat);
    REGISTERCOMMAND("WaitEndRtc", WaitEndRtc);
    REGISTERCOMMAND("BackToMenu", BackToMenu);
    REGISTERCOMMAND("BlocFader", BlocFader);
    REGISTERCOMMAND("SetBlackScreen", SetBlackScreen);
    REGISTERCOMMAND("UnMuteSounds", UnMuteSounds);
    REGISTERCOMMAND("DisplayFollowSplines", DisplayFollowSplines);
    REGISTERCOMMAND("DebugWeaponCamera", DebugWeaponCamera);
    REGISTERCOMMAND("KillFade", KillFade);
    REGISTERCOMMAND("PauseTheDynamics", PauseDynamics);
    REGISTERCOMMAND("KillHelicopter", KillHelicopter);
    REGISTERCOMMAND("PauseConsole", PauseConsole);
    REGISTERCOMMAND("UnPauseFade", UnPauseFade);
    REGISTERCOMMAND("ReplacePLayer", ReplacePLayer);
    REGISTERCOMMAND("EndOfMission", EndOfMission);
    REGISTERCOMMAND("LoadMissionData", LoadMissionData);
    REGISTERCOMMAND("UnlockAll", UnlockAll);
    REGISTERCOMMAND("CheckAutoStart", CheckAutoStart);
    REGISTERCOMMAND("WinCurrentMission", WinCurrentMission);
    REGISTERCOMMAND("TeleportToMission", TeleportToMission);
    REGISTERCOMMAND("SetAutoCompletion", SetAutoCompletion);
    REGISTERCOMMAND("GetMemoryStats", GetMemoryStats);
    REGISTERCOMMAND("LoseCurrentMission", LoseCurrentMission);
    REGISTERCOMMAND("SEEStartedMission", SeeStartedMission);
    REGISTERCOMMAND("SEERunningMission", SeeRunningMission);
    REGISTERCOMMAND("InfoMissions", InfoMissions);
    REGISTERCOMMAND("ContinueAfterMission", ContinueAfterMission);
    REGISTERCOMMAND("MissionSoonToBeCleaned", MissionSoonToBeCleaned);
    REGISTERCOMMAND("STARTMission", StartMission);
    REGISTERCOMMAND("StartMissionVolume", StartMissionVolume);
    REGISTERCOMMAND("UpdateInterfaceToBeDraw", UpdateInterfaceToBeDraw);
    REGISTERCOMMAND("AskMenuSave", AskMenuSave);
    REGISTERCOMMAND("AskFailureMenu", AskFailureMenu);
    REGISTERCOMMAND("KillPlayer", KillPlayer);
    REGISTERCOMMAND("SeeEnemies", SeeEnemies);
    REGISTERCOMMAND("SWitchEnemies", SwitchEnemies);
    REGISTERCOMMAND("NoBackOmniInRtc", SetNoBackOmniInRtc);
    REGISTERCOMMAND("Save", Save);
    REGISTERCOMMAND("Load", Load);
    REGISTERCOMMAND("DisplaySurfaceBox", ShowSurfaceBox);
    REGISTERCOMMAND("CompleteObjectif", CompleteObjectif);
    REGISTERCOMMAND("DebugMenuBox", DebugMenuBox);
    REGISTERCOMMAND("SetBOrderMargin", SetBorderMargin);
    REGISTERCOMMAND("SayStartingDiaLoG", SayStartingDialog);
    REGISTERCOMMAND("NetConnect", CommandNetConnect);
    REGISTERCOMMAND("SetLoadingDraw", SetLoadingDraw);
    REGISTERCOMMAND("AddRTC", AddRTC);
    REGISTERCOMMAND("AddMISSION", AddMISSION);
    REGISTERCOMMAND("UnlockNeed", UnlockNeed);
    REGISTERCOMMAND("UnlockTT", UnlockTT);
    REGISTERCOMMAND("UnlockRTC", UnlockRTC);
    REGISTERCOMMAND("UnlockPlayMission", UnlockPlayMission);
    REGISTERCOMMAND("CheckUnlock", CheckUnlock);
    REGISTERCOMMAND("SetDefaultMissionValues", SetDefaultMissionValues);
    REGISTERCOMMAND("ForceUnLock", ForceUnLock);
    REGISTERCOMMAND("CLONEClassDone", CloneClassDone);
    REGISTERCOMMAND("AddTextInfos", AddTextInfos);
    REGISTERCOMMAND("VOID", VoidFunc);
    REGISTERCOMMAND("AddInGameTextInfos", AddInGameTextInfos);
    REGISTERCOMMAND("StartTUTORIAL", StartTUTORIAL);
    REGISTERCOMMAND("AddLevel", AddLevel);
    REGISTERCOMMAND("AddMaterialLib", AddMaterialLib);
    REGISTERCOMMAND("AddLevelRTC", AddLevelRTC);
    REGISTERCOMMAND("AddLevelMPEG", AddLevelMPEG);
    REGISTERCOMMAND("AddLevelMenu", AddLevelMenu);
    REGISTERCOMMAND("AddLevelDemoMenu", AddLevelDemoMenu);
    REGISTERCOMMAND("AddMpegMenu", AddMpegMenu);
    REGISTERCOMMAND("AddCharacter", AddCharacter);
    REGISTERCOMMAND("ChangeStartBase", ChangeStartBase);
    REGISTERCOMMAND("AddMUSIC", AddMusic);
    REGISTERCOMMAND("PlayLevel", PlayLevel);
    REGISTERCOMMAND("LAUNCHMission", LaunchMission);
    REGISTERCOMMAND("PlayLevelMulti", PlayLevelMulti);
    REGISTERCOMMAND("StackPlayRtc", StackPlayRtc);
    REGISTERCOMMAND("BlindageFadeAfterRTC", BlindageFadeAfterRTC);
    REGISTERCOMMAND("ChangeCurrentPerso", ChangeCurrentPerso);
    REGISTERCOMMAND("Show3DArrow", Show3DArrow);
    REGISTERCOMMAND("MakeAllBF", MakeAllBF);
    REGISTERCOMMAND("MakeRTCBF", MakeRTCBF);
    REGISTERCOMMAND("SetRtcFatherDummy", SetRtcFatherDummy);
    REGISTERCOMMAND("StartFadeToBlack", StartFadeToBlack);
    REGISTERCOMMAND("StartFadeFromBlack", StartFadeFromBlack);
    REGISTERCOMMAND("StartSTRIP", StartStrip);
    REGISTERCOMMAND("EndSTRIP", EndStrip);
    REGISTERCOMMAND("EnableNightmareDifficulty", EnableNightmareDifficulty);
    REGISTERCOMMAND("PlayerPP", PlayerPP);
    REGISTERCOMMAND("PersoSPEED", PersoSpeed);
    REGISTERCOMMAND("StartMENUDefinition", StartMENUDefinition);
    REGISTERCOMMAND("MENUDialog", MENUDialog);
    REGISTERCOMMAND("MENUButton", MENUButton);
    REGISTERCOMMAND("EndMENUDialog", EndMENUDialog);
    REGISTERCOMMAND("MENUButtonDesc", MENUButtonDesc);
    REGISTERCOMMAND("MENUButtonPictDesc", MENUButtonPictDesc);
    REGISTERCOMMAND("MENUButtonText", MENUButtonText);
    REGISTERCOMMAND("MENUStyleTextScroll", MENUStyleTextScroll);
    REGISTERCOMMAND("MENUButtonAlignVert", MENUButtonAlignVert);
    REGISTERCOMMAND("MENUButtonBox", MENUButtonBox);
    REGISTERCOMMAND("MENUStyleBoxCoinScale", MENUStyleBoxCoinScale);
    REGISTERCOMMAND("MENUButtonAnimateCyclicTRUE", MENUButtonAnimateCyclicTRUE);
    REGISTERCOMMAND("MENUButtonBITmap", MENUButtonBitmap);
    REGISTERCOMMAND("MENUButtonSelectable", MENUButtonSelectable);
    REGISTERCOMMAND("MENUButtonHidden", MENUButtonHidden);
    REGISTERCOMMAND("MENUButtonBLink", MENUButtonBlink);
    REGISTERCOMMAND("MENUStyleText", MENUStyleText);
    REGISTERCOMMAND("MENUStyleBox", MENUStyleBox);
    REGISTERCOMMAND("MENUStyleBITmapColor", MENUStyleBitmapColor);
    REGISTERCOMMAND("MENUStyleBITMAP", MENUStyleBitmap);
    REGISTERCOMMAND("MENUStyleBitmapDim", MENUStyleBitmapDim);
    REGISTERCOMMAND("MENUPlatform", MENUPlatform);
    REGISTERCOMMAND("MENUUpdate", MENUUpdate);
    REGISTERCOMMAND("MENUDEBug", MENUDEBug);
    REGISTERCOMMAND("MENUSTyleTextStruct", MENUSTyleTextStruct);
    REGISTERCOMMAND("MENUStyleTextEqual", MENUStyleTextEqual);
    REGISTERCOMMAND("MENUSurroundingBitMaps", MENUSurroundingBitMaps);
    REGISTERCOMMAND("MENUButtonSurroundingBitmaps", MENUButtonSurroundingBitmaps);
    REGISTERCOMMAND("MenuButtonMAJ", MenuButtonMAJ);
    REGISTERCOMMAND("MENUStateAnimation", MENUStateAnimation);
    REGISTERCOMMAND("MENUStyleCyclicAnimation", MENUStyleCyclicAnimation);
    REGISTERCOMMAND("MENUBoxAutoShrink", MENUBoxAutoShrink);
    REGISTERCOMMAND("EndMENURessourceParsing", EndMENURessourceParsing);
    REGISTERCOMMAND("RemoveAllDialogs", RemoveAllDialogs);
    REGISTERCOMMAND("LoadINPUT", LoadINPUT);
    REGISTERCOMMAND("InputDefAdd", InputDefAdd);
    REGISTERCOMMAND("RESETTextAdd", RESETTextAdd);
    REGISTERCOMMAND("RemapTextAdd", RemapTextAdd);
    REGISTERCOMMAND("DPlayRtc", DPlayRtc);
    REGISTERCOMMAND("CheatNoRtc", CheatNoRtc);
    REGISTERCOMMAND("DebugItemMgr", DebugItemMgr);
    REGISTERCOMMAND("DebugNmyMgr", DebugNmyMgr);
    REGISTERCOMMAND("DebugWhiteFade", DebugWhiteFade);
    REGISTERCOMMAND("NoFadeAndStrip", NoFadeAndStrip);
    REGISTERCOMMAND("SetLightLevel", SetLightLevel);
    REGISTERCOMMAND("SetLanguageAuto", SetLanguageAuto);
    REGISTERCOMMAND("ChoosePlayMovie", ChoosePlayMovie);
    REGISTERCOMMAND("PlayLevelMUSIC", PlayLevelMusic);
    REGISTERCOMMAND("InitLanguageMC", InitLanguageMC);
    REGISTERCOMMAND("DisplayLegalText", DisplayLegalText);
    REGISTERCOMMAND("INITPreLoadingRTC", InitPreloadingRTC);
    REGISTERCOMMAND("PreLoadingRTC", PreloadingRTC);
    REGISTERCOMMAND("StartPreLoadedRTC", StartPreloadedRTC);
    REGISTERCOMMAND("KeepPreLoadedRunningRTC", KeepPreloadedRunningRTC);
    REGISTERCOMMAND("SHutPreLoadedRTC", ShutPreloadedRTC);
    REGISTERCOMMAND("SHUTPreLoadingRTC", ShutPreloadingRTC);
    REGISTERCOMMAND("InitGameMgr", InitGameMgr);
    REGISTERCOMMAND("AddMultiGame", AddMultiGame);
    REGISTERCOMMAND("AddChampionShip", AddChampionShip);
    REGISTERCOMMAND("AddBriefingINFOS", AddBriefingINFOS);
    REGISTERCOMMAND("AddComboChampionship", AddComboChampionship);
    REGISTERCOMMAND("ADDLogicMission", ADDLogicMission);
    REGISTERCOMMAND("AddLogicLevel", AddLogicLevel);
    REGISTERCOMMAND("BuyAll", BuyAll);
    REGISTERCOMMAND("BeRich", BeRich);
    REGISTERCOMMAND("NOInterface", NOInterface);
    REGISTERCOMMAND("ADDLogicEndMission", ADDLogicEndMission);
    REGISTERCOMMAND("ADDLogicOpeningMission", ADDLogicOpeningMission);
    REGISTERCOMMAND("AddLangDefine", AddLangDefine);
    REGISTERCOMMAND("EnableDebugTools", EnableDebugTools);
    REGISTERCOMMAND("DisableDebugTools", DisableDebugTools);
    REGISTERCOMMAND("AddIngameDiaLoG", AddIngameDialog);
}

Bool SetGameLogicAgent() {
    if (gData.Cons->GetNbParam() != 3) {
        return FALSE;
    }

    Name_Z l_WorldClassName(Name_Z::GetID("WORLD_Z"));
    Name_Z l_WorldName(gData.Cons->GetStrParam(1));
    World_ZHdl l_WorldHdl(gData.ClassMgr->GetObjectByName(l_WorldName, l_WorldClassName));
    if (!l_WorldHdl) {
        return FALSE;
    }

    S32 l_GameId = gData.GameMgr->GetGameIdByWorld(l_WorldHdl);
    if (l_GameId < 0) {
        return FALSE;
    }

    Agent_ZHdl l_AgentHdl = gScriptMgr->NewAgent(gData.Cons->GetStrParam(2));
    LogicAgent_GHdl l_LogicAgentHdl = l_AgentHdl;
    gScriptMgr->AddLogicAgent(l_LogicAgentHdl, gData.GameMgr->GetGame(l_GameId));
    return TRUE;
}

Bool CAM_Debug() {
    return TRUE;
}

Bool CameraMouseControl() {
    return TRUE;
}

Bool ActiveTeleport() {
    return TRUE;
}

Bool DebugMC() {
    return TRUE;
}

Bool WaitEndRtc() {
    return TRUE;
}

Bool UnPauseFade() {
    return TRUE;
}

Bool BackToMenu() {
    return TRUE;
}

Bool BlocFader() {
    return TRUE;
}

Bool SetBlackScreen() {
    return TRUE;
}

Bool UnMuteSounds() {
    return TRUE;
}

Bool KillFade() {
    return TRUE;
}

Bool KillHelicopter() {
    return TRUE;
}

Bool PauseDynamics() {
    return TRUE;
}

Bool DebugWeaponCamera() {
    return TRUE;
}

Bool DisplayFollowSplines() {
    return TRUE;
}

Bool PauseConsole() {
    return TRUE;
}

Bool ConvertToQuat() {
    return TRUE;
}

Bool GotoDummyTeleport() {
    return TRUE;
}

Bool GotoDummyName() {
    return TRUE;
}

Bool DebugMenuBox() {
    return TRUE;
}

Bool SetBorderMargin() {
    return TRUE;
}

Bool ReplacePLayer() {
    return TRUE;
}

Bool EndOfMission() {
    return TRUE;
}

Bool LoadMissionData() {
    return TRUE;
}

Bool BuyAll() {
    return TRUE;
}

Bool AskMenuSave() {
    return TRUE;
}

Bool AskFailureMenu() {
    return TRUE;
}

Bool LoseCurrentMission() {
    return TRUE;
}

Bool StartMissionVolume() {
    return TRUE;
}

Bool UpdateInterfaceToBeDraw() {
    return TRUE;
}

Bool StartMission() {
    return TRUE;
}

Bool SeeRunningMission() {
    return TRUE;
}

Bool SeeStartedMission() {
    return TRUE;
}

Bool InfoMissions() {
    return TRUE;
}

Bool ContinueAfterMission() {
    return TRUE;
}

Bool MissionSoonToBeCleaned() {
    return TRUE;
}

Bool SetAutoCompletion() {
    return TRUE;
}

Bool GetMemoryStats() {
    return TRUE;
}

Bool TeleportToMission() {
    return TRUE;
}

Bool WinCurrentMission() {
    return TRUE;
}

Bool KillPlayer() {
    return TRUE;
}

Bool SetNoBackOmniInRtc() {
    return TRUE;
}

Bool SwitchEnemies() {
    return TRUE;
}

Bool SeeEnemies() {
    return TRUE;
}

Bool CheckAutoStart() {
    return TRUE;
}

Bool UnlockAll() {
    return TRUE;
}

Bool Save() {
    return TRUE;
}

Bool Load() {
    return TRUE;
}

Bool ShowSurfaceBox() {
    return TRUE;
}

Bool DPlayRtc() {
    return TRUE;
}

Bool CompleteObjectif() {
    return TRUE;
}

Bool CheatNoRtc() {
    return TRUE;
}

Bool DebugItemMgr() {
    return TRUE;
}

Bool SetLightLevel() {
    return TRUE;
}

Bool DebugNmyMgr() {
    return TRUE;
}

Bool DebugWhiteFade() {
    return TRUE;
}

Bool NoFadeAndStrip() {
    return TRUE;
}

Bool SayStartingDialog() {
    return TRUE;
}

Bool CommandNetConnect() {
    return TRUE;
}

Bool DisplayLegalText() {
    return TRUE;
}

Bool SetLanguageAuto() {
    LanguageEnum_Z l_Language = GetLanguage();
    langDefineDA& l_Defines = gScriptMgr->GetLangDefines();
    for (S32 i = 0; i < l_Defines.GetSize(); ++i) {
        langDefine& l_Define = l_Defines[i];
        if (l_Language == l_Define.m_Lang) {
            SetLanguage(l_Define.m_TrTextId, l_Define.m_DialogId, l_Define.m_MpegId);
            return TRUE;
        }
    }
    langDefine& l_Define = l_Defines[0];
    SetLanguage(l_Define.m_TrTextId, l_Define.m_DialogId, l_Define.m_MpegId);
    return TRUE;
}

Bool ChoosePlayMovie() {
    return TRUE;
}

Bool PlayLevelMusic() {
    return TRUE;
}

Bool InitLanguageMC() {
    gScriptMgr->GetMcMgr()->InitLanguageMC();
    return TRUE;
}

Bool SetLoadingDraw() {
    if (strcmp(gData.Cons->GetStrParam(1), "OFF") == 0) {
        gScriptMgr->GetLoadingDraw()->StopAnimLoading(FALSE);
    }
    else {
        gScriptMgr->GetLoadingDraw()->StopAnimLoading(TRUE);
    }
    return TRUE;
}

Bool UnlockPlayMission() {
    return TRUE;
}

Bool UnlockRTC() {
    return TRUE;
}

Bool UnlockTT() {
    return TRUE;
}

Bool UnlockNeed() {
    gScriptMgr->GetUnlockEvents().AddUnlockEvent(gData.Cons);
    return TRUE;
}

Bool VoidFunc() {
    return TRUE;
}

Bool AddTextInfos() {
    return TRUE;
}

Bool AddInGameTextInfos() {
    return TRUE;
}

Bool StartTUTORIAL() {
    return TRUE;
}

Bool CloneClassDone() {
    gScriptMgr->CloneClassDone();
    return TRUE;
}

Bool ForceUnLock() {
    return TRUE;
}

Bool CheckUnlock() {
    gScriptMgr->CheckUnlock(TRUE);
    return TRUE;
}

Bool SetDefaultMissionValues() {
    return TRUE;
}

Bool AddMISSION() {
    return TRUE;
}

Bool AddRTC() {
    return TRUE;
}

Bool AddCharacter() {
    return TRUE;
}

Bool AddMaterialLib() {
    return TRUE;
}

Bool AddLevel() {
    return TRUE;
}

Bool AddLevelMPEG() {
    return TRUE;
}

Bool AddLevelRTC() {
    LevelData_GHdl l_LevelDataHdl = gScriptMgr->GetLevelData(gData.Cons->GetStrParam(1));
    if (l_LevelDataHdl) {
        l_LevelDataHdl->SetRTCs(gData.Cons->GetStrParam(2), gData.Cons->GetStrParam(3));
    }
    return TRUE;
}

Bool AddMpegMenu() {
    MenuMpegTextDA& l_MpegTexts = gScriptMgr->GetMpegTexts();
    S32 l_Index = l_MpegTexts.Add();
    strcpy((Char*)l_MpegTexts[l_Index].m_Data, gData.Cons->GetStrParam(1));
    return TRUE;
}

Bool AddLevelDemoMenu() {
    return TRUE;
}

Bool AddLevelMenu() {
    return TRUE;
}

Bool AddMultiGame() {
    return TRUE;
}

Bool AddChampionShip() {
    return TRUE;
}

Bool AddBriefingINFOS() {
    return TRUE;
}

Bool AddComboChampionship() {
    return TRUE;
}

Bool AddMusic() {
    return TRUE;
}

Bool ChangeStartBase() {
    return TRUE;
}

Bool PlayLevelMulti() {
    return TRUE;
}

Bool PlayLevel() {
    S32 l_CharacterIds[4];
    if (gData.Cons->GetNbParam() == 2) {
        gScriptMgr->PlayLevel(strupr(gData.Cons->GetStrParam(1)), 0, NULL, NULL, NULL);
    }
    else {
        l_CharacterIds[0] = gScriptMgr->GetIdCharacter(strupr(gData.Cons->GetStrParam(2)));
        Console_Z* l_Console = gData.Cons;
        S32 l_Count = gData.Cons->GetNbParam();
        if (l_Count == 3) {
            gScriptMgr->PlayLevel(strupr(gData.Cons->GetStrParam(1)), 1, l_CharacterIds, NULL, NULL);
        }
        else if (l_Count == 4) {
            gScriptMgr->PlayLevel(strupr(gData.Cons->GetStrParam(1)), 1, l_CharacterIds, l_Console->GetStrParam(3), NULL);
        }
        else if (l_Count == 5) {
            gScriptMgr->PlayLevel(strupr(gData.Cons->GetStrParam(1)), 1, l_CharacterIds, l_Console->GetStrParam(3), l_Console->GetStrParam(4));
        }
    }
    return TRUE;
}

Bool LaunchMission() {
    return TRUE;
}

Bool StackPlayRtc() {
    return TRUE;
}

Bool BlindageFadeAfterRTC() {
    return TRUE;
}

Bool Show3DArrow() {
    return TRUE;
}

Bool ChangeCurrentPerso() {
    return TRUE;
}

Bool MakeAllBF() {
    return TRUE;
}

Bool MakeRTCBF() {
    return TRUE;
}

Bool EnableNightmareDifficulty() {
    return TRUE;
}

Bool PersoSpeed() {
    return TRUE;
}

Bool PlayerPP() {
    return TRUE;
}

Bool EndStrip() {
    return TRUE;
}

Bool StartStrip() {
    return TRUE;
}

Bool StartFadeFromBlack() {
    gScriptMgr->GetInGameDatas()->StartFadeFromBlack(0.3f);
    return TRUE;
}

Bool StartFadeToBlack() {
    return TRUE;
}

Bool SetRtcFatherDummy() {
    return TRUE;
}

Bool InitPreloadingRTC() {
    return TRUE;
}

Bool PreloadingRTC() {
    return TRUE;
}

Bool StartPreloadedRTC() {
    return TRUE;
}

Bool KeepPreloadedRunningRTC() {
    return TRUE;
}

Bool ShutPreloadedRTC() {
    return TRUE;
}

Bool ShutPreloadingRTC() {
    return TRUE;
}

Bool InitGameMgr() {
    return gScriptMgr->GetCTFGameMgr().Init();
}

Bool ADDLogicMission() {
    return TRUE;
}

Bool AddLogicLevel() {
    return TRUE;
}

Bool ADDLogicEndMission() {
    return TRUE;
}

Bool ADDLogicOpeningMission() {
    return TRUE;
}

Bool NOInterface() {
    return TRUE;
}

Bool BeRich() {
    return TRUE;
}

Bool AddLangDefine() {
    if (gData.Cons->GetNbParam() != 6) {
        return FALSE;
    }

    LanguageEnum_Z l_Language = LANG_NONE;
    if (!strcmp(gData.Cons->GetStrParam(5), "JAPANESE")) {
        l_Language = LANG_JAPANESE_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "ENGLISH")) {
        l_Language = LANG_ENGLISH_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "FRENCH")) {
        l_Language = LANG_FRENCH_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "SPANISH")) {
        l_Language = LANG_SPANISH_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "GERMAN")) {
        l_Language = LANG_GERMAN_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "ITALIAN")) {
        l_Language = LANG_ITALIAN_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "DUTCH")) {
        l_Language = LANG_DUTCH_Z;
    }
    if (!strcmp(gData.Cons->GetStrParam(5), "PORTUGUESE")) {
        l_Language = LANG_PORTUGUESE_Z;
    }

    S32 l_MpegId = atoi(gData.Cons->GetStrParam(4));
    S32 l_DialogId = atoi(gData.Cons->GetStrParam(3));
    S32 l_TrTextId = atoi(gData.Cons->GetStrParam(2));
    langDefine l_Define;
    l_Define.m_LangNameTrTextId = atoi(gData.Cons->GetStrParam(1));
    l_Define.m_TrTextId = l_TrTextId;
    l_Define.m_DialogId = l_DialogId;
    l_Define.m_MpegId = l_MpegId;
    l_Define.m_Lang = l_Language;
    gScriptMgr->GetArrayLang().AddLangDefine(l_Define);
    return TRUE;
}

Bool EnableDebugTools() {
    gScriptMgr->SetEnableDebugTools(TRUE);
    return TRUE;
}

Bool DisableDebugTools() {
    return TRUE;
}

Bool AddIngameDialog() {
    Name_Z l_Name(gData.Cons->GetStrParam(1));
    Float l_Param1 = atof(gData.Cons->GetStrParam(2));
    Float l_Param2 = atof(gData.Cons->GetStrParam(3));
    Bool l_Flag = atof(gData.Cons->GetStrParam(4)) > 0.0;
    Float l_Param3 = atof(gData.Cons->GetStrParam(5));
    Float l_Param4 = atof(gData.Cons->GetStrParam(6));
    S32 l_DialogGroupId = gScriptMgr->AddInGameDialog(l_Name, l_Param1, l_Param2, l_Param3, l_Param4, l_Flag);
    for (S32 i = 7; i < gData.Cons->GetNbParam(); ++i) {
        gScriptMgr->AddTTDialog(l_DialogGroupId, atoi(gData.Cons->GetStrParam(i)));
    }
    return TRUE;
}

#undef COMMAND_STUB
