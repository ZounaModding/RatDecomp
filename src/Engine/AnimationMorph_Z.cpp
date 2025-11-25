#include "Animation_Z.h"
#include "AnimationMorph_Z.h"
#include "MeshMorph_Z.h"
#include "Types_Z.h"

// $SABE TODO: Finish matching
void AnimationMorph_Z::UpdateCct(Float i_Time, const AnimationMorphData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMorphKeyId_Z& i_KeyId, MorphTarget_Z& i_MorphTarget) {
    if (i_AnimData.m_MorphKfr.GetNbKeys()) {
        S32 l_CurMorph = i_KeyId.m_CurMorph;
        S32 l_StartKfr = i_Cct.m_StartKfr;
        S32 l_NbKfr = i_Cct.m_NbKfr;
        Float l_Morph;
        i_KeyId.m_CurMorph = i_AnimData.m_MorphKfr.GetCctValue(l_NbKfr, l_StartKfr, i_Time, l_Morph, l_CurMorph);
        i_MorphTarget.m_Morph = l_Morph;
    }
}

void AnimationMorph_Z::Load(AnimationMorphData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_MorphKfr.Load(i_Data);
}
