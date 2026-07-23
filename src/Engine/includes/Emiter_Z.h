#ifndef _EMITER_Z_H_
#define _EMITER_Z_H_
#include "Object_Z.h"

class Emiter_Z : public Object_Z {
public:
    virtual ~Emiter_Z() { }

    virtual void Load(void** i_Data);
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void GetCollision(Node_Z* i_UnkNode, Sphere_Z& i_UnkSphere, Vec3f& i_UnkVec3f, Float i_UnkFloat, ColLineResult_Z* o_ColLineResult, S32 i_UnkS32);

    static BaseObject_Z* NewObject() { return NewL_Z(26) Emiter_Z; }
};

#endif // _EMITER_Z_H_
