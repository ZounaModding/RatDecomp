#ifndef _DYNPTRARRAY_Z_H_
#define _DYNPTRARRAY_Z_H_
#include "Types_Z.h"
#include "Memory_Z.h"
#include "Assert_Z.h"

#define DYNARRAY_Z_EXP(exp) \
    { EXCEPTIONC_Z(exp, "Array_Z %s %s (size %d)", #exp, TYPEINFO_Z(this), GetSize()); }

#define DYA_SIZEBITS 18
#define DYA_RSVSIZEBITS (32 - DYA_SIZEBITS)
#define DYA_SIZEMAX ((1 << DYA_SIZEBITS))
#define DYA_RSVSIZEMAX ((1 << DYA_RSVSIZEBITS))

template <class T, int Granularity = 8, int Align = _ALLOCDEFAULTALIGN>
class DynPtrArray_Z {
public:
    DynPtrArray_Z() {
        m_Size = 0;
        m_ReservedSize = 0;
        m_ArrayPtr = NULL;
    }

    ~DynPtrArray_Z() {
        if (m_ArrayPtr) {
            Free_Z(m_ArrayPtr);
            m_ArrayPtr = NULL;
        }
    }

    void SetSize(int i_NewSize, const Bool i_ResizeOnlyIfGreater = FALSE) {
        DYNARRAY_Z_EXP(i_NewSize <= DYA_SIZEMAX);
        DYNARRAY_Z_EXP(i_NewSize >= 0);
        int i;
        if (i_NewSize > m_Size) {
            if ((i_NewSize - m_Size > m_ReservedSize) || !i_ResizeOnlyIfGreater) {
                Realloc(i_NewSize);
                m_ReservedSize = 0;
            }
            else if ((i_NewSize - m_Size <= m_ReservedSize)) {
                m_ReservedSize -= i_NewSize - m_Size;
            }
            m_Size = i_NewSize;
        }
        else if (i_NewSize < m_Size) {
            if (i_ResizeOnlyIfGreater) {
                m_ReservedSize += m_Size - i_NewSize;
                m_Size = i_NewSize;
            }
            else {
                Realloc(i_NewSize);
                m_Size = i_NewSize;
                m_ReservedSize = 0;
            }
        }
    }

    void SetReserve(int i_NewReservedSize) {
        if (i_NewReservedSize < m_Size)
            SetSize(i_NewReservedSize);
        else {
            int l_NewReserved = i_NewReservedSize - m_Size;
            DYNARRAY_Z_EXP(l_NewReserved <= DYA_RSVSIZEMAX);
            if (l_NewReserved > DYA_RSVSIZEMAX)
                l_NewReserved = DYA_RSVSIZEMAX;
            i_NewReservedSize = m_Size + l_NewReserved;
            Realloc(i_NewReservedSize);
            m_ReservedSize = l_NewReserved;
        }
    }

    T* GetArrayPtr() const {
        return m_ArrayPtr;
    }

    int GetSize() const {
        return m_Size;
    }

    int Add() {
        if (!m_ReservedSize) {
            DYNARRAY_Z_EXP(Granularity <= DYA_RSVSIZEMAX);
            m_ReservedSize = Granularity;
            Realloc(m_ReservedSize + m_Size);
        }
        DYNARRAY_Z_EXP(m_Size < DYA_SIZEMAX);
        m_Size++;
        m_ReservedSize--;
        return m_Size - 1;
    }

    int Add(const T& i_Ele) {
        DYNARRAY_Z_EXP(m_ArrayPtr == NULL || !(&i_Ele >= m_ArrayPtr && &i_Ele < m_ArrayPtr + (U32)GetSize()));
        if (!m_ReservedSize) {
            DYNARRAY_Z_EXP(Granularity <= DYA_RSVSIZEMAX);
            m_ReservedSize = Granularity;
            Realloc(m_ReservedSize + m_Size);
        }
        m_ArrayPtr[m_Size] = i_Ele;
        DYNARRAY_Z_EXP(m_Size < DYA_SIZEMAX);
        m_Size++;
        m_ReservedSize--;
        return m_Size - 1;
    }

    void Empty() {
        SetSize(0, TRUE);
    }

    void Flush() {
        SetSize(0);
    }

    void Minimize() {
        Realloc(m_Size);
        m_ReservedSize = 0;
    }

    T& Get(int i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_ArrayPtr[i_Index];
    }

    const T& Get(int i_Index) const {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_ArrayPtr[i_Index];
    }

    const T& operator[](int i_Index) const {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_ArrayPtr[i_Index];
    }

    T& operator[](int i_Index) {
        DYNARRAY_Z_EXP(i_Index < m_Size);
        return m_ArrayPtr[i_Index];
    }

    DynPtrArray_Z<T, Granularity, Align>& operator=(const DynPtrArray_Z<T, Granularity, Align>& i_Src) {
        Empty();
        SetReserve(i_Src.GetSize());
        for (S32 i = 0; i < i_Src.GetSize(); ++i) {
            Add(i_Src[i]);
        }
        return *this;
    }

private:
    void Realloc(int i_NewNbElement) {
        if (i_NewNbElement) {
            if (m_ArrayPtr)
                m_ArrayPtr = (T*)ReallocL_Z(m_ArrayPtr, i_NewNbElement * sizeof(T), 311);
            else
                m_ArrayPtr = (T*)AllocAlignL_Z(i_NewNbElement * sizeof(T), 317, Align);
            DYNARRAY_Z_EXP(m_ArrayPtr != NULL);
        }
        else {
            if (m_ArrayPtr) {
                Free_Z(m_ArrayPtr);
                m_ArrayPtr = NULL;
            }
        }
    }

    U32 m_ReservedSize : 14,
        m_Size : 18;
    T* m_ArrayPtr;
};

#endif
