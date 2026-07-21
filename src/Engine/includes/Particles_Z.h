#ifndef _PARTICLES_Z_H_
#define _PARTICLES_Z_H_
#include "Object_Z.h"
#include "ParticlesEle_Z.h"

#define FL_PARTICLES_DISABLE_PLANE_UPDATE (FL_OBJECTDATAS_LAST) // 0x10000 - Disable collision plane update for this particle

class ParticlesData_Z;

class Particles_Z : public Object_Z {
public:
    Particles_Z();
    virtual ~Particles_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data);
    virtual S32 GetCollisions(
        Node_Z* i_Node,
        ObjectDatas_Z* i_Data,
        const Sphere_Z& i_Sph,
        const Vec3f& i_Dir,
        Float i_Len,
        ColLineResult_Z* o_Result,
        S32 i_MaxElem,
        U64 i_Flag,
        U64 i_NoFlag
    );

    static BaseObject_Z* NewObject() {
        return NewL_Z(39) Particles_Z;
    }

    U32 HasEmittersFlag(U32 i_Flag);

protected:
    U8 m_Pad_0x80[32];
};

class ParticlesData_Z : public ObjectDatas_Z {
public:
    ParticlesData_Z();
    virtual ~ParticlesData_Z();
    virtual void Load(void** i_Data);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void CloneEle(const ParticlesEle_ZDA& o_Elements, const Sphere_Z& i_BSphere);

protected:
    Sphere_Z m_BSphere;
    Float m_UnkFloat_0x40;
    Float m_UnkFloat_0x44;
    ParticlesDataEle_ZDA m_ParticleEmittersData;
    Vec3fDA m_WorldSpacePlanes;
};

#endif // _PARTICLES_Z_H_
