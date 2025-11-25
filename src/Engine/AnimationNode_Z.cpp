#include "AnimationNode_Z.h"
#include "Animation_Z.h"
#include "BoneNode_Z.h"
#include "Sys_Z.h"

void AnimationNode_Z::UpdateCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_RegMsgIndex) {
    Vec3f l_Vec;
    Quat l_Quat;

    // Rotation
    if (i_Frame.m_Cct->m_RotNbKfr > 1) {
        i_Frame.m_KeyId->m_CurRot = i_Frame.m_Data->m_RotKfr.GetCctValue(
            i_Frame.m_Cct->m_RotStartKfr,
            i_Frame.m_Cct->m_RotNbKfr,
            i_Times.y,
            l_Quat,
            i_Frame.m_KeyId->m_CurRot
        );
        i_Frame.m_UpdateBoneNode->SetRotation(l_Quat);
    }
    else {
        if (i_Frame.m_Cct->m_BezierRotNbKfr > 1) {
            i_Frame.m_KeyId->m_CurRot = i_Frame.m_Data->m_BezierRotKfr.GetCctValue(
                i_Frame.m_Cct->m_BezierRotStartKfr,
                i_Frame.m_Cct->m_BezierRotNbKfr,
                i_Times.y,
                l_Quat,
                i_Frame.m_KeyId->m_CurRot
            );
            i_Frame.m_UpdateBoneNode->SetRotation(l_Quat);
        }
    }

    // Scale
    if (i_Frame.m_Cct->m_ScaleNbKfr > 1) {
        i_Frame.m_KeyId->m_CurScale = i_Frame.m_Data->m_ScaleKfr.GetCctValue(
            i_Frame.m_Cct->m_ScaleStartKfr,
            i_Frame.m_Cct->m_ScaleNbKfr,
            i_Times.y,
            l_Vec,
            i_Frame.m_KeyId->m_CurScale
        );
        if (i_Frame.m_Modifier) {
            l_Vec += i_Frame.m_Modifier->m_Scale;
        }
        i_Frame.m_UpdateBoneNode->SetScale(l_Vec);
    }

    // Translation
    if (i_Frame.m_Cct->m_TransNbKfr > 1) {
        i_Frame.m_KeyId->m_CurTrans = i_Frame.m_Data->m_TransKfr.GetCctValue(
            i_Frame.m_Cct->m_TransStartKfr,
            i_Frame.m_Cct->m_TransNbKfr,
            i_Times.y,
            l_Vec,
            i_Frame.m_KeyId->m_CurTrans
        );
        if (i_Frame.m_Modifier) {
            l_Vec += i_Frame.m_Modifier->m_Translation;
        }
        i_Frame.m_UpdateBoneNode->SetTranslation(l_Vec);
    }

    // Message
    if (i_Frame.m_Cct->m_MsgNbKfr) {
        i_Frame.m_Data->m_MsgKfr.GetCctValue(
            i_Frame.m_Cct->m_MsgStartKfr,
            i_Frame.m_Cct->m_MsgNbKfr,
            i_Times.x,
            i_Times.y,
            i_Times.z,
            i_Frame.m_RegMsg,
            io_RegMsgIndex
        );
    }
}

