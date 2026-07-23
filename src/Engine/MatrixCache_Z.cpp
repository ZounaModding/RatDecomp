#include "MatrixCache_Z.h"
#include "Assert_Z.h"
#include "Program_Z.h"

Mat4x4Buffer_Z::Mat4x4Buffer_Z() {
    U16 i;
    m_CurBuffer = 0;
    m_MaxInUseMatNb = 0;
    m_FreeMatNb = User_NbMatrix;
    m_TotalMatNb = m_FreeMatNb;
    m_MatArray.SetSize(m_FreeMatNb);
    m_FreeMatIndices.SetSize(m_FreeMatNb);
    for (i = 0; i < m_FreeMatNb; i++) {
        m_MatArray[i].SetIdentity();
    }
    for (i = 0; i < m_FreeMatNb; i++) {
        m_FreeMatIndices[i] = i;
    }
    m_FreeMatDbIdNb = m_TotalMatNb / 3;
    m_MatIdArray.SetSize(m_FreeMatDbIdNb);
    for (i = 0; i < m_FreeMatDbIdNb; i++) {
        m_MatIdArray[i].Id[0] = 0;
        m_MatIdArray[i].Id[1] = 0;
    }
    m_FreeMatDbIdIndices.SetSize(m_FreeMatDbIdNb);
    U16 l_FreeIdx = m_FreeMatDbIdNb - 1;
    for (i = 0; i < m_FreeMatDbIdNb; l_FreeIdx--, i++) {
        m_FreeMatDbIdIndices[i] = l_FreeIdx;
    }
    GetNewMatrix();
    REGISTERCOMMAND("GetMatrixUsage", GetMatrixUsage);
}

U16 Mat4x4Buffer_Z::GetAMatrix() {
    return 0;
}

U16 Mat4x4Buffer_Z::GetNewMatrix() {
    if (!m_FreeMatNb) {
        GetMatrixUsage();
    }

    ASSERTLE_Z(m_FreeMatNb, "Increase User_NbMatrix in your Config_Z.cpp", 52, "FreeMatNb");

    U16 l_MatIdx = m_FreeMatIndices[--m_FreeMatNb];

    m_MatArray[l_MatIdx].SetIdentity();

    S32 l_InUseMatNb = m_TotalMatNb - m_FreeMatNb;
    if (l_InUseMatNb > m_MaxInUseMatNb) {
        m_MaxInUseMatNb = l_InUseMatNb;
    }

    if (m_FreeMatDbIdNb == 0) {
        m_FreeMatDbIdNb = MAT4X4BUFFER_GRANULARITY;

        U16 l_OldMatDbIdNb = m_MatIdArray.GetSize();
        U16 l_NewMatDbIdNb = l_OldMatDbIdNb + m_FreeMatDbIdNb;

        m_MatIdArray.SetSize(l_NewMatDbIdNb);

        for (U16 i = l_OldMatDbIdNb; i < (U32)l_NewMatDbIdNb; i++) {
            m_MatIdArray[(U32)i].Id[0] = 0;
            m_MatIdArray[(U32)i].Id[1] = 0;
        }

        if (m_FreeMatDbIdIndices.GetSize() < m_FreeMatDbIdNb) {
            m_FreeMatDbIdIndices.SetSize(m_FreeMatDbIdNb);
        }

        for (U16 i = 0; i < m_FreeMatDbIdNb; i++) {
            m_FreeMatDbIdIndices[i] = l_OldMatDbIdNb++;
        }
    }

    U16 l_MatDbIdx = m_FreeMatDbIdIndices[--m_FreeMatDbIdNb];
    m_MatIdArray[l_MatDbIdx].Id[m_CurBuffer] = l_MatIdx;

    return l_MatDbIdx;
}

void Mat4x4Buffer_Z::RemoveMatrix(U16 i_Id) {
}

Bool GetMatrixUsage() {
    return FALSE;
}
