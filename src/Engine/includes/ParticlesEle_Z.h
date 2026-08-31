#ifndef _PARTICLESELE_Z_H_
#define _PARTICLESELE_Z_H_
#include "DynArray_Z.h"
#include "Keyframer_Z.h"
#include "Material_ZHdl.h"
#include "MaterialAnim_ZHdl.h"
#include "Math_Z.h"
#include "Main_Z.h"
#include "ParticlesManager_Z.h"
#include "SystemObject_Z.h"

#define PARTICLES_ELE_MAX_QUANTITY 256

struct ParticlesEle_Z {
    ParticlesEle_Z();
    void Load(void** i_Data);
    void EndLoad();
    void AfterEndLoad();
    void MarkHandles();

    inline U32 HasFlag(U32 i_Flag) const {
        return m_Flag & i_Flag;
    }

    S16 m_MaxQuantity;
    U16 m_Rate;
    Float m_UnkFloat_0x4;
    Float m_Speed;
    Float m_Rotation;
    Float m_ConeRadius;
    Float m_ConeHeight;
    Float m_ConeRadius2;
    Float m_OffPlaneSpread;
    Float m_PeriodTime;
    Float m_LifeTime;
    Float m_SizeX;
    Float m_SizeY;
    Float m_BubbleAmplitude;
    Float m_BubblePeriod;
    Float m_BubbleVariation;
    Float m_Gravity;
    Float m_Reflex0;
    Float m_Reflex1;
    U32 m_Flag;
    KeyframerVec2fLinearComp_Z m_EmitLifeVelocity;
    KeyframerVec4fLinearComp_Z m_EmitSRQG;
    KeyframerVec4fLinearComp_Z m_PartSizeXRVA;
    KeyframerFloatLinearComp_Z m_PartSizeY;
    KeyframerFloatLinearComp_Z m_PartGravity;
    KeyframerVec3fLinear_Z m_EmitColor;
    KeyframerVec3fLinear_Z m_PartColor;
    Material_ZHdl m_MaterialHdl;
    MaterialAnim_ZHdl m_MaterialAnimHdl;
};

typedef DynArray_Z<ParticlesEle_Z, 1> ParticlesEle_ZDA;

struct ParticleSphere_Z {
    ParticleSphere_Z() {
        m_UnkId = -1;
    }

    Sphere_Z m_Sphere;
    S16 m_UnkId;
    S16 m_Pad_0x12;
    U32 m_Pad_0x14[3];
};

class ParticlesDataEle_Z {
public:
    ParticlesDataEle_Z()
        : m_EntryHdl() {
        m_MaxQuantity = 0;
        Reset();
    }

    ~ParticlesDataEle_Z() {
        if (gData.ParticlesMgr && m_EntryHdl.IsValid()) {
            gData.ParticlesMgr->ReleaseObject(m_EntryHdl);
        }
        m_EntryHdl.Invalidate();
    }

    inline void Reset() {
        if (gData.ParticlesMgr) {
            if (m_EntryHdl.IsValid()) {
                gData.ParticlesMgr->ReleaseObject(m_EntryHdl);
            }
            m_EntryHdl = gData.ParticlesMgr->RegisterObject(m_MaxQuantity);
        }
        else {
            m_EntryHdl.Invalidate();
        }
    }

    inline ParticleSphere_Z* GetSpheres() {
        return m_Spheres;
    }

    void GetBSphere(Sphere_Z& io_Sphere);

    ParticlesManager_Z::Hdl m_EntryHdl;
    U8 m_Pad_0x4[12];
    ParticleSphere_Z m_Spheres[8];
    S16 m_MaxQuantity;
    U8 m_Pad_0x112[14];
};

typedef DynArray_Z<ParticlesDataEle_Z, 1> ParticlesDataEle_ZDA;

#endif // _PARTICLESELE_Z_H_
