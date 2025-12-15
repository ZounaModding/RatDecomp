#include "Animation_Z.h"
#include "Types_Z.h"
#include "Assert_Z.h"
#include "AnimMessage_Z.h"

Bool Animation_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    m_NodeKfr.m_MsgKfr.MarkHandles();
    return TRUE;
}

S32 Animation_Z::GetIndexOfNodeById(S32 i_NodeId) const {
    AnimationConcatNode_Z* l_ArrayPtr = m_NodeCct.GetArrayPtr();
    for (S32 i = 0; i < GetNbAnimationNode(); i++, l_ArrayPtr++) {
        S16 l_BoneId = l_ArrayPtr->m_BoneID;
        if (l_BoneId == i_NodeId) {
            return i;
        }
    }
    return -1;
}

Float Animation_Z::GetTimeFromMessage(S32 i_MsgId, S32 i_NodeId, Float i_StartTime) {
    S32 l_Idx = 0;
    S32 l_Nb;
    S32 l_KeyNb;
    S32 l_MsgIndex;
    S16 l_Cur;
    S16 l_MsgNb;

    if (i_NodeId >= 0) {
        l_Idx = GetIndexOfNodeById(i_NodeId);
        if (l_Idx < 0) return -1.0f;
    }

    for (l_Nb = GetNbAnimationNode(); l_Idx < l_Nb; l_Idx++) {
        l_Cur = m_NodeCct[l_Idx].m_MsgStartKfr;
        // $SABE: This is stupid but it makes it match
        // TODO: Match properly
        --l_Cur;
        ++l_Cur;
        for (l_MsgNb = m_NodeCct[l_Idx].m_MsgNbKfr; l_MsgNb--; l_Cur++) {
            KeyMessage_Z& l_Key = m_NodeKfr.m_MsgKfr.GetKeys()[l_Cur];
            l_KeyNb = l_Key.GetNb();
            for (l_MsgIndex = 0; l_MsgIndex < l_KeyNb; l_MsgIndex++) {
                if (l_Key.Get(l_MsgIndex).m_Id == i_MsgId && l_Key.GetTime() >= i_StartTime) {
                    return l_Key.GetTime();
                }
            }
        }
    }

    return -1.0f;
}

Bool Animation_Z::GetMessageFromNode(S32 i_NodeId, Float i_PrecTime, Float i_CurTime, Float i_MaxTime, RegMessage_Z* o_Msg, S32& io_NbMsg) {
    S32 l_Idx = GetIndexOfNodeById(i_NodeId);
    if (l_Idx >= 0) {
        m_NodeKfr.m_MsgKfr.GetCctValue(
            m_NodeCct[l_Idx].m_MsgStartKfr,
            m_NodeCct[l_Idx].m_MsgNbKfr,
            i_PrecTime,
            i_CurTime,
            i_MaxTime,
            o_Msg,
            io_NbMsg
        );
        if (io_NbMsg) {
            return TRUE;
        }
    }
    return FALSE;
}

