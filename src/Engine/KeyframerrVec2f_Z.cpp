#include "Keyframer_Z.h"

S32 KeyframerVec2fLinear_Z::GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyVec2fLinear_Z* l_Key = &m_Keys[i_KeyOffset] - 1;

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

        KeyVec2fLinear_Z* l_PrevKey = &l_Key[-1];
        if (i_KeyOffset == l_NbKey || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Vec2f l_Prev;
            Vec2f l_Cur;
            l_Key->Get(l_Cur);
            l_PrevKey->Get(l_Prev);
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            Float l_PrevX = l_Prev.x;
            Float l_CurX = l_Cur.x;
            Float l_CurY = l_Cur.y;
            Float l_PrevY = l_Prev.y;
            o_Value = l_Prev + Vec2f(l_CurX - l_PrevX, l_CurY - l_PrevY) * t;
        }
    }

    return i_KeyOffset;
}

void KeyframerVec2fLinear_Z::GetDerivate(Float i_Time, Vec2f& o_Value) {
    if (m_Flag & FL_KEYFRAMER_SQUARE) {
        o_Value = VEC2F_NULL;
        return;
    }
    S32 l_Idx = 1;
    S32 l_NbKeys = m_Keys.GetSize();
    KeyVec2fLinear_Z* l_Key = m_Keys.GetArrayPtr();
    Float l_PrevTime = l_Key->GetTime();
    l_Key++;
    Float l_CurTime = l_Key->GetTime();

    while (l_Idx < l_NbKeys) {
        if (l_PrevTime <= i_Time && l_CurTime > i_Time)
            break;

        l_Idx++;
        l_Key++;

        if (l_Idx == l_NbKeys)
            break;

        l_PrevTime = l_CurTime;
        l_CurTime = l_Key->GetTime();
    }

    KeyVec2fLinear_Z* l_PrevKey = &l_Key[-1];

    if (l_NbKeys == l_Idx) {
        l_PrevKey->Get(o_Value);
    }
    else {
        l_Key->Get(o_Value);
        Vec2f l_Prev;
        l_PrevKey->Get(l_Prev);
        Float range = (l_CurTime - l_PrevTime);
        Float t = 1 / range;
        o_Value -= l_Prev;
        o_Value *= t;
    }
}

S32 KeyframerVec2fLinearComp_Z::GetValue(Float i_Time, Vec2f& o_Value, S32 i_KeyOffset) {
    S32 l_NbKey = GetNbKeys();
    KeyVec2fLinearComp_Z* l_Key = &GetKey(i_KeyOffset) - 1;

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

        KeyVec2fLinearComp_Z* l_PrevKey = &l_Key[-1];
        if (i_KeyOffset == l_NbKey || (m_Flag & FL_KEYFRAMER_SQUARE)) {
            l_PrevKey->Get(o_Value);
        }
        else {
            Float t = (i_Time - l_PrevTime) / (l_CurTime - l_PrevTime);
            l_Key->Get(o_Value);
            Vec2f l_Prev;
            l_PrevKey->Get(l_Prev);
            o_Value -= l_Prev;
            o_Value *= t;
            o_Value += l_Prev;
        }
    }

    return i_KeyOffset;
}
