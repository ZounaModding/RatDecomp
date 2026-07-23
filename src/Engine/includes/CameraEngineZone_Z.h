#ifndef _CAMERAENGINEZONE_Z_H_
#define _CAMERAENGINEZONE_Z_H_
#include "CameraEngine_Z.h"
#include "Math_Z.h"

class World_Z;

class CameraEngineZone_Z : public CameraEngine_Z {
public:
    virtual void Init();

    virtual ~CameraEngineZone_Z() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void InterpMessage(const int& a1, Float a2);
    virtual void CaptureInit();
    virtual void CaptureDo(Float a1);
    virtual void CaptureEnd();
    virtual void CaptureParams(S32 a1, Float a2);
    virtual void CaptureStart(S32 a1);

    Vec3f CameraGameColl(const World_Z* i_World, Vec3f i_CamPos, Vec3f i_CamTarget, Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(68) CameraEngineZone_Z; }
};

#endif // _CAMERAENGINEZONE_Z_H_
