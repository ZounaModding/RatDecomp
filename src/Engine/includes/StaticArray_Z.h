#ifndef _STATICARRAY_Z_H_
#define _STATICARRAY_Z_H_

#include "DynArray_Z.h"

template <class T, int ReservedSize, Bool DeleteObject = TRUE, Bool InitObject = TRUE>
class StaticArray_Z {
private:
    Char m_ArrayChar[ReservedSize * sizeof(T)];
    int m_Size;

public:
    StaticArray_Z() {
        m_Size = 0;
    }

    StaticArray_Z(T i_Values[], S32 i_Size) {
        m_Size = 0;
        for (int i = 0; i < i_Size; i++)
            Add(i_Values[i]);
    }

    StaticArray_Z(const StaticArray_Z<T, ReservedSize>& i_Src) {
        m_Size = 0;
        for (int i = 0; i < i_Src.GetSize(); i++)
            Add(i_Src[i]);
    }

    ~StaticArray_Z() {
        if (DeleteObject) {
            for (int i = 0; i < m_Size; i++) {
                Get(i).~T();
            }
        }
    }

    T& Get(int i_Index) {
        return *(T*)(m_ArrayChar + i_Index * sizeof(T));
    }

    const T& Get(int i_Index) const {
        return *(T*)(m_ArrayChar + i_Index * sizeof(T));
    }

    T& operator[](int i_Index) {
        return Get(i_Index);
    }

    const T& operator[](int i_Index) const {
        return Get(i_Index);
    }

    int GetSize() const {
        return m_Size;
    }

    int Add() {
        if (m_Size >= ReservedSize) {
            DYNARRAY_Z_EXP(m_Size < ReservedSize);
            // $SABE: Bug?
#ifdef BUGFIXES_Z
            if (DeleteObject)
                Get(ReservedSize - 1).~T();
#endif
            if (InitObject)
                new (&Get(ReservedSize - 1)) T();
            return ReservedSize - 1;
        }

        if (InitObject)
            new (&Get(m_Size++)) T();
        else
            m_Size++;
        return m_Size - 1;
    }

    int Add(const T& i_Ele) {
        if (m_Size >= ReservedSize) {
            DYNARRAY_Z_EXP(m_Size < ReservedSize);
            // $SABE: Bug?
#ifdef BUGFIXES_Z
            if (DeleteObject)
                Get(ReservedSize - 1).~T();
#endif
            if (InitObject)
                new (&Get(ReservedSize - 1)) T(i_Ele);
            return ReservedSize - 1;
        }

        if (InitObject)
            new (&Get(m_Size++)) T(i_Ele);
        else {
            Get(m_Size) = i_Ele;
            m_Size++;
        }
        return m_Size - 1;
    }

    Bool IsFull() {
        return ReservedSize == m_Size ? TRUE : FALSE;
    }

    void SetSize(int i_NewSize) {
        int i;
        if (i_NewSize != m_Size) {
            if (i_NewSize > m_Size) {
                if (InitObject) {
                    for (i = m_Size; i < i_NewSize; i++) {
                        new (&Get(i)) T;
                    }
                }
            }
            else if (i_NewSize < m_Size) {
                if (DeleteObject) {
                    for (i = m_Size - 1; i >= i_NewSize; i--) {
                        Get(i).~T();
                    }
                }
            }
            m_Size = i_NewSize;
        }
    }

    void Remove(int i_Index) {
        DYNARRAY_Z_EXP((((U32)i_Index) < (U32)m_Size));
        if (DeleteObject)
            Get(i_Index).~T();
        memcpy(&Get(i_Index), &Get(i_Index + 1), (m_Size - i_Index - 1) * sizeof(T));
        m_Size--;
    }

    void Insert(int i_Index, const T& i_Ele) {
        DYNARRAY_Z_EXP(i_Index < m_Size && i_Index >= 0);
        if (m_Size == 0 || m_Size >= ReservedSize) {
            return;
        }
        memmove(&Get(i_Index + 1), &Get(i_Index), (m_Size - i_Index) * sizeof(T));
        if (InitObject)
            new (&Get(i_Index)) T(i_Ele);
        else
            Get(i_Index) = i_Ele;
        m_Size++;
    }

    void Insert(int i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size && i_Index >= 0);
        if (m_Size == 0 || m_Size >= ReservedSize) {
            return;
        }
        memmove(&Get(i_Index + 1), &Get(i_Index), (m_Size - i_Index) * sizeof(T));
        if (InitObject)
            new (&Get(i_Index)) T();
        m_Size++;
    }
} Aligned_Z(16);

#endif
