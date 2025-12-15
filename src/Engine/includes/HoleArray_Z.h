#ifndef _HOLEARRAY_Z_H_
#define _HOLEARRAY_Z_H_
#include "BitArray_Z.h"

template <class T, S32 Granularity = 32>
class HoleArray_Z {
    DynArray_Z<T, Granularity> m_DA;
    BitArray_Z m_BA;
    S32 m_Size;
    S32 m_NbElement;

public:
    HoleArray_Z() {
        m_Size = 0;
        m_NbElement = 0;
    }

    ~HoleArray_Z() {
        for (int i = 0; i < m_Size; i++) {
            Remove(i);
        }
    }

    S32 GetSize() const { return m_Size; }

    U32 IsElement(S32 i_Index) const {
        if (i_Index < 0 || i_Index >= m_Size) {
            return 0;
        }
        else {
            return m_BA.GetBit(i_Index);
        }
    }

    S32 FindFirst() const {
        if (m_Size == 0) return -1;
        return m_BA.FindFirstBit();
    }

    S32 FindNext(S32 i_Cur) const {
        i_Cur++;
        if (i_Cur >= m_Size) return -1;
        return m_BA.FindFirstBit(TRUE, i_Cur);
    }

    S32 Add() {
        S32 l_Free;
        if (m_Size == 0)
            l_Free = -1;
        else
            l_Free = m_BA.FindFirstBit(FALSE);
        m_NbElement++;
        if (l_Free == -1) {
            m_DA.Add();
            m_BA.SetSize(m_Size + 1);
            m_BA.SetBit(m_Size);
            return m_Size++;
        }
        else {
            new (&m_DA[l_Free]) T;
            m_BA.SetBit(l_Free);
            return l_Free;
        }
    }

    S32 Add(const T& i_Elem) {
        S32 l_Free;
        if (m_Size == 0)
            l_Free = -1;
        else
            l_Free = m_BA.FindFirstBit(FALSE);
        m_NbElement++;
        if (l_Free == -1) {
            S32 l = m_DA.Add(i_Elem);
            m_BA.SetSize(m_Size + 1);
            m_BA.SetBit(m_Size);
            return m_Size++;
        }
        else {
            m_DA[l_Free] = i_Elem;
            m_BA.SetBit(l_Free);
            return l_Free;
        }
    }

    Bool Remove(S32 i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        if (m_BA.GetBit(i_Index)) {
            m_DA[i_Index].~T();
            m_BA.ClearBit(i_Index);
            m_NbElement--;
            return TRUE;
        }
        else {
            return FALSE;
        }
    }

    void Minimize() {
        S32 l_LastID = m_BA.FindLastBit(TRUE, m_BA.m_Size - 1);
        if (l_LastID == -1) {
            m_DA.Flush();
            m_BA.Flush();
            m_Size = 0;
            m_NbElement = 0;
        }
        else {
            m_Size = l_LastID + 1;
            m_DA.SetSize(m_Size);
            m_BA.SetSize(m_Size);
            m_DA.Minimize();
            m_BA.Minimize();
        }
    }

    T& Get(S32 i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_DA[i_Index];
    }

    const T& Get(S32 i_Index) const {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_DA[i_Index];
    }

    T& operator[](int i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return Get(i_Index);
    }

    const T& operator[](int i_Index) const {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return Get(i_Index);
    }
};

#endif
