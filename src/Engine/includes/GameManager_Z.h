#ifndef _GAMEMANAGER_Z_H_
#define _GAMEMANAGER_Z_H_
#include "CameraUser_ZHdl.h"
#include "CameraOccluder_ZHdl.h"
#include "CameraStream_ZHdl.h"
#include "Game_ZHdl.h"
#include "Game_Z.h"
#include "CameraUser_Z.h"
#include "CameraStream_Z.h"
#include "CameraOccluder_Z.h"

#define MAX_NUMBER_OF_PLAYERS 4
#define MAX_VIEWPORT 4
#define NONE_VIEWPORT -1
#define START_PREDEFINED_VIEWPORT 4
#define USER_VIEWPORT_FIRST 4
#define USER_VIEWPORT_SECOND 5
#define USER_VIEWPORT_LAST 6
#define GLOBAL_VIEWPORT 6
#define GLOBAL_DEBUG_VIEWPORT GLOBAL_VIEWPORT
#define TOTAL_VIEWPORT 7

class Viewport_Z;

void RegisterGameMgrCommand();
Bool SetMultiGame();
Bool SetMonoGame();
Bool SetGame(Bool i_IsMono);
Bool AddGamePlayer();
Bool AddMenuPlayer();
Bool AddPlayer(Bool i_IsMenu);
Bool RemoveGamePlayer();
Bool ActivateGamePlayer();
Bool DeactivateGamePlayer();
Bool SwitchGameToMono();
Bool SwitchGameToMulti();
Bool ActivateGame();
Bool DeactivateGame();
Bool SwitchCameraUser();
Bool SetPersoCamera();
Bool ResetGame();
Bool PlayerGoto();
Bool RemoveGame();
Bool SetRtc();
Bool PlayRtc();
Bool ShutRtc();
Bool LoadRtc();
Bool AddTransText();
Bool SetLanguage();
Bool AddParameter();
Bool ReadParameters();
Bool MarkMemory();
Bool ShowUnmarkedMemory();

class GameManager_Z {
public:
    GameManager_Z();

    void Update(Float i_DeltaTime);
    S32 GetGameIdByWorld(const World_ZHdl& i_WorldHdl);
    void DebugDisplay(Viewport_Z* i_Vp);

    inline Bool IsCameraUser(const S32 i_VpId) const {
        return m_CameraUserBools[i_VpId];
    }

    inline S32 GetNbGame() const {
        return m_GameHdls.GetSize();
    }

    inline Game_ZHdl& GetGame(int i_GameId) {
        return m_GameHdls[i_GameId];
    }

    inline void EndPause() {
        m_IsPaused = FALSE;
    }

private:
    CameraUser_ZHdl m_CameraUserHdls[MAX_VIEWPORT];
    Bool m_CameraUserBools[MAX_VIEWPORT];
    Game_ZHdlDA m_GameHdls;
    CameraOccluder_ZHdl m_CameraOccluderHdls[MAX_VIEWPORT];
    CameraStream_ZHdl m_CameraStreamHdls[MAX_VIEWPORT];
    Bool m_IsPaused;
};

#endif
