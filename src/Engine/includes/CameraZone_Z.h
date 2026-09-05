#ifndef _CAMERAZONE_Z_H_
#define _CAMERAZONE_Z_H_
#include "SplineZone_Z.h"
#include "CameraZoneData_Z.h"

class CameraZone_Z : public SplineZone_Z {
public:
    virtual ~CameraZone_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void Reset();

    Bool GetCameraZoneData(const Vec3f& i_WorldPosition, CameraZoneData_Z& o_Data);
    U16 GetTriggerFlag(S32 i_TriggerId);
    void EnableTriggerFlag(S32 i_TriggerId, U16 i_Flag);
    Bool GetCameraZoneInterpol(const Vec3f& i_WorldPosition, const Vec3f& i_WorldFocus, const CameraZoneData_Z& i_Data, CameraZoneInterpolData_Z& o_Data);

    static BaseObject_Z* NewObject() { return NewL_Z(56) CameraZone_Z; }
};

#endif // _CAMERAZONE_Z_H_
