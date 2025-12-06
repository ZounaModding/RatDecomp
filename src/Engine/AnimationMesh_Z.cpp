#include "Animation_Z.h"
#include "AnimationMesh_Z.h"
#include "Skel_Z.h"

void AnimationMesh_Z::UpdateCct(Float i_Time, const AnimationMeshData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, AnimationMeshKeyId_Z& i_KeyId, Skel_Z* i_Skel) {
    if (i_AnimData.m_VisibilityKfr.GetNbKeys()) {
        Float l_Visibility;
        S16 l_Cur = i_KeyId.m_CurVisibility;
        l_Cur = i_AnimData.m_VisibilityKfr.GetCctValue(i_Cct.m_StartKfr, i_Cct.m_NbKfr, i_Time, l_Visibility, l_Cur);
        i_KeyId.m_CurVisibility = l_Cur;
        if (l_Visibility) {
            i_Skel->UnhideObjectNamed(i_Cct.m_ObjectName, FALSE);
        }
        else {
            i_Skel->HideObjectNamed(i_Cct.m_ObjectName, FALSE);
        }
    }
}

void AnimationMesh_Z::EndCct(Float i_Time, const AnimationMeshData_Z& i_AnimData, const AnimationConcat_Z& i_Cct, Skel_Z* i_Skel) {
    if (i_AnimData.m_VisibilityKfr.GetNbKeys()) {
        if (i_Cct.m_NbKfr) {
            S32 l_Nb = i_Cct.m_NbKfr;
            l_Nb = i_Cct.m_StartKfr + l_Nb - 1;
            if (i_AnimData.m_VisibilityKfr.GetKey(l_Nb).GetValue()) {
                i_Skel->UnhideObjectNamed(i_Cct.m_ObjectName, FALSE);
            }
            else {
                i_Skel->HideObjectNamed(i_Cct.m_ObjectName, FALSE);
            }
        }
    }
}

void AnimationMesh_Z::Load(AnimationMeshData_Z& i_AnimData, void** i_Data) {
    i_AnimData.m_VisibilityKfr.Load(i_Data);
}
