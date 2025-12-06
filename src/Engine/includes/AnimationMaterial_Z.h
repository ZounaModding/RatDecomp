#ifndef _ANIMATIONMATERIAL_Z_H_
#define _ANIMATIONMATERIAL_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class Material_Z;
class AnimationConcatMtl_Z;

struct AnimationMaterialKeyId_Z {
    S16 m_CurTransU;
    S16 m_CurTransV;
    S16 m_CurDiffuse;
    S16 m_CurEmissive;
    S16 m_CurRot;
    AnimationMaterialKeyId_Z();
    void Reset();
};

struct AnimationMtlData_Z {
    KeyframerFloatComp_Z m_TransUKfr;
    KeyframerFloatComp_Z m_TransVKfr;
    KeyframerVec3fComp_Z m_EmissiveColorKfr;
    KeyframerVec3fComp_Z m_DiffuseColorKfr;
    KeyframerFloatComp_Z m_RotKfr;

    inline void Flush() {
        m_TransUKfr.Flush();
        m_TransVKfr.Flush();
        m_EmissiveColorKfr.Flush();
        m_DiffuseColorKfr.Flush();
        m_RotKfr.Flush();
    }
};

class AnimationMaterial_Z {
private:
    Name_Z m_ObjectName;
    int m_ObjectId;
    AnimationMtlData_Z m_Data;

public:
    static void UpdateCct(Float i_Time, const AnimationMtlData_Z& i_AnimData, const AnimationConcatMtl_Z& i_Cct, AnimationMaterialKeyId_Z& i_KeyId, Material_Z* i_Material);
    static void Load(AnimationMtlData_Z& i_AnimData, void** i_Data);
};
#endif // _ANIMATIONMATERIAL_Z_H_
