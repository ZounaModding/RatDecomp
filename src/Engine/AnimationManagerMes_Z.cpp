#include "AnimationManager_Z.h"

void AnimationManager_Z::ReserveMsgArray() {
    m_UpdateFlag = 0;
    // Inline most likely
    m_DfltRegMsg.m_SkelPtr = NULL;
    m_DfltRegMsg.m_BoneNodePtr = NULL;
    m_DfltRegMsg.m_NodePtr = NULL;
    // $SABE: ??????
    S32 i = 3;
    U32 l_Ptr = (U32)this + 0xc; // Randomly take offset of m_Enum.m_NbFree (private member btw)
    while (l_Ptr -= 4, i--) {
        *(U32*)(l_Ptr + 0xe0) = 0; // Use the pointer to offset into m_UnkBytes_0x18[4*i] to set them to 0
    }
    // Finish inline
    m_DfltSkelMsg.m_SkelPtr = NULL;
    m_DfltSkelMsg.m_VsSkelPtr = NULL;
    m_DfltSkelMsg.m_BoneNodePtr = NULL;
    m_DfltSkelMsg.m_VsBoneNodePtr = NULL;
    m_DfltSkelMsg.m_VsAgentPtr = NULL;
    m_DfltSkelMsg.m_CanVsSphereCollide = false;
    m_DfltSkelMsg.m_CanSphereCollide = false;
    m_AnimMsg = NewL_Z(17) RegMessage_Z[MAX_REGMSG_ARRAY_SIZE];
    m_BufAnimMsg = NewL_Z(18) RegMessage_Z[MAX_REGMSG_ARRAY_SIZE];
    m_BufAnimMsgNb = 0;
    m_AnimMsgNb = 0;
    m_SkelMsg = NewL_Z(22) SkelMessage_Z[MAX_SKELMSG_ARRAY_SIZE];
    m_BufSkelMsg = NewL_Z(23) SkelMessage_Z[MAX_SKELMSG_ARRAY_SIZE];
    m_BufSkelMsgNb = 0;
    m_SkelMsgNb = 0;
}

void AnimationManager_Z::ReleaseMsgArray() {
    if (m_SkelMsg) {
        Delete_Z[] m_SkelMsg;
    }
    if (m_BufSkelMsg) {
        Delete_Z[] m_BufSkelMsg;
    }
    if (m_AnimMsg) {
        Delete_Z[] m_AnimMsg;
    }
    if (m_BufAnimMsg) {
        Delete_Z[] m_BufAnimMsg;
    }
    m_BufSkelMsg = NULL;
    m_SkelMsg = NULL;
    m_BufAnimMsg = NULL;
    m_AnimMsg = NULL;
}

void AnimationManager_Z::SwapMsgArray() {
    SkelMessage_Z* l_TempSkelMsg = m_SkelMsg;
    m_SkelMsg = m_BufSkelMsg;
    m_BufSkelMsg = l_TempSkelMsg;
    m_SkelMsgNb = m_BufSkelMsgNb;
    RegMessage_Z* l_TempRegMsg = m_AnimMsg;
    m_AnimMsg = m_BufAnimMsg;
    m_BufAnimMsg = l_TempRegMsg;
    m_AnimMsgNb = m_BufAnimMsgNb;
    m_BufSkelMsgNb = 0;
    m_BufAnimMsgNb = 0;
}

RegMessage_Z& AnimationManager_Z::GetRegMessage(S32 i_Idx) {
    if (i_Idx < 0 || i_Idx >= MAX_REGMSG_ARRAY_SIZE || i_Idx >= m_AnimMsgNb) {
        return m_DfltRegMsg;
    }
    return m_AnimMsg[i_Idx];
}

S32 AnimationManager_Z::GetMaxRegMessage() {
    return MAX_REGMSG_ARRAY_SIZE;
}

RegMessage_Z* AnimationManager_Z::GetRegMessage() {
    return m_BufAnimMsg;
}

S32& AnimationManager_Z::GetRegMessageNb() {
    if (m_BufAnimMsgNb >= MAX_REGMSG_ARRAY_SIZE) {
        PrintfMsgAndCount();
        ASSERTLE_Z(m_BufAnimMsgNb < MAX_REGMSG_ARRAY_SIZE, "Increase MAX_REGMSG_ARRAY_SIZE", 94, "BufAnimMsgNb<MAX_REGMSG_ARRAY_SIZE");
    }
    return m_BufAnimMsgNb;
}

void AnimationManager_Z::PrintfMsgAndCount() {
    S32DA l_MsgCountById;
    RegMessage_Z* l_Msg = m_BufAnimMsg;
    S32 i;
    S32 l_MsgNb = m_BufAnimMsgNb;
    for (i = 0; i < l_MsgNb; i++, l_Msg++) {
        S32 l_OldSize = l_MsgCountById.GetSize();
        if (l_Msg->m_Id >= l_OldSize) {
            l_MsgCountById.SetSize(l_Msg->m_Id + 1);
            while (l_OldSize < l_MsgCountById.GetSize()) {
                l_MsgCountById[l_OldSize] = 0;
                l_OldSize++;
            }
        }
        l_MsgCountById[l_Msg->m_Id]++;
    }
    S32* l_Ptr = l_MsgCountById.GetArrayPtr();
    for (i = l_MsgCountById.GetSize(); i > 0; i--) {
        // $SABE: In a non master build here they would presumably print the message counts per ID
        l_Ptr++;
    }
}

S32& AnimationManager_Z::GetSkelMessageNb() {
    return m_BufSkelMsgNb;
}

SkelMessage_Z* AnimationManager_Z::GetSkelMessage() {
    return m_BufSkelMsg;
}
