#include "AnimationManager_Z.h"

void AnimationManager_Z::ReserveMsgArray() {
    m_UpdateFlag = 0;
    m_DfltRegMsg.m_SkelPtr = NULL;
    m_DfltRegMsg.m_BoneNodePtr = NULL;
    m_DfltRegMsg.m_NodePtr = NULL;
    // $SABE: ??????
    S32 i = 3;
    U32 l_Ptr = (U32)this + 0xc; // Randomly take offset of m_Enum.m_NbFree (private member btw)
    while (l_Ptr -= 4, i--) {
        *(U32*)(l_Ptr + 0xe0) = 0; // Use the pointer to offset into m_UnkBytes_0x18[4*i] to set them to 0
    }
    m_DfltSkelMsg.m_SkelPtr = NULL;
    m_DfltSkelMsg.m_VsSkelPtr = NULL;
    m_DfltSkelMsg.m_BoneNodePtr = NULL;
    m_DfltSkelMsg.m_VsBoneNodePtr = NULL;
    m_DfltSkelMsg.m_VsAgentPtr = NULL;
    m_DfltSkelMsg.m_CanVsSphereCollide = false;
    m_DfltSkelMsg.m_CanSphereCollide = false;
    m_AnimMsg = NewL_Z(17) RegMessage_Z[512];
    m_BufAnimMsg = NewL_Z(18) RegMessage_Z[512];
    m_BufAnimMsgNb = 0;
    m_AnimMsgNb = 0;
    m_SkelMsg = NewL_Z(22) SkelMessage_Z[256];
    m_BufSkelMsg = NewL_Z(23) SkelMessage_Z[256];
    m_BufSkelMsgNb = 0;
    m_SkelMsgNb = 0;
}
