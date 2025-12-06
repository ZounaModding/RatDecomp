#include "AnimationMaterial_Z.h"
#include "Animation_Z.h"
#include "Material_Z.h"

void AnimationMaterial_Z::UpdateCct(Float i_Time, const AnimationMtlData_Z& i_AnimData, const AnimationConcatMtl_Z& i_Cct, AnimationMaterialKeyId_Z& i_KeyId, Material_Z* i_Material) {
    Float l_Value;
    Vec3f l_Vec;
    S16 l_Cur;

    // Translation U
    if (i_Cct.m_TransUNbKfr) {
        l_Cur = i_KeyId.m_CurTransU;
        l_Cur = i_AnimData.m_TransUKfr.GetCctValue(i_Cct.m_TransUStartKfr, i_Cct.m_TransUNbKfr, i_Time, i_Material->GetTranslation().x, l_Cur);
        i_KeyId.m_CurTransU = l_Cur;
    }

    // Translation V
    if (i_Cct.m_TransVNbKfr) {
        l_Cur = i_KeyId.m_CurTransV;
        l_Cur = i_AnimData.m_TransVKfr.GetCctValue(i_Cct.m_TransVStartKfr, i_Cct.m_TransVNbKfr, i_Time, i_Material->GetTranslation().y, l_Cur);
        i_KeyId.m_CurTransV = l_Cur;
    }

    // Diffuse Color
    if (i_Cct.m_DiffuseColorNbKfr) {
        l_Cur = i_KeyId.m_CurDiffuse;
        l_Cur = i_AnimData.m_DiffuseColorKfr.GetCctValue(i_Cct.m_DiffuseColorStartKfr, i_Cct.m_DiffuseColorNbKfr, i_Time, l_Vec, l_Cur);
        i_KeyId.m_CurDiffuse = l_Cur;
        i_Material->SetDiffuse(l_Vec);
    }

    // Emissive Color
    if (i_Cct.m_EmissiveColorNbKfr) {
        l_Cur = i_KeyId.m_CurEmissive;
        l_Cur = i_AnimData.m_EmissiveColorKfr.GetCctValue(i_Cct.m_EmissiveColorStartKfr, i_Cct.m_EmissiveColorNbKfr, i_Time, l_Vec, l_Cur);
        i_KeyId.m_CurEmissive = l_Cur;
        i_Material->SetEmissive(l_Vec);
    }

    // Rotation
    if (i_Cct.m_RotNbKfr) {
        l_Cur = i_KeyId.m_CurRot;
        l_Cur = i_AnimData.m_RotKfr.GetCctValue(i_Cct.m_RotStartKfr, i_Cct.m_RotNbKfr, i_Time, l_Value, l_Cur);
        i_KeyId.m_CurRot = l_Cur;
        i_Material->SetRotation(l_Value);
    }

    i_Material->UpdateTMatrix();
}

void AnimationMaterial_Z::Load(AnimationMtlData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_TransUKfr.Load(i_Data);
    i_AnimData.m_TransVKfr.Load(i_Data);
    i_AnimData.m_DiffuseColorKfr.Load(i_Data);
    i_AnimData.m_EmissiveColorKfr.Load(i_Data);
    i_AnimData.m_RotKfr.Load(i_Data);
}
