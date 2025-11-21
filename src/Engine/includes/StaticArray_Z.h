#ifndef _STATICARRAY_Z_H_
#define _STATICARRAY_Z_H_

#include <BitArray_Z.h>

template <class T, int ReservedSize, Bool DeleteObject = TRUE, Bool InitObject = TRUE>
class StaticArray_Z {
private:
    Char ArrayChar[ReservedSize * sizeof(T)];
    int Size;

    T& Get(int Id) {
        return *(T*)(ArrayChar + Id * sizeof(T));
    }

    const T& Get(int Id) const {
        return *(T*)(ArrayChar + Id * sizeof(T));
    }

public:
    StaticArray_Z() {
        Size = 0;
    }

    StaticArray_Z(T nValue[], S32 nSize) {
        Size = 0;
        for (int i = 0; i < nSize; i++)
            Add(nValue[i]);
    }

    StaticArray_Z(const StaticArray_Z<T, ReservedSize>& Src) {
        Size = 0;
        for (int i = 0; i < Src.GetSize(); i++)
            Add(Src[i]);
    }

    ~StaticArray_Z() {
        if (DeleteObject)
            for (int i = 0; i < Size; i++)
                Get(i).~T();
    }
};

#endif
