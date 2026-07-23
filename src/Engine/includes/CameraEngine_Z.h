#ifndef _CAMERAENGINE_Z_H_
#define _CAMERAENGINE_Z_H_
#include "Manipulator_Z.h"

class Game_ZHdl;

class CameraEngine_Z : public Manipulator_Z {
public:
    virtual ~CameraEngine_Z() { }

    virtual void Init() { }

    virtual Bool MarkHandles();
    virtual void ActionOnActivate();

    virtual void InterpMessage(const int& a1, Float a2) { }

    virtual void SetGame(const Game_ZHdl& a1) { }

    virtual void UpdateFX(Float a1);

    virtual void CaptureInit() { }

    virtual void CaptureDo(Float a1) { }

    virtual void CaptureEnd() { }

    virtual void CaptureParams(S32 a1, Float a2) { }

    virtual void CaptureStart(S32 a1) { }
};

#endif // _CAMERAENGINE_Z_H_
