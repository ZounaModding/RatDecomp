#include "Keyframer_Z.h"

// TODO: Finish matching
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
            Vec4f l_Prev;
            l_PrevKey->Get(l_Prev);
            Vec4f l_Cur;
            l_Key->Get(l_Cur);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Vec4f def = l_Prev + (Vec4f(l_Cur.x - l_Prev.x, l_Cur.y - l_Prev.y, l_Cur.z - l_Prev.z, 1.0f)) * t;
            o_Value = Vec4f(def.x, def.y, def.z, 1.0f);

            // Also tried this but it's worse???
            // Vec4f l_Prev;
            // l_PrevKey->Get(l_Prev);
            // Vec3f l_Cur;
            // Vec4f abc;
            // l_Key->Get(l_Cur);
            // Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            // Vec3f eee = (l_Cur - l_Prev.xyz());
            // abc = Vec4f(eee);
            // Vec4f ghi = l_Prev + abc * t;
            // o_Value = Vec4f(ghi.x, ghi.y, ghi.z, 1.0f);
        }
    }

    return i_KeyOffset;
}
