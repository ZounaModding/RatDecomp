#include "Keyframer_Z.h"

S32 KeyframerFlag_Z::GetValue(Float i_Time, U32& o_Value, S32 i_KeyOffset) {
    while (i_KeyOffset < GetNbKeys()) {
        if ((m_Keys[i_KeyOffset - 1].GetTime() <= i_Time) && (m_Keys[i_KeyOffset].GetTime() > i_Time)) break;
        i_KeyOffset++;
    }
    o_Value = m_Keys[i_KeyOffset - 1].m_Value;
    return i_KeyOffset;
}
