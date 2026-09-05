#ifndef _CAMERAENGINEZONE_Z_H_
#define _CAMERAENGINEZONE_Z_H_
#include "CameraEngine_Z.h"
#include "Math_Z.h"
#include "SystemObject_Z.h"
#include "CameraZone_ZHdl.h"

class World_Z;
class Bitmap_Z;

#define CAMERA_ENGINE_CAPTURE_TYPE_PATCH 0
#define CAMERA_ENGINE_CAPTURE_TYPE_FRAME 1
#define CAMERA_ENGINE_CAPTURE_TYPE_TEXTURE 2
#define CAMERA_ENGINE_CAPTURE_TYPE_NONE 3

#define CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE 0
#define CAMERA_ENGINE_CAPTURE_PARAM_FRAME_NB 1
#define CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB 2

class CameraEngineZone_Z : public CameraEngine_Z {
public:
    virtual void Init();

    virtual ~CameraEngineZone_Z() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void InterpMessage(const int& i_Message, Float i_Param);
    virtual void CaptureInit();
    virtual void CaptureDo(Float a1);
    virtual void CaptureEnd();
    virtual void CaptureParams(S32 a1, Float a2);
    virtual void CaptureStart(S32 a1);

    Vec3f CameraGameColl(const World_Z* i_World, Vec3f i_CamPos, Vec3f i_CamTarget, Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(68) CameraEngineZone_Z; }

private:
    Vec3f m_CameraWorldFocus;
    Bool m_FixOrFocusRemoved;
    Float m_RemovedTime;
    Float m_SmoothFactor;
    Vec3f m_SmoothedFocus;
    Vec3f m_SmoothedCameraDistance;
    Bool m_First;
    Bool m_Capture;
    Bitmap_Z* m_CaptureBitmap;
    CameraZone_ZHdl m_CameraZoneHdl;
    Vec2f m_CaptureDepl;
    Sphere_Z m_CaptureSph;
    S32 m_CaptureNb;
    Float m_CapturedTime;
    Float m_CapturedTotalTime;
    Vec3f m_CapturePos;
    Float m_CaptureDelta;
    Float m_CaptureFramerate;
    Float m_CapturePatchMax;
    S32 m_CaptureFrameNb;
    S32 m_CaptureType;
    S32 m_UnkS32_0xac;
    Vec3f m_Velocity;
    Vec3f m_OldCamPos;
};

#endif // _CAMERAENGINEZONE_Z_H_
