#include "AnimationManager_Z.h"

S32 AnimationManager_Z::AddSkelId(const Name_Z& i_SkelName) {
    for (S32 i = m_SkelOriginalNodes.FindFirst(); i >= 0; i = m_SkelOriginalNodes.FindNext(i)) {
        if (m_SkelOriginalNodes[i].m_SkelName == i_SkelName) {
            m_SkelOriginalNodes[i].m_RefCount++;
            return i;
        }
    }
    S32 l_SkelId = m_SkelOriginalNodes.Add();
    m_SkelOriginalNodes[l_SkelId].m_SkelName = i_SkelName;
    m_SkelOriginalNodes[l_SkelId].m_RefCount = 1;
    return l_SkelId;
}

void AnimationManager_Z::SetOriginalArray(S32 i_SkelId, BoneNode_Z* i_OriginalBones) {
    m_SkelOriginalNodes[i_SkelId].m_BonesArrayPtr = i_OriginalBones;
    m_SkelOriginalNodes[i_SkelId].m_RootBonePtr = i_OriginalBones->GetRoot();
}

void AnimationManager_Z::RemoveSkelNodes(S32 i_SkelId) {
    m_SkelOriginalNodes[i_SkelId].m_RefCount--;
    if (m_SkelOriginalNodes[i_SkelId].m_RefCount == 0) {
        m_SkelOriginalNodes.Remove(i_SkelId);
    }
}

void AnimationManager_Z::MarkSkelHandles() {
    for (S32 i = m_SkelOriginalNodes.FindFirst(); i >= 0; i = m_SkelOriginalNodes.FindNext(i)) {
        if (m_SkelOriginalNodes[i].m_RefCount != 0) {
            m_SkelOriginalNodes[i].m_RootBonePtr->MarkHandles();
        }
    }
}
