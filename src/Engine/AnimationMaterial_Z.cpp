#include "AnimationMaterial_Z.h"
#include "Sys_Z.h"

void AnimationMaterial_Z::Load(AnimationMtlData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_TransUKfr.Load(i_Data);
    i_AnimData.m_TransVKfr.Load(i_Data);
    i_AnimData.m_DiffuseColorKfr.Load(i_Data);
    i_AnimData.m_EmissiveColorKfr.Load(i_Data);
    i_AnimData.m_RotKfr.Load(i_Data);
}
