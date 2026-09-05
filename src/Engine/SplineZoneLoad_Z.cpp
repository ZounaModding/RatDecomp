#include "SplineZone_Z.h"
#include "Sys_Z.h"

void SplineZone_Z::Load(void** i_Data) {
    S32 l_Nb;

    Object_Z::Load(i_Data);

    LOADBYTES_Z(&m_UnkVec4f_0x80, sizeof(m_UnkVec4f_0x80));

    LOADDYNARRAY_Z(m_Points);
    LOADDYNARRAY_Z(m_Splines);
    LOADDYNARRAY_Z(m_SplineZones);
    LOADDYNARRAY_Z(m_SplineIndices);
    LOADDYNARRAY_Z(m_SplineZoneLinks);

    LOADRANGE_Z(m_SplineZoneSead.m_PMin, m_SplineZoneSead.m_SizeY);

    l_Nb = m_SplineZoneSead.m_SizeX * m_SplineZoneSead.m_SizeY;
    m_SplineZoneSead.m_GridDA.SetSize(l_Nb);
    LOADARRAY_Z(m_SplineZoneSead.m_GridDA.GetArrayPtr(), l_Nb);

    LOADDYNARRAY_Z(m_SplineZoneSead.m_ZoneIdDA);
}
