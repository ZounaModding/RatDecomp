#include "Points_Z.h"
#include "Sys_Z.h"

void Points_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);

    S32 l_Nb;
    LOADDYNARRAY_Z(m_Vertices);
    LOADDYNARRAY_Z(m_TangentBinormalVertices);

    m_Morpher.Load(i_Data);
}

void Points_Z::Clean() {
    Object_Z::Clean();

    m_Vertices.SetSize(0);
    m_TangentBinormalVertices.SetSize(0);
}
