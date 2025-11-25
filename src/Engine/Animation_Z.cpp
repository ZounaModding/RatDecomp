#include "Animation_Z.h"
#include "Types_Z.h"

Bool Animation_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    m_NodeKfr.m_MsgKfr.MarkHandles();
    return TRUE;
}

S32 Animation_Z::GetIndexOfNodeById(S32 i_NodeId) const {
    AnimationConcatNode_Z* l_ArrayPtr = m_NodeCct.GetArrayPtr();
    for (S32 i = 0; i < m_NodeCct.GetSize(); i++) {
        if (i_NodeId == l_ArrayPtr->GetBoneId()) {
            return i;
        }
        l_ArrayPtr++;
    }
    return -1;
}

// TODO: Match
Float Animation_Z::GetTimeFromMessage(S32 i_MsgId, S32 i_NodeId, Float i_StartTime) {
    S32 l_Idx = 0;
    if (i_NodeId >= 0) {
        l_Idx = GetIndexOfNodeById(i_NodeId);
        if (l_Idx < 0) return -1.0f;
    }
    for (S32 Index2 = l_Idx; Index2 < m_NodeCct.GetSize(); Index2++) {
        for (S16 l_Cur = m_NodeCct[Index2].GetMsgStart(), l_Nb = m_NodeCct[Index2].GetMsgNb(); l_Nb != 0, l_Nb--; l_Cur++) {
            KeyMessage_Z& Key = m_NodeKfr.m_MsgKfr.GetKeys()[l_Cur];
            for (S32 MsgIndex = 0; MsgIndex < Key.GetNb(); MsgIndex++) {
                if (Key.Get(MsgIndex).m_Id == i_MsgId && Key.GetTime() >= i_StartTime) {
                    return Key.GetTime();
                }
            }
        }
    }
    return -1.0f;
}