void Animation_Z::BuildSmooth(
    Float i_Time,
    Float i_SmoothTime,
    const BoneNodePtr_ZDA& i_OriginalBones,
    const BoneNodePtr_ZDA& i_InstantiatedBones,
    const AnimationNodeModifier_ZDA& i_Modifiers,
    AnimationConcatNode_ZDA& i_Ccts,
    AnimationNodeData_Z& i_Data
) {
    S32 i;
    S32 l_CctNb = GetNbAnimationNode();
    i_Ccts.SetSize(l_CctNb, TRUE);
    S32 l_KeyNb = 0;
    AnimationConcatNode_Z* l_Cct = m_NodeCct.GetArrayPtr();
    for (i = 0; i < l_CctNb; i++, l_Cct++) {
        if (l_Cct->m_BoneID < i_InstantiatedBones.GetSize() && i_InstantiatedBones[l_Cct->m_BoneID]) {
            l_KeyNb += 2;
        }
    }
    i_Data.Flush();
    i_Data.m_TransKfr.SetReserve(l_KeyNb);
    i_Data.m_ScaleKfr.SetReserve(l_KeyNb);
    i_Data.m_RotKfr.SetReserve(l_KeyNb);

    AnimationConcatNode_Z* l_NewCct = i_Ccts.GetArrayPtr();
    l_Cct = m_NodeCct.GetArrayPtr();
    S32 l_MsgNb = 0;
    BoneTRS_Z l_Trs;
    Vec3f l_Times = Vec3f(0.0f, i_Time, 0.0f);
    AnimationCctNodeFrame_Z l_Frame;
    l_Frame.m_RegMsg = NULL;
    l_Frame.m_BoneTrs = &l_Trs;
    l_Frame.m_Data = &m_NodeKfr;
    l_Frame.m_Modifier = NULL;
    KeyRot_Z l_StartRot;
    KeyRot_Z l_CurRot;
    KeyVec3fComp_Z l_StartScale;
    KeyVec3fComp_Z l_CurScale;
    KeyVec3fComp_Z l_StartTrans;
    KeyVec3fComp_Z l_CurTrans;
    l_StartRot.SetTime(0.0f);
    l_CurRot.SetTime(i_SmoothTime);
    l_StartScale.SetTime(0.0f);
    l_CurScale.SetTime(i_SmoothTime);
    l_StartTrans.SetTime(0.0f);
    l_CurTrans.SetTime(i_SmoothTime);
    for (i = 0; i < l_CctNb; i++, l_Cct++, l_NewCct++) {
        S16 l_BoneId = l_Cct->m_BoneID;
        l_NewCct->m_BoneID = l_BoneId;
        if (l_BoneId < i_InstantiatedBones.GetSize()) {
            BoneNode_Z* l_BoneNode = i_InstantiatedBones[l_BoneId];
            if (l_BoneNode) {
                ASSERTLE_Z(
                    i_OriginalBones[l_BoneId] && i_OriginalBones[l_BoneId]->GetName() == l_BoneNode->GetName(),
                    "",
                    252,
                    "OriginalNodes[Id] && OriginalNodes[Id]->GetName()==pNode->GetName()"
                );
                l_NewCct->m_Flag = FL_ANIM_CCT_NONE;
                l_NewCct->m_BoneName = l_BoneNode->GetName();
                l_NewCct->m_TransStartKfr = i_Data.m_TransKfr.GetNbKeys();
                l_NewCct->m_RotStartKfr = i_Data.m_RotKfr.GetNbKeys();
                l_NewCct->m_ScaleStartKfr = i_Data.m_ScaleKfr.GetNbKeys();
                l_NewCct->m_ScaleNbKfr = 2;
                l_NewCct->m_RotNbKfr = 2;
                l_NewCct->m_TransNbKfr = 2;
                l_NewCct->m_BezierRotNbKfr = 0;
                l_NewCct->m_MsgNbKfr = 0;
                l_Frame.m_BoneNode = l_BoneNode;
                l_Frame.m_OriginalBoneNode = i_OriginalBones[l_BoneId];
                l_Frame.m_Cct = l_Cct;
                l_Trs = l_BoneNode->GetTRS();
                AnimationNode_Z::GetCct(l_Times, l_Frame, l_MsgNb);
                BoneTRS_Z& l_OriginalTrs = l_BoneNode->GetTRS();
                l_StartRot.Set(l_OriginalTrs.m_Rotation);
                l_CurRot.Set(l_Trs.m_Rotation);
                Vec3f l_Tgt = l_Trs.m_Scale - l_OriginalTrs.m_Scale;
                l_StartScale.Set(l_OriginalTrs.m_Scale, VEC3F_NULL, l_Tgt);
                l_CurScale.Set(l_Trs.m_Scale, -l_Tgt, VEC3F_NULL);
                l_Tgt = l_Trs.m_Translation - l_OriginalTrs.m_Translation;
                l_StartTrans.Set(l_OriginalTrs.m_Translation, VEC3F_NULL, l_Tgt);
                l_CurTrans.Set(l_Trs.m_Translation, -l_Tgt, VEC3F_NULL);
                i_Data.m_TransKfr.AddKey(l_StartTrans);
                i_Data.m_TransKfr.AddKey(l_CurTrans);
                i_Data.m_ScaleKfr.AddKey(l_StartScale);
                i_Data.m_ScaleKfr.AddKey(l_CurScale);
                i_Data.m_RotKfr.AddKey(l_StartRot);
                i_Data.m_RotKfr.AddKey(l_CurRot);
            }
        }
    }
    while (i != l_CctNb) {
        l_NewCct->m_TransNbKfr = 0;
        l_NewCct->m_RotNbKfr = 0;
        l_NewCct->m_ScaleNbKfr = 0;
        l_NewCct->m_BezierRotNbKfr = 0;
        l_NewCct->m_MsgNbKfr = 0;
        l_NewCct++;
    }
}

void Animation_Z::GetNodeCct(const Vec3f& i_Times, const BoneNodePtr_ZDA& i_OriginalBones, const BoneNodePtr_ZDA& i_InstantiatedBones, const AnimationNodeModifier_ZDA& i_Modifiers) {
    GetExtNodeCct(i_Times, i_OriginalBones, i_InstantiatedBones, i_Modifiers, m_NodeCct, m_NodeKfr);
}

void Animation_Z::GetExtNodeCct(
    const Vec3f& i_Times,
    const BoneNodePtr_ZDA& i_OriginalBones,
    const BoneNodePtr_ZDA& i_InstantiatedBones,
    const AnimationNodeModifier_ZDA& i_Modifiers,
    const AnimationConcatNode_ZDA& i_Ccts,
    const AnimationNodeData_Z& i_Data
) {
    S32 i;
    S32 l_MsgNb;
    S16 l_BoneId;

    AnimationCctNodeFrame_Z l_Frame;
    l_Frame.m_RegMsg = NULL;
    l_Frame.m_Modifier = NULL;
    l_Frame.m_Data = &i_Data;
    S32 l_CctCount = i_Ccts.GetSize();
    AnimationConcatNode_Z* l_Cct = i_Ccts.GetArrayPtr();
    for (i = 0; i < l_CctCount; i++, l_Cct++) {
        l_BoneId = (S32)l_Cct->m_BoneID;
        if (l_Cct->m_BoneID < i_InstantiatedBones.GetSize()) {
            BoneNode_Z* l_BoneNode = i_InstantiatedBones[l_BoneId];
            l_Frame.m_BoneNode = l_BoneNode;
            if (l_BoneNode) {
                ASSERTLE_Z(
                    i_OriginalBones[l_BoneId] && i_OriginalBones[l_BoneId]->GetName() == l_Frame.m_BoneNode->GetName(),
                    "",
                    451,
                    "OriginalNodes[Id] && OriginalNodes[Id]->GetName()==Frame.pNode->GetName()"
                );
                l_Frame.m_OriginalBoneNode = i_OriginalBones[l_BoneId];
                l_Frame.m_Cct = l_Cct;
                l_Frame.m_BoneTrs = &l_Frame.m_BoneNode->GetTRS();
                AnimationNode_Z::GetCct(i_Times, l_Frame, l_MsgNb);
            }
        }
    }
}
