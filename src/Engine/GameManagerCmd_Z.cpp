#include "GameManager_Z.h"
#include "ClassManager_Z.h"
#include "Language_Z.h"
#include "Console_Z.h"
#include "Parameters_Z.h"
#include "DebugTools_Z.h"
#include "Memory_Z.h"
#include "Program_Z.h"
#include "Console_Z.h"
#include "Language_Z.h"
#include "Renderer_Z.h"
#include "LodAgent_Z.h"

void RegisterGameMgrCommand() {
    RegisterGameCommand();
    REGISTERCOMMANDC("SetGame", SetMultiGame, " WorldName NbPlayers [AgentCameraClassName=CameraGame]");
    REGISTERCOMMANDC("ActivateGame", ActivateGame, " WorldName");
    REGISTERCOMMANDC("DeactivateGame", DeactivateGame, " WorldName");
    REGISTERCOMMANDC("RemoveGame", RemoveGame, " WorldName ");
    REGISTERCOMMANDC("SetMonoGame", SetMonoGame, " WorldName NbPlayers [AgentCameraClassName=CameraGame]");
    REGISTERCOMMANDC("SwitchGameToMono", SwitchGameToMono, " WorldName");
    REGISTERCOMMANDC("SwitchGameToMUlti", SwitchGameToMulti, " WorldName NbVp");
    REGISTERCOMMANDC("AddGamePlayer", AddGamePlayer, " WorldName PlayerId(1-MAX_NUMBER_OF_PLAYERS) PlayerName [TeamId=0]");
    REGISTERCOMMANDC("AddMenuPlayer", AddMenuPlayer, " WorldName PlayerId(1-MAX_NUMBER_OF_PLAYERS) PlayerName [TeamId=0]");
    REGISTERCOMMANDC("ReMoveGamePlayer", RemoveGamePlayer, " WorldName PlayerId(1-MAX_NUMBER_OF_PLAYERS)");
    REGISTERCOMMANDC("REsetGame", ResetGame, " VpId(1-MAX_VIEWPORT)");
    REGISTERCOMMANDC("DeactivateGamePlayer", DeactivateGamePlayer, " WorldName PlayerId(1-MAX_NUMBER_OF_PLAYERS)");
    REGISTERCOMMANDC("ACtivateGamePlayer", ActivateGamePlayer, " WorldName PlayerId(1-MAX_NUMBER_OF_PLAYERS)");
    REGISTERCOMMANDC("PlayerGotoCoord", PlayerGoto, "  X Y Z Viewport");
    REGISTERCOMMANDC("SwitchCameraUser", SwitchCameraUser, " ViewPortId");
    REGISTERCOMMANDC("SetPersoCamera", SetPersoCamera, " ViewPortId");
    REGISTERCOMMANDC("SetRTC", SetRtc, " WorldName RtcNb");
    REGISTERCOMMANDC("LoadRTC", LoadRtc, " WorldName RtcId RtcName [AgentRtcClassName=RtcGame]");
    REGISTERCOMMANDC("PlayRTC", PlayRtc, " WorldName RtcId [ViewPortId]");
    REGISTERCOMMANDC("SHutRTC", ShutRtc, " WorldName RtcId");
    REGISTERCOMMAND("TransText", AddTransText);
    REGISTERCOMMAND("SetLanguage", SetLanguage);
    REGISTERCOMMAND("FreeLanguage", FreeLanguage);
    REGISTERCOMMAND("PutParameter", AddParameter);
    REGISTERCOMMAND("ReadParameters", ReadParameters);
    REGISTERCOMMAND("MarkMEMory", MarkMemory);
    REGISTERCOMMAND("ShowUnmarkedMEMory", ShowUnmarkedMemory);
}

Bool SetMultiGame() {
    return SetGame(FALSE);
}

Bool SetMonoGame() {
    return SetGame(TRUE);
}

// TODO: Finish matching
Bool SetGame(Bool i_IsMono) {
    if (gData.Cons->GetNbParam() < 3 || !gData.Cons->IsParamFloat(2)) {
        return FALSE;
    }

    const Name_Z& l_WorldClassName = Name_Z(Name_Z::GetID("WORLD"));
    World_ZHdl l_WorldHdl = gData.ClassMgr->GetObjectByName(Name_Z(gData.Cons->GetParamStr(1)), l_WorldClassName);
    S32 l_PlayerCount = (S32)gData.Cons->GetParamFloat(2);

    if (!l_WorldHdl.IsValid()) {
        return FALSE;
    }
    if (l_PlayerCount < 1 || l_PlayerCount > MAX_NUMBER_OF_PLAYERS || l_PlayerCount > MAX_VIEWPORT) {
        return FALSE;
    }

    if (gData.Cons->GetNbParam() == 4) {
        Name_Z l_CameraAgentClass(gData.Cons->GetParamStr(3));
        gData.GameMgr->AddGame(l_WorldHdl, gData.Cons->GetParamStr(1), l_PlayerCount, i_IsMono, l_CameraAgentClass);
    }
    else {
        gData.GameMgr->AddGame(l_WorldHdl, gData.Cons->GetParamStr(1), l_PlayerCount, i_IsMono, Name_Z(Name_Z::GetID("CameraAgent_Z")));
    }

    return TRUE;
}

Bool AddGamePlayer() {
    return AddPlayer(FALSE);
}

Bool AddMenuPlayer() {
    return TRUE;
}

Bool AddPlayer(Bool i_IsMenu) {
    return TRUE;
}

Bool RemoveGamePlayer() {
    return TRUE;
}

