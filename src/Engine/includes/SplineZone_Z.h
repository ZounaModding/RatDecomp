#ifndef _SPLINEZONE_Z_H_
#define _SPLINEZONE_Z_H_
#include "Object_Z.h"

class Bitmap_Z;

class SplineZone_Z : public Object_Z {
public:
    virtual ~SplineZone_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void Reset();
    virtual void DrawBitmap(Bitmap_Z* i_Bitmap, const Sphere_Z& i_ClipSphere, const Vec2f& i_Pos);

    void GetZone(const Vec3f& i_CamPos, S32& o_ZoneId);

    static BaseObject_Z* NewObject() { return NewL_Z(101) SplineZone_Z; }
};

#endif // _SPLINEZONE_Z_H_
