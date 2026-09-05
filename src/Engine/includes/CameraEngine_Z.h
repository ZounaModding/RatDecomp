#ifndef _CAMERAENGINE_Z_H_
#define _CAMERAENGINE_Z_H_
#include "Game_ZHdl.h"
#include "LodMove_ZHdl.h"
#include "Manipulator_Z.h"
#include "Node_ZHdl.h"
#include "World_ZHdl.h"

class CameraEngine_Z : public Manipulator_Z {
public:
    virtual ~CameraEngine_Z() { }

    virtual void Init() {
        Manipulator_Z::Init();
        SetGroup(ag_camera);
    }

    virtual Bool MarkHandles();
    virtual void ActionOnActivate();

    virtual void InterpMessage(const int& a1, Float a2) { }

    virtual void SetGame(const Game_ZHdl& i_GameHdl) { m_GameHdl = i_GameHdl; }

    virtual void UpdateFX(Float a1);

    virtual void CaptureInit() { }

    virtual void CaptureDo(Float a1) { }

    virtual void CaptureEnd() { }

    virtual void CaptureParams(S32 a1, Float a2) { }

    virtual void CaptureStart(S32 a1) { }

    void SetViewport(S32 i_ViewportId) { m_ViewportId = i_ViewportId; }

    S32 GetViewport() const { return m_ViewportId; }

    const World_ZHdl& GetWorld();
    const Node_ZHdl& GetCameraNode();

protected:
    S32 m_ViewportId;
    LodMove_ZHdl m_LodMoveHdl;
    Game_ZHdl m_GameHdl;
};

#endif // _CAMERAENGINE_Z_H_
