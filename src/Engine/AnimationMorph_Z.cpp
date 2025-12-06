#include "Animation_Z.h"
#include "AnimationMorph_Z.h"
#include "MeshMorph_Z.h"
#include "Types_Z.h"

void AnimationMorph_Z::UpdateCct(Float i_Time, const AnimationMorphData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMorphKeyId_Z& i_KeyId, MorphTarget_Z& i_MorphTarget) {
    if (i_AnimData.m_MorphKfr.GetNbKeys()) {
        Float l_Visibility;
        S16 l_Cur = i_KeyId.m_CurMorph;
        Float l_Morph;
        l_Cur = i_AnimData.m_MorphKfr.GetCctValue(i_Cct.m_StartKfr, i_Cct.m_NbKfr, i_Time, l_Morph, l_Cur);
        i_KeyId.m_CurMorph = l_Cur;
        i_MorphTarget.m_Morph = l_Morph;
    }
}

void AnimationMorph_Z::Load(AnimationMorphData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_MorphKfr.Load(i_Data);
}
