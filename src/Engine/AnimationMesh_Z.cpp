#include "Animation_Z.h"
#include "AnimationMesh_Z.h"
#include "Skel_Z.h"

// $SABE TODO: Finish matching
void AnimationMesh_Z::UpdateCct(Float i_Time, const AnimationMeshData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMeshKeyId_Z& i_KeyId, Skel_Z* i_Skel) {
    if (i_AnimData.m_VisibilityKfr.GetNbKeys()) {
        S32 l_StartKfr = i_Cct.m_StartKfr;
        S32 l_NbKfr = i_Cct.m_NbKfr;
        Float l_Visibility;
        i_KeyId.m_CurVisibility = i_AnimData.m_VisibilityKfr.GetCctValue(l_NbKfr, l_StartKfr, i_Time, l_Visibility, i_KeyId.m_CurVisibility);
        if (l_Visibility != 0.0f) {
            i_Skel->UnhideObjectNamed(i_Cct.m_ObjectName, FALSE);
        }
        else {
            i_Skel->HideObjectNamed(i_Cct.m_ObjectName, FALSE);
        }
    }
}

void AnimationMesh_Z::Load(AnimationMeshData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_VisibilityKfr.Load(i_Data);
}
