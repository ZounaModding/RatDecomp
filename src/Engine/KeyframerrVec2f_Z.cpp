#include "Keyframer_Z.h"

S32 KeyframerVec2fLinear_Z::GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyVec2fLinear_Z* l_Key = &m_Keys[i_KeyOffset] - 1;

    if (l_NbKey == i_KeyOffset) {
        o_Value = l_Key->Get();
    }
    else {
        Float l_PrevTime = l_Key->GetTime();
        l_Key++;
        Float l_CurTime = l_Key->GetTime();

        while (i_KeyOffset < l_NbKey) {
            if (l_PrevTime <= i_Time && l_CurTime > i_Time)
                break;

            i_KeyOffset++;
            l_Key++;

            if (i_KeyOffset == l_NbKey)
                break;

            l_PrevTime = l_CurTime;
            l_CurTime = l_Key->GetTime();
        }

        KeyVec2fLinear_Z* l_PrevKey = l_Key - 1;
        const Vec2f& l_Prev = l_PrevKey->Get();
        const Vec2f& l_Cur = l_Key->Get();
        if (i_KeyOffset == l_NbKey || (m_Flag & 0x8)) {
            o_Value = l_Prev;
        }
        else {
            o_Value = l_Prev + (l_Cur - l_Prev) * ((i_Time - l_PrevTime) / (l_CurTime - l_PrevTime));
        }
    }

    return i_KeyOffset;
}

void KeyframerVec2fLinear_Z::GetDerivate(Float i_Time, Vec2f& o_Value) {
}

S32 KeyframerVec2fLinearComp_Z::GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset) {
}
