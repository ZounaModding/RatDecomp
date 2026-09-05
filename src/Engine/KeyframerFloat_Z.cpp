#include "Keyframer_Z.h"

// TODO: Finish matching
S32 KeyframerFloatComp_Z::GetValue(Float i_Time, Float& o_Value, Bool& o_Side, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyFloatComp_Z* l_Key = &GetKey(i_KeyOffset) - 1;
    Float l_Slope = 0.0f;

    if (l_NbKey == i_KeyOffset) {
        l_Key->Get(o_Value);
        l_Key->GetTgtIn(l_Slope);
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

        KeyFloatComp_Z* l_PrevKey = &l_Key[-1];
        if (i_KeyOffset == l_NbKey || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Float l_T0, l_P0, l_P1, l_T1;
            l_Key->Get(l_P1);
            l_Key->GetTgtIn(l_T1);
            l_PrevKey->Get(l_P0);
            l_PrevKey->GetTgtOut(l_T0);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Float l_A = l_P0 * 2 - l_P1 * 2 + l_T0 + l_T1;
            Float l_B = l_P0 * -3 + l_P1 * 3 - l_T0 * 2 - l_T1;
            o_Value = l_P0 + (l_T0 + (l_B + t * l_A) * t) * t;
            l_Slope = l_T0 + (l_B * 2 + 3 * t * l_A) * t;
        }
    }

    if (l_Slope >= 0.0f) {
        o_Side = TRUE;
    }
    else {
        o_Side = FALSE;
    }

    return i_KeyOffset;
}

S32 KeyframerFloatLinearComp_Z::GetValue(Float i_Time, Float& o_Value, S32 i_KeyOffset) {
    return 0;
}
