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

// TODO: Finish matching
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
        if (i_KeyOffset == l_NbKey || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Vec3f T0, P0, P1, T1;
            l_Key->Get(P1);
            l_Key->GetTgtIn(T1);
            l_PrevKey->Get(P0);
            l_PrevKey->GetTgtOut(T0);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Vec3f a1 = P1 * 2; // Why aren't we getting fmadds/fmsubs/fnmsubs ??
            Vec3f b1 = P0 * 2;
            Vec3f c1 = b1 - a1;
            Vec3f d1 = c1 + T0;
            Vec3f e1 = d1 + T1;
            //Vec3f e1 = ((P0*2-P1*2)+T0)+T1; //<- Why does this cause random stores in the stack and blow up stack size

            Vec3f a2 = T0 * 2; // Why aren't we getting fmadds/fmsubs/fnmsubs ??
            Vec3f b2 = P1 * 3;
            //Vec3f c2 = -P0;
            Vec3f d2 = P0 * -3;
            Vec3f e2 = d2 + b2;
            Vec3f f2 = e2 - a2;
            Vec3f g2 = f2 - T1;
            //Vec3f g2 = (((-P0*3)+(P1*3))-(T0*2))-T1; //<- Why does this cause random stores in the stack and blow up stack size

            Vec3f a3 = e1 * t; // Why aren't we getting fmadds
            Vec3f b3 = g2 + a3;
            Vec3f c3 = b3 * t;
            Vec3f d3 = T0 + c3;
            Vec3f f3 = d3 * t;
            o_Value = P0 + f3;
            //o_Value = P0+((T0+((g2+(e1*t))*t))*t); //<- Why does this cause random stores in the stack and blow up stack size
        }
    }

    return i_KeyOffset;
}
