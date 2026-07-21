#ifndef MATRIX_CACHE_Z_H
#define MATRIX_CACHE_Z_H
#include "DynArray_Z.h"
#include "Math_Z.h"

#define MAT4X4BUFFER_GRANULARITY 0x400

struct MatDbId {
    U16 Id[2];
};

struct CacheState_Z;

class Mat4x4Buffer_Z {
public:
    Mat4x4Buffer_Z();
    void RemoveMatrix(U16 i_Id);
    U16 GetNewMatrix();
    U16 GetAMatrix();
    void GetState(CacheState_Z& o_State);

    inline Mat4x4* GetMatrix(U16 i_Id) {
        U32 l_Id = m_MatIdArray[i_Id].Id[m_CurBuffer];
        if (!l_Id) {
            l_Id = GetAMatrix();
            m_MatIdArray[i_Id].Id[m_CurBuffer] = l_Id;
        }
        return &m_MatArray[l_Id];
    }

    inline S32 GetCurBuffer() const {
        return m_CurBuffer;
    }

private:
    DynArray_Z<Mat4x4, 32, FALSE, FALSE> m_MatArray;
    DynArray_Z<MatDbId, 32, FALSE> m_MatIdArray;
    S32 m_CurBuffer;
    U16 m_TotalMatNb;
    U16 m_MaxInUseMatNb;
    U16 m_FreeMatNb;
    DynArray_Z<U16> m_FreeMatIndices;
    U16 m_FreeMatDbIdNb;
    DynArray_Z<U16> m_FreeMatDbIdIndices;
};

Bool GetMatrixUsage();

#endif
