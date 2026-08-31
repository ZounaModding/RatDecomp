#include "SplineZone_Z.h"

SplineZone_Z::SplineZone_Z() {
    SetGeometryType(SPLINEZONE_Z);
}

void SplineZone_Z::Clean() {
    Object_Z::Clean();
    m_Points.Empty();
    m_Splines.Empty();
    m_SplineZones.Empty();
    m_SplineZoneSead.Empty();
    m_SplineIndices.Empty();
    m_SplineZoneLinks.Empty();
}

void SplineZone_Z::Reset() {
    for (S32 i = 0; i < m_SplineZones.GetSize(); i++) {
        if (m_SplineZones[i].m_Flag & FL_ZONE_SPLINE_NO_MARK) {
            m_SplineZones[i].m_Flag |= FL_ZONE_SPLINE_OCCLUDER;
        }
    }
}

// TODO: Finish matching
Bool SplineZone_Z::GetZone(const Vec3f& i_CamPos, S32& o_ZoneId) {
    o_ZoneId = -1;
    Vec2f l_CamPos2D(i_CamPos.x, i_CamPos.z);

    Bool l_Found = FALSE;
    Float l_Delta = 10E+10f;
    U16* l_ZoneIds;
    S16 l_ZoneNb = m_SplineZoneSead.IsRef(l_CamPos2D, &l_ZoneIds);

    for (S16 i = 0; i < l_ZoneNb; i++) {
        U16 l_ZoneId = *l_ZoneIds++;
        SplineZone& l_Zone = m_SplineZones[l_ZoneId];
        U16 l_DirectionFlag = FL_SPLINE_NORMAL_DIR;
        S16 l_SplineNb = l_Zone.m_SplineIdsNb;
        U16* l_SplineIds = &m_SplineIndices[l_Zone.m_SplineIdsIdx];
        U16 l_SplineDirectionBits = l_Zone.m_SplineDirectionBits;

        U16 l_PointId;
        if (l_SplineDirectionBits & l_DirectionFlag) {
            l_PointId = m_Splines[*l_SplineIds].m_P0;
        }
        else {
            l_PointId = m_Splines[*l_SplineIds].m_P1;
        }

        Vec3f l_Point0 = m_Points[l_PointId];
        S32 l_IntersectionNb = 0;

        for (S16 j = 0; j < l_SplineNb; j++, l_DirectionFlag <<= 1) {
            Vec3f l_Point1;
            if (l_SplineDirectionBits & l_DirectionFlag) {
                S32 l_SplineId = *l_SplineIds++;
                l_Point1 = m_Points[m_Splines[l_SplineId].m_P1];
            }
            else {
                S32 l_SplineId = *l_SplineIds++;
                l_Point1 = m_Points[m_Splines[l_SplineId].m_P0];
            }

            Vec3f l_MinPoint;
            Vec3f l_MaxPoint;
            if (l_Point0.z > l_Point1.z) {
                l_MinPoint = l_Point1;
                l_MaxPoint = l_Point0;
            }
            else {
                l_MinPoint = l_Point0;
                l_MaxPoint = l_Point1;
            }

            if (l_MinPoint.z < l_CamPos2D.y && l_MaxPoint.z >= l_CamPos2D.y) {
                if (l_MinPoint.x < l_CamPos2D.x && l_MaxPoint.x < l_CamPos2D.x) {
                    l_IntersectionNb++;
                }
                if (((l_MinPoint.x <= l_CamPos2D.x && l_MaxPoint.x >= l_CamPos2D.x) || (l_MaxPoint.x <= l_CamPos2D.x && l_MinPoint.x >= l_CamPos2D.x)) && l_MaxPoint.z - l_MinPoint.z != 0.0f) {
                    Float l_Intersection = l_MinPoint.x + ((l_CamPos2D.y - l_MinPoint.z) * (l_MaxPoint.x - l_MinPoint.x)) / (l_MaxPoint.z - l_MinPoint.z);
                    if (l_Intersection < l_CamPos2D.x) {
                        l_IntersectionNb++;
                    }
                }
            }

            l_Point0 = l_Point1;
        }

        if (l_IntersectionNb & 1) {
            Float l_CurrentDelta = l_Zone.m_Y - i_CamPos.y;
            if (!l_Found || (l_CurrentDelta > 0.0f && l_CurrentDelta < l_Delta)) {
                if (l_CurrentDelta > 0.0f && l_CurrentDelta < l_Delta) {
                    l_Delta = l_CurrentDelta;
                }
                o_ZoneId = l_ZoneId;
                l_Found = TRUE;
            }
        }
    }

    return l_Found;
}

void SplineZoneSead_Z::Empty() {
    m_GridDA.Empty();
    m_ZoneIdDA.Empty();
    m_SizeY = 0;
    m_SizeX = 0;
    m_MaxZoneIndex = 0;
}

// TODO: Finish matching
S32 SplineZoneSead_Z::IsRef(const Vec2f& i_Pos, U16** o_ZoneIds) {
    Float l_YFactor = m_InvDiag.y * (Float)m_SizeY;
    Float l_XFactor = m_InvDiag.x * (Float)m_SizeX;
    S16 l_X = (S16)(l_XFactor * (i_Pos.x - m_PMin.x));
    if (l_X < 0 || l_X >= m_SizeX) {
        return 0;
    }

    S16 l_Y = (S16)(l_YFactor * (i_Pos.y - m_PMin.y));
    if (l_Y < 0 || l_Y >= m_SizeY) {
        return 0;
    }

    SplineZoneVoxel_Z& l_Voxel = m_GridDA[l_X + l_Y * m_SizeX];
    S16 l_ZoneNb = l_Voxel.m_ZoneNb;
    if (!l_ZoneNb) {
        return 0;
    }

    *o_ZoneIds = &m_ZoneIdDA[l_Voxel.m_ZoneIdx];
    return l_ZoneNb;
}
