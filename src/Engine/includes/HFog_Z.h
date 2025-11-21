#ifndef _FOG_Z_H_
#define _FOG_Z_H_
#include "Math_Z.h"
#include "Object_Z.h"

class HFogData_Z : public ObjectDatas_Z {

private:
    Float m_Unk0x28;
    Float m_Unk0x2C;
    Mat4x4 m_UnkMatrix;
    Vec3f m_Color;
    Float m_Active;
};

#endif
