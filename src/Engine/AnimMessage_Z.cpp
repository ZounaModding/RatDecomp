#include "AnimMessage_Z.h"
#include "Program_Z.h"

// TODO: Finish matching
S32 KeyframerMessage_Z::Get(
    Float i_StartTime,
    Float i_CurTime,
    Float i_MaxTime,
    const Message_Z& i_Msg,
    RegMessage_Z& o_Value
) {
    S32 l_KeyIndex0 = -1;
    S32 l_KeyIndex1 = -1;
    S32 l_Idx;
    for (l_Idx = 0; l_Idx < GetNbKeys(); l_Idx++) {
        if (m_Keys[l_Idx].GetTime() >= i_StartTime) {
            l_KeyIndex0 = l_Idx;
            break;
        }
    }
    if (i_CurTime >= i_MaxTime) {
        l_KeyIndex1 = GetNbKeys() - 1;
    }
    else {
        for (l_Idx = GetNbKeys() - 1; l_Idx >= 0; l_Idx--) {
            if (m_Keys[l_Idx].GetTime() < i_CurTime) {
                l_KeyIndex1 = l_Idx;
                break;
            }
        }
    }
    if (l_KeyIndex1 >= 0 && l_KeyIndex0 >= 0) {
        const KeyMessage_Z* l_CurKey = &m_Keys[l_KeyIndex0];
        for (l_Idx = l_KeyIndex0; l_Idx <= l_KeyIndex1; l_Idx++, l_CurKey++) {
            Float l_Time = l_CurKey->GetTime();
            S32 l_Size = l_CurKey->m_Messages.GetSize();
            Message_Z* l_CurMsg = l_CurKey->m_Messages.GetArrayPtr();
            for (S32 l_MsgIdx = 0; l_MsgIdx < l_Size; l_MsgIdx++, l_CurMsg++) {
                if (l_CurMsg->m_Id == i_Msg.m_Id && l_CurMsg->m_U32Param == i_Msg.m_U32Param) {
                    o_Value = *l_CurMsg;
                    o_Value.m_Time = l_Time;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// TODO: Finish matching
S32 KeyframerMessage_Z::GetValue(
    Float i_StartTime,
    Float i_CurTime,
    Float i_MaxTime,
    RegMessage_Z* o_Value,
    S32& io_MsgNb,
    S32 i_MsgId
) const {
    if (i_CurTime < i_StartTime) {
        GetValue(i_StartTime, i_MaxTime, i_MaxTime, o_Value, io_MsgNb, -1);
        return GetValue(0, i_CurTime, i_MaxTime, o_Value, io_MsgNb, -1);
    }
    S32 l_KeyIndex0 = -1;
    S32 l_KeyIndex1 = -1;
    S32 l_Idx;
    for (l_Idx = 0; l_Idx < GetNbKeys(); l_Idx++) {
        if (m_Keys[l_Idx].GetTime() >= i_StartTime) {
            l_KeyIndex0 = l_Idx;
            break;
        }
    }
    if (i_CurTime >= i_MaxTime) {
        l_KeyIndex1 = GetNbKeys() - 1;
    }
    else {
        for (l_Idx = GetNbKeys() - 1; l_Idx >= 0; l_Idx--) {
            if (m_Keys[l_Idx].GetTime() < i_CurTime) {
                l_KeyIndex1 = l_Idx;
                break;
            }
        }
    }
    if (l_KeyIndex1 >= 0 && l_KeyIndex0 >= 0) {
        const KeyMessage_Z* l_CurKey = &m_Keys[l_KeyIndex0];
        for (l_Idx = l_KeyIndex0; l_Idx <= l_KeyIndex1; l_Idx++, l_CurKey++) {
            Float l_Time = l_CurKey->GetTime();
            S32 l_Size = l_CurKey->m_Messages.GetSize();
            Message_Z* l_CurMsg = l_CurKey->m_Messages.GetArrayPtr();
            for (S32 l_MsgIdx = 0; l_MsgIdx < l_Size; l_MsgIdx++, l_CurMsg++) {
                if (i_MsgId < 0 || l_CurMsg->m_Id == i_MsgId) {
                    o_Value[io_MsgNb] = *l_CurMsg;
                    o_Value[io_MsgNb++].m_Time = l_Time;
                }
            }
        }
    }
    return l_KeyIndex1;
}

// TODO: Finish matching
S32 KeyframerMessage_Z::GetCctValue(
    S32 i_StartKey,
    S32 i_KeyCount,
    Float i_StartTime,
    Float i_CurTime,
    Float i_MaxTime,
    RegMessage_Z* o_Value,
    S32& io_MsgNb
) const {
    if (i_CurTime < i_StartTime) {
        GetCctValue(i_StartKey, i_KeyCount, i_StartTime, i_MaxTime, i_MaxTime, o_Value, io_MsgNb);
        return GetCctValue(i_StartKey, i_KeyCount, 0, i_CurTime, i_MaxTime, o_Value, io_MsgNb);
    }
    S32 l_KeyIndex0 = -1;
    S32 l_KeyIndex1 = -1;
    S32 l_Idx;
    S32 l_LastKey = i_StartKey + i_KeyCount;
    for (l_Idx = i_StartKey; l_Idx < l_LastKey; l_Idx++) {
        if (m_Keys[l_Idx].GetTime() >= i_StartTime) {
            l_KeyIndex0 = l_Idx;
            break;
        }
    }
    if (i_CurTime >= i_MaxTime) {
        l_KeyIndex1 = i_StartKey + i_KeyCount - 1;
    }
    else {
        for (l_Idx = l_LastKey - 1; l_Idx >= i_StartKey; l_Idx--) {
            if (m_Keys[l_Idx].GetTime() < i_CurTime) {
                l_KeyIndex1 = l_Idx;
                break;
            }
        }
    }
    if (l_KeyIndex1 >= 0 && l_KeyIndex0 >= 0) {
        const KeyMessage_Z* l_CurKey = &m_Keys[l_KeyIndex0];
        for (l_Idx = l_KeyIndex0; l_Idx <= l_KeyIndex1; l_Idx++, l_CurKey++) {
            Float l_Time = l_CurKey->GetTime();
            S32 l_Size = l_CurKey->m_Messages.GetSize();
            Message_Z* l_CurMsg = l_CurKey->m_Messages.GetArrayPtr();
            for (S32 l_MsgIdx = 0; l_MsgIdx < l_Size; l_MsgIdx++, l_CurMsg++) {
                o_Value[io_MsgNb] = *l_CurMsg;
                o_Value[io_MsgNb++].m_Time = l_Time;
            }
        }
    }
    return l_KeyIndex1;
}

// TODO: Finish matching
void KeyframerMessage_Z::Load(void** i_Data) {
    S32 l_KeyNb;
    LOADDYNARRAYSIZENB_Z(m_Keys, l_KeyNb);
    for (S32 i = 0; i < l_KeyNb; i++) {
        S32 l_MsgNb;
        LOAD_Z(m_Keys[i].m_Time);
        LOAD_Z(l_MsgNb);
        m_Keys[i].SetNb(l_MsgNb);
        for (S32 j = 0; j < l_MsgNb; j++) {
            // TODO: Variation of LOADRANGE_Z, if it's used in many more places consider making a macro
            LOADBYTES_Z(&m_Keys[i].m_Messages[j].m_Id, ((U8*)&m_Keys[i].m_Messages[j].m_NameParam - (U8*)&m_Keys[i].m_Messages[j].m_Id));
            // Can't get it to load a ptr to the dyn array into a register (addi r6, r31, 0x4),
            // instead it just ends up loading the dyn array's message pointer directly (addi r0, r28, 0x8)
            Message_ZDA& msgs = m_Keys[i].GetMessages();
            gData.ClassMgr->LoadName(msgs[j].m_NameParam, i_Data);
        }
    }
}

void KeyframerMessage_Z::MarkHandles() {
    S32 i;
    S32 j;
    KeyMessage_ZDA& l_Keys = GetKeys();
    for (i = 0; i < l_Keys.GetSize(); i++) {
        KeyMessage_Z& l_CurKey = l_Keys[i];
        for (j = 0; j < l_CurKey.GetNb(); j++) {
            if (l_CurKey.IsLinkHdl(j)) {
                gData.ClassMgr->MarkU32Handle(l_CurKey.Get(j).m_U32Param);
            }
        }
    }
}

void KeyframerMessage_Z::EndLoad() {
    S32 i;
    S32 j;
    KeyMessage_ZDA& l_Keys = GetKeys();
    for (i = 0; i < l_Keys.GetSize(); i++) {
        KeyMessage_Z& l_CurKey = l_Keys[i];
        for (j = 0; j < l_CurKey.GetNb(); j++) {
            if (l_CurKey.IsLinkHdl(j)) {
                BaseObject_ZHdl l_Hdl;
                gData.ClassMgr->UpdateLinkFromId(l_Hdl, l_CurKey.Get(j).m_U32Param);
                l_CurKey.Get(j).m_U32Param = gData.ClassMgr->HandleToU32(l_Hdl);
            }
        }
    }
}

void KeyframerMessage_Z::EndLinks(const ResourceObjectLink_Z& i_ResObjLink) {
    S32 i;
    S32 j;
    KeyMessage_ZDA& l_Keys = GetKeys();
    for (i = 0; i < GetNbKeys(); i++) {
        KeyMessage_Z& l_CurKey = l_Keys[i];
        for (j = 0; j < l_CurKey.GetNb(); j++) {
            if (l_CurKey.IsLinkHdl(j)) {
                BaseObject_ZHdl l_Hdl;
                i_ResObjLink.UpdateLinkFromId(l_Hdl, l_CurKey.Get(j).m_U32Param);
                l_CurKey.Get(j).m_U32Param = gData.ClassMgr->HandleToU32(l_Hdl);
            }
        }
    }
}
