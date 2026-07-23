#ifndef _LIGHTDATA_Z_H_
#define _LIGHTDATA_Z_H_

#include "Object_Z.h"

class LightData_Z : public ObjectDatas_Z {
public:
    LightData_Z();

    virtual ~LightData_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() { return NewL_Z(42) LightData_Z; }

    inline void SetColor(const Vec3f& i_Color) { m_Color = i_Color; }

    inline void SetAmbiant(const Vec3f& i_Ambient) { m_Ambient = i_Ambient; }

    inline void SetDir(const Vec3f& i_Direction) { m_Direction = i_Direction; }

private:
    Float m_UnkFloat_0x28;
    Float m_UnkFloat_0x2C;
    Vec3f m_Direction;
    Vec3f m_Color;
    Vec3f m_UnkVec3f_0x48;
    U32 m_LightFlag;
    Vec3f m_Ambient;
    Vec3f m_UnkVec3f_0x64;
};

#endif //_LIGHTDATA_Z_H_
