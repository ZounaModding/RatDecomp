#include "Keyframer_Z.h"
#include "Program_Z.h"

S32 KeyframerHdl_Z::GetValue(Float i_Time, BaseObject_ZHdl& o_Value, S32 i_KeyOffset) {
    while (i_KeyOffset < GetNbKeys()) {
        if ((m_Keys[i_KeyOffset - 1].GetTime() <= i_Time) && (m_Keys[i_KeyOffset].GetTime() > i_Time)) break;
        i_KeyOffset++;
    }
    o_Value = m_Keys[i_KeyOffset - 1].m_Value;
    return i_KeyOffset;
}

void KeyframerHdl_Z::MarkHandles() {
    for (S32 i = 0; i < GetNbKeys(); i++) {
        if (m_Keys[i].m_Value.IsValid()) {
            gData.ClassMgr->GetPtr(m_Keys[i].m_Value)->MarkHandles();
        }
    }
}
