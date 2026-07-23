#ifndef _CAMERAZONE_Z_H_
#define _CAMERAZONE_Z_H_
#include "SplineZone_Z.h"

class CameraZone_Z : public SplineZone_Z {
public:
    virtual ~CameraZone_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void Reset();

    static BaseObject_Z* NewObject() { return NewL_Z(56) CameraZone_Z; }
};

#endif // _CAMERAZONE_Z_H_
