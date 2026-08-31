#include "CameraStream_Z.h"
#include "Renderer_Z.h"
#include "World_Z.h"
#include "Camera_Z.h"
#include "GameManager_Z.h"

void CameraStream_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_camera_stream);
    m_ViewportId = NONE_VIEWPORT;
}

// TODO: Finish matching
void CameraStream_Z::Update(Float i_DeltaTime) {
    Viewport_Z& l_Vp = gData.MainRdr->GetViewport(m_ViewportId);
    S32 l_GameId = gData.GameMgr->GetGameIdByWorld(l_Vp.GetWorld());
    if (l_GameId < 0) {
        return;
    }

    Game_Z* l_Game = gData.GameMgr->GetGame(l_GameId);
    World_Z* l_World = l_Game->GetWorld();
    Vec3f l_CamPos = l_Vp.GetCamera()->GetWorldTranslation();
    l_Game->Stream(l_CamPos, HANDLE_NULL);
}