void AnimationNode_Z::GetCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_RegMsgIndex) {
    Vec3f l_Vec;
    Vec3f l_OriginalVec;
    Quat l_Quat;

    // Rotation
    if (i_Frame.m_Cct->m_RotNbKfr) {
        i_Frame.m_Data->m_RotKfr.GetCctValue(
            i_Frame.m_Cct->m_RotStartKfr,
            i_Frame.m_Cct->m_RotNbKfr,
            i_Times.y,
            l_Quat
        );
        i_Frame.m_UpdateBoneNode->SetRotation(l_Quat);
    }
    else {
        if (i_Frame.m_Cct->m_BezierRotNbKfr) {
            i_Frame.m_Data->m_BezierRotKfr.GetCctValue(
                i_Frame.m_Cct->m_BezierRotStartKfr,
                i_Frame.m_Cct->m_BezierRotNbKfr,
                i_Times.y,
                l_Quat
            );
            i_Frame.m_UpdateBoneNode->SetRotation(l_Quat);
        }
        else {
            if ((i_Frame.m_Cct->m_Flag & ANIM_NODE_RESET_ROT) != 0) {
                i_Frame.m_UpdateBoneNode->SetRotation(i_Frame.m_OriginalBoneNode->GetRotation());
            }
        }
    }

    // Scale
    if (i_Frame.m_Cct->m_ScaleNbKfr) {
        i_Frame.m_Data->m_ScaleKfr.GetCctValue(
            i_Frame.m_Cct->m_ScaleStartKfr,
            i_Frame.m_Cct->m_ScaleNbKfr,
            i_Times.y,
            l_Vec
        );
        if (i_Frame.m_Modifier) {
            l_Vec += i_Frame.m_Modifier->m_Scale;
        }
        i_Frame.m_UpdateBoneNode->SetScale(l_Vec);
    }
    else {
        if (i_Frame.m_Cct->m_Flag & ANIM_NODE_RESET_SCALE) {
            l_OriginalVec = i_Frame.m_OriginalBoneNode->GetScale();
            if (i_Frame.m_Modifier) {
                l_OriginalVec += i_Frame.m_Modifier->m_Scale;
            }
            i_Frame.m_UpdateBoneNode->SetScale(l_OriginalVec);
        }
    }

    // Translation
    if (i_Frame.m_Cct->m_TransNbKfr) {
        i_Frame.m_Data->m_TransKfr.GetCctValue(
            i_Frame.m_Cct->m_TransStartKfr,
            i_Frame.m_Cct->m_TransNbKfr,
            i_Times.y,
            l_Vec
        );
        if (i_Frame.m_Modifier) {
            l_Vec += i_Frame.m_Modifier->m_Translation;
        }
        i_Frame.m_UpdateBoneNode->SetTranslation(l_Vec);
    }
    else {
        if (i_Frame.m_Cct->m_Flag & ANIM_NODE_RESET_TRANS) {
            l_OriginalVec = i_Frame.m_OriginalBoneNode->GetTranslation();
            if (i_Frame.m_Modifier) {
                l_OriginalVec += i_Frame.m_Modifier->m_Translation;
            }
            i_Frame.m_UpdateBoneNode->SetTranslation(l_OriginalVec);
        }
    }

    // Message
    if (i_Frame.m_RegMsg) {
        if (i_Frame.m_Cct->m_MsgNbKfr) {
            i_Frame.m_Data->m_MsgKfr.GetCctValue(
                i_Frame.m_Cct->m_MsgStartKfr,
                i_Frame.m_Cct->m_MsgNbKfr,
                i_Times.x,
                i_Times.y,
                i_Times.z,
                i_Frame.m_RegMsg,
                io_RegMsgIndex
            );
        }
    }
}

void AnimationNode_Z::CctMessage(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_RegMsgIndex) {
    if (i_Frame.m_RegMsg) {
        if (i_Frame.m_Cct->m_MsgNbKfr) {
            i_Frame.m_Data->m_MsgKfr.GetCctValue(
                i_Frame.m_Cct->m_MsgStartKfr,
                i_Frame.m_Cct->m_MsgNbKfr,
                i_Times.x,
                i_Times.y,
                i_Times.z,
                i_Frame.m_RegMsg,
                io_RegMsgIndex
            );
        }
    }
}

void AnimationNode_Z::Load(AnimationNodeData_Z& i_AnimData, void** i_Data) {
    MEMCPYFROM_Z(&i_AnimData.m_Flag, *i_Data, sizeof(i_AnimData.m_Flag));
    i_AnimData.m_RotKfr.Load(i_Data);
    i_AnimData.m_BezierRotKfr.Load(i_Data);
    i_AnimData.m_ScaleKfr.Load(i_Data);
    i_AnimData.m_TransKfr.Load(i_Data);
    i_AnimData.m_MsgKfr.Load(i_Data);
}
