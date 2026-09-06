#include "Keyframer_Z.h"

void KeyVec3fComp_Z::Set(Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
    m_ValueX = i_Value.x * KEY_FLOAT_COMP_FACTOR;
    m_ValueY = i_Value.y * KEY_FLOAT_COMP_FACTOR;
    m_ValueZ = i_Value.z * KEY_FLOAT_COMP_FACTOR;
    m_TgtInX = i_TgtIn.x * KEY_FLOAT_COMP_FACTOR;
    m_TgtInY = i_TgtIn.y * KEY_FLOAT_COMP_FACTOR;
    m_TgtInZ = i_TgtIn.z * KEY_FLOAT_COMP_FACTOR;
    m_TgtOutX = i_TgtOut.x * KEY_FLOAT_COMP_FACTOR;
    m_TgtOutY = i_TgtOut.y * KEY_FLOAT_COMP_FACTOR;
    m_TgtOutZ = i_TgtOut.z * KEY_FLOAT_COMP_FACTOR;
}

S32 KeyframerVec3fComp_Z::GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyVec3fComp_Z* l_Key = &GetKey(i_KeyOffset) - 1;

    if (l_NbKey == i_KeyOffset) {
        l_Key->Get(o_Value);
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

        KeyVec3fComp_Z* l_PrevKey = &l_Key[-1];
        if (l_NbKey == i_KeyOffset || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Vec3f l_T0, l_P0, l_P1, l_T1;
            l_Key->Get(l_P1);
            l_Key->GetTgtIn(l_T1);
            l_PrevKey->Get(l_P0);
            l_PrevKey->GetTgtOut(l_T0);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Float l_AX = l_P0.x * 2 - l_P1.x * 2 + l_T0.x + l_T1.x;
            Float l_AY = l_P0.y * 2 - l_P1.y * 2 + l_T0.y + l_T1.y;
            Float l_AZ = l_P0.z * 2 - l_P1.z * 2 + l_T0.z + l_T1.z;
            Float l_BX = l_P0.x * -3 + l_P1.x * 3 - l_T0.x * 2 - l_T1.x;
            Float l_BY = l_P0.y * -3 + l_P1.y * 3 - l_T0.y * 2 - l_T1.y;
            Float l_BZ = l_P0.z * -3 + l_P1.z * 3 - l_T0.z * 2 - l_T1.z;
            o_Value.x = l_P0.x + (l_T0.x + (l_BX + t * l_AX) * t) * t;
            o_Value.y = l_P0.y + (l_T0.y + (l_BY + t * l_AY) * t) * t;
            o_Value.z = l_P0.z + (l_T0.z + (l_BZ + t * l_AZ) * t) * t;
        }
    }

    return i_KeyOffset;
}

S32 KeyframerVec3fLinear_Z::GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset) {
    return 0;
}

S32 KeyframerVec3f_Z::GetValue(Float i_Time, Vec3f& o_Value, S32 i_KeyOffset) {
    return 0;
}