Bool ActivateGamePlayer() {
    return TRUE;
}

Bool DeactivateGamePlayer() {
    return TRUE;
}

Bool SwitchGameToMono() {
    return TRUE;
}

Bool SwitchGameToMulti() {
    return TRUE;
}

// TODO: Finish matching
Bool ActivateGame() {
    if (gData.Cons->GetNbParam() != 2) {
        return FALSE;
    }

    const Name_Z& l_WorldClassName = Name_Z(Name_Z::GetID("WORLD"));
    World_ZHdl l_WorldHdl = gData.ClassMgr->GetObjectByName(Name_Z(gData.Cons->GetParamStr(1)), l_WorldClassName);
    if (!l_WorldHdl.IsValid()) {
        return FALSE;
    }

    S32 l_GameId = gData.GameMgr->GetGameIdByWorld(l_WorldHdl);
    if (l_GameId < 0) {
        return FALSE;
    }

    gData.GameMgr->ActivateGame(l_GameId);
    return TRUE;
}

Bool DeactivateGame() {
    return TRUE;
}

Bool SwitchCameraUser() {
    return TRUE;
}

Bool SetPersoCamera() {
    return TRUE;
}

Bool ResetGame() {
    return TRUE;
}

Bool PlayerGoto() {
    return TRUE;
}

Bool RemoveGame() {
    return TRUE;
}

Bool SetRtc() {
    return TRUE;
}

Bool PlayRtc() {
    return TRUE;
}

Bool ShutRtc() {
    return TRUE;
}

Bool LoadRtc() {
    return TRUE;
}

void GameManager_Z::DebugDisplay(Viewport_Z* i_Vp) {
    if (!(gData.m_EngineFlag & FL_POS_PERSO)) {
        return;
    }

    Renderer_Z* l_Renderer = i_Vp->GetRenderer();
    Float l_AddY[MAX_VIEWPORT];
    S32 i;
    for (i = 0; i < MAX_VIEWPORT; i++) {
        l_AddY[i] = 0.0f;
    }

    for (i = 0; i < GetNbGame(); i++) {
        Game_Z* l_Game = GetGame(i);
        if (l_Game->GetFirstVp() < 0) {
            continue;
        }

        S32 j;
        for (j = 0; j < l_Game->GetNbPlayer(); j++) {
            if (!l_Game->GetPlayerAgent(j)) {
                continue;
            }

            LodAgent_Z* l_Agent = l_Game->GetPlayerAgent(j);
            S32 l_ViewportId = l_Agent->GetViewportId();
            if (l_ViewportId < 0 || l_ViewportId >= MAX_VIEWPORT) {
                l_ViewportId = 0;
            }

            S32 l_StartX;
            S32 l_StartY;
            S32 l_SizeX;
            S32 l_SizeY;
            Viewport_Z& l_Viewport = l_Renderer->GetViewport(l_ViewportId);
            l_Viewport.GetPosAndSize(l_StartX, l_StartY, l_SizeX, l_SizeY);
            Camera_Z* l_Camera = (Camera_Z*)l_Viewport.GetCamera()->GetObject();

            l_AddY[l_ViewportId] += 20.0f;
            Vec2f l_Position((Float)l_StartX + 80.0f, (Float)l_StartY + l_AddY[l_ViewportId]);
            const Vec3f& l_Translation = l_Agent->GetNode()->GetTranslation();

            String_Z<ARRAY_CHAR_MAX> l_String;
            l_String.Sprintf("(%.02f,%.02f,%.02f)", l_Translation.x, l_Translation.y, l_Translation.z);
            l_Renderer->DrawStringOutline(l_Position, l_String);

            Vec2f l_CameraPosition = l_Position;
            l_CameraPosition.y += 16.0f;
            l_String.Sprintf("Cam (N=%.02f,F=%.02f)", l_Camera->GetNearClip(), l_Camera->GetFarClip());
            l_Renderer->DrawStringOutline(l_CameraPosition, l_String);
        }
    }
}

Bool AddTransText() {
    if (gData.Cons->GetNbParam() < 3) {
        return TRUE;
    }

    AddString((S32)gData.Cons->GetParamFloat(1), gData.Cons->GetParamStr(2));
    return TRUE;
}

Bool SetLanguage() {
    if (gData.Cons->GetNbParam() < 2) {
        return TRUE;
    }
    SetLanguage((int)gData.Cons->GetParamFloat(1), -1, 0);
    return TRUE;
}

Bool AddParameter() {
    if (gData.Cons->GetNbParam() == 3) {
        Char* l_Param = gData.Cons->GetStrParam(2);
        if ('A' <= *l_Param) {
            Name_Z l_Name;
            if (l_Param) {
                l_Name = Name_Z::GetID(l_Param);
            }
            else {
                l_Name = Name_Z();
            }
            U32 l_Id = l_Name.GetID();
            AddParameters(gData.Cons->GetParamFloat(1), *((Float*)&l_Id));
        }
        else {
            AddParameters(gData.Cons->GetParamFloat(1), gData.Cons->GetParamFloat(2));
        }
    }
    return TRUE;
}

Bool ReadParameters() {
    if (gData.Cons->GetNbParam() < 2) {
        return TRUE;
    }
    ReadParameters(gData.Cons->GetParamFloat(1));
    return TRUE;
}

Bool MarkMemory() {
    s_MarkMem(1);
    return TRUE;
}

Bool ShowUnmarkedMemory() {
    s_ShowUnMarkedMem();
    return TRUE;
}
