#ifndef _BITARRAY_Z_H_
#define _BITARRAY_Z_H_
#include "DynArray_Z.h"
#include "Types_Z.h"

/* sizeof(BitArray_Z) == 0xC*/
class BitArray_Z {
public:
    U32DA m_BitsDA;
    S32 m_Size;

    BitArray_Z();
    BitArray_Z(S32 i_Size);
    BitArray_Z(const BitArray_Z& i_Src);
    ~BitArray_Z();
    BitArray_Z& operator=(const BitArray_Z& i_Src);

    void SetAllBits();
    void ClearAllBits();
    void ClearBitRange(S32 i_StartIndex, S32 i_EndIndex);
    S32 FindFirstBit(Bool i_State = TRUE, S32 i_firstBitToCheck = 0) const;
    S32 FindLastBit(Bool i_State, S32 i_firstBitToCheck) const;

    inline void Minimize() {
        m_BitsDA.Minimize();
    }

    inline void Flush() {
        m_BitsDA.Flush();
        m_Size = 0;
    }

    inline void SetSize(S32 i_Size) {
        m_Size = i_Size;
        if (i_Size && (i_Size >> 5) + 1 > m_BitsDA.GetSize()) {
            m_BitsDA.SetSize((m_Size >> 5) + 1);
        }
    }

    inline S32 GetSize() const { return m_Size; }

    // TODO: When all uses of GetBit (inlined) are decompiled, make it match properly
    inline U32 GetBit(S32 i_Bit) const {
        DYNARRAY_Z_EXP(i_Bit < m_Size);
        return m_BitsDA[i_Bit >> 5] & (1 << (i_Bit & 0x1F));
    }

    inline void SetBit(S32 i_Bit) {
        DYNARRAY_Z_EXP(i_Bit < m_Size);
        m_BitsDA[i_Bit >> 5] |= (1 << (i_Bit & 0x1F));
    }

    inline void ClearBit(S32 i_Bit) {
        DYNARRAY_Z_EXP(i_Bit < m_Size);
        m_BitsDA[i_Bit >> 5] &= ~(U32)(1 << (i_Bit & 0x1F));
    }
};

#endif
