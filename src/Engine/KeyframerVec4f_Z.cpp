#include "Keyframer_Z.h"

S32 KeyframerVec4fLinear_Z::GetValue(Float i_Time, Vec4f& o_Value, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyVec4fLinear_Z* l_Key = &m_Keys[i_KeyOffset] - 1;

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

        KeyVec4fLinear_Z* l_PrevKey = &l_Key[-1];
        if (i_KeyOffset == l_NbKey || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Vec3f l_Prev;
            Vec3f l_Cur;
            l_Key->Get(l_Cur);
            l_PrevKey->Get(l_Prev);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Float l_PrevX = l_Prev.x;
            Float l_CurX = l_Cur.x;
            Float l_CurY = l_Cur.y;
            Float l_PrevY = l_Prev.y;
            Float l_CurZ = l_Cur.z;
            Float l_PrevZ = l_Prev.z;
            Vec3f l_Delta(l_CurX - l_PrevX, l_CurY - l_PrevY, l_CurZ - l_PrevZ);
            o_Value = l_Prev + Vec4f(l_Delta) * t;
        }
    }

    return i_KeyOffset;
}

S32 KeyframerVec4fLinearComp_Z::GetValue(Float i_Time, Vec4f& o_Value, S32 i_KeyOffset) {
    return 0;
}
