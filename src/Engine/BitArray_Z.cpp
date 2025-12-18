#include "BitArray_Z.h"

BitArray_Z::BitArray_Z() {
    m_Size = 0;
}

BitArray_Z::~BitArray_Z() {
}

BitArray_Z::BitArray_Z(S32 i_Size) {
    SetSize(i_Size);
}

// $VIOLET: Never used! but seems its needed to link properly
BitArray_Z::BitArray_Z(const BitArray_Z& i_Src) {
    DYNARRAY_Z_EXP(&i_Src != this);
    m_BitsDA = i_Src.m_BitsDA;
    m_Size = i_Src.m_Size;
}

BitArray_Z& BitArray_Z::operator=(const BitArray_Z& i_Src) {
    DYNARRAY_Z_EXP(&i_Src != this);
    m_BitsDA = i_Src.m_BitsDA;
    m_Size = i_Src.m_Size;
    return *this;
}

void BitArray_Z::ClearBitRange(S32 i_StartIndex, S32 i_EndIndex) {
    DYNARRAY_Z_EXP(i_StartIndex < m_Size);
    DYNARRAY_Z_EXP(i_EndIndex < m_Size);
    DYNARRAY_Z_EXP(i_StartIndex < i_EndIndex);
    S32 l_Start = i_StartIndex >> 5;
    S32 l_BitCpt = i_StartIndex & 0x1F;
    U32 l_Bit = (1 << l_BitCpt) - 1;

    S32 l_End = i_EndIndex >> 5;
    U32 l_BitCptEnd = i_EndIndex & 0x1F;
    U32 l_BitEnd = ~(2 * ((1 << l_BitCptEnd)) - 1);

    if (l_Start == l_End) {
        m_BitsDA[l_Start] &= l_Bit | l_BitEnd;
        return;
    }

    m_BitsDA[l_Start] &= l_Bit;

    if (l_Start < l_End) {
        l_Start++;
        for (; l_Start < l_End; m_BitsDA[l_Start] = 0, l_Start++);
    }
    m_BitsDA[l_End] &= l_BitEnd;
}

void BitArray_Z::SetAllBits() {
    U32* l_Bits = m_BitsDA.GetArrayPtr();
    S32 l_Size = m_BitsDA.GetSize();
    while (l_Size--) {
        *l_Bits++ = 0xFFFFFFFF;
    }
}

void BitArray_Z::ClearAllBits() {
    U32* l_Bits = m_BitsDA.GetArrayPtr();
    S32 l_Size = m_BitsDA.GetSize();
    while (l_Size--) {
        *l_Bits++ = 0;
    }
}

S32 BitArray_Z::FindFirstBit(Bool i_State, S32 i_FirstBitToCheck) const {
    DYNARRAY_Z_EXP(i_FirstBitToCheck <= m_Size);
    if (i_FirstBitToCheck >= m_Size) {
        return -1;
    }
    int l_Index;
    int l_CurBits;
    S32 l_BitIndex = i_FirstBitToCheck >> 5;
    S32 l_BitCpt = i_FirstBitToCheck & 0x1F;
    int l_Size = m_BitsDA.GetSize();
    if (l_BitCpt) {
        U32 l_Bit = 1 << l_BitCpt;
        l_CurBits = m_BitsDA[l_BitIndex];
        if (!i_State)
            l_CurBits = ~l_CurBits;
        l_CurBits &= ~(l_Bit - 1);
        if (l_CurBits) {
            S32 x = ((l_CurBits) & (-l_CurBits));
            S32 a = 0;
            if (x & 0xFFFF0000) a += 16;
            if (x & 0xFF00FF00) a += 8;
            if (x & 0xF0F0F0F0) a += 4;
            if (x & 0xCCCCCCCC) a += 2;
            if (x & 0xAAAAAAAA) a += 1;
            l_Index = (l_BitIndex << 5) + a;
            if (l_Index >= m_Size) {
                return -1;
            }
            return l_Index;
        }
        l_BitIndex++;
    }

    if (i_State) {
        for (; (l_BitIndex < l_Size) && (!(l_CurBits = m_BitsDA[l_BitIndex])); l_BitIndex++);
        if (l_BitIndex < l_Size) {
            S32 x = ((l_CurBits) & (-l_CurBits));
            S32 a = 0;
            if (x & 0xFFFF0000) a += 16;
            if (x & 0xFF00FF00) a += 8;
            if (x & 0xF0F0F0F0) a += 4;
            if (x & 0xCCCCCCCC) a += 2;
            if (x & 0xAAAAAAAA) a += 1;
            l_Index = (l_BitIndex << 5) + a;
            if (l_Index >= m_Size) {
                return -1;
            }
            return l_Index;
        }
    }
    else {
        for (; (l_BitIndex < l_Size) && (!(l_CurBits = ~m_BitsDA[l_BitIndex])); l_BitIndex++);
        if (l_BitIndex < l_Size) {
            S32 x = ((l_CurBits) & (-l_CurBits));
            S32 a = 0;
            if (x & 0xFFFF0000) a += 16;
            if (x & 0xFF00FF00) a += 8;
            if (x & 0xF0F0F0F0) a += 4;
            if (x & 0xCCCCCCCC) a += 2;
            if (x & 0xAAAAAAAA) a += 1;
            l_Index = (l_BitIndex << 5) + a;
            if (l_Index >= m_Size) {
                return -1;
            }
            return l_Index;
        }
    }
    return -1;
}

S32 BitArray_Z::FindLastBit(Bool i_State, S32 i_FirstBitToCheck) const {
    DYNARRAY_Z_EXP(i_FirstBitToCheck <= m_Size);
    while (i_FirstBitToCheck >= 0) {
        if (i_State) {
            if (GetBit(i_FirstBitToCheck)) {
                break;
            }
        }
        else {
            if (!GetBit(i_FirstBitToCheck)) {
                break;
            }
        }
        i_FirstBitToCheck--;
    }
    return i_FirstBitToCheck;
}
