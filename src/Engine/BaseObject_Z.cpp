#include "BaseObject_Z.h"
#include <String_Z.h>
#include "Program_Z.h"
#include "ABC_Agent.h"
#include "ABC_ScriptManager.h"

String_Z<ARRAY_CHAR_MAX> DefaultStringBaseObject;
String_Z<ARRAY_CHAR_MAX> DefaultStringRscBaseObject;

void BaseObject_Z::SetHandle(S32 i_ID, S8 i_Key) {
    m_Handle.m_RealID.Ref.ID = i_ID;
    m_Handle.m_RealID.Ref.Key = i_Key;
}

const BaseObject_ZHdl& BaseObject_Z::GetHandle() const {
    return m_Handle;
}

String_Z<ARRAY_CHAR_MAX>* BaseObject_Z::GetDebugName() const {
    DefaultStringBaseObject.Sprintf("%d", m_Name.m_ID);
    return &DefaultStringBaseObject;
}

String_Z<ARRAY_CHAR_MAX>* BaseObject_Z::GetDebugRscName() const {
    Name_Z l_RscName = gData.ClassMgr->GetHandleName(m_Handle);
    DefaultStringRscBaseObject.Sprintf("%d", l_RscName.m_ID);
    return &DefaultStringRscBaseObject;
}

void BaseObject_Z::Send(abc_message i_Msg, ABC_Agent* i_Receiver, Float i_Param) {
    i_Receiver->Receive(i_Msg, GetHandle(), i_Param);
}

void BaseObject_Z::SendTimed(abc_message i_Msg, ABC_Agent* i_Receiver, Float i_Delay, Float i_Param) {
    gData.ScriptMgr->SendTimed(i_Msg, i_Receiver, i_Delay, i_Param);
}

Bool BaseObject_Z::MarkHandles() {
    return gData.ClassMgr->MarkHandle(GetHandle());
}

void BaseObject_Z::SetName(const Name_Z& i_Name, Bool i_ResToo) {
    m_Name = i_Name;
    if (i_ResToo)
        gData.ClassMgr->ChangeHandleName(GetHandle(), i_Name);
}

void BaseObject_Z::LoadLinks(void** i_Data) {
    gData.ClassMgr->LoadName(m_Name, i_Data);
}
