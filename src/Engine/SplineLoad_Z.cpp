#include "Spline_Z.h"
#include "Sys_Z.h"

void Spline_Z::Load(void** i_Data) {
    S32 l_Nb;

    Object_Z::Load(i_Data);

    LOADDYNARRAY_Z(m_Points);
    LOADDYNARRAY_Z(m_Splines);

    LOADBYTES_Z(&m_UnkVec4f_0x90, sizeof(m_UnkVec4f_0x90) + sizeof(m_Length));
}
