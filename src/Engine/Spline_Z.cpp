#include "Spline_Z.h"

Spline_Z::Spline_Z() {
    SetGeometryType(SPLINE_Z);
    EnableFlag(FL_OBJECT_NO_SEADCOLLIDE);
    m_Length = 0.0f;
}

void Spline_Z::Clean() {
}

// TODO: Finish matching
Float Spline_Z::GetValueOnTrack(Float i_Ratio, Vec3f& o_Pos) {
    Float l_LengthRatio = m_Length * i_Ratio;
    S32 l_NbSplines = m_Splines.GetSize();
    Spline* l_Spline = m_Splines.GetArrayPtr();
    Float l_Ratio = i_Ratio;

    if (l_NbSplines > 1) {
        Float l_CurLength = 0.0f;

        for (S32 i = 0; i < l_NbSplines; i++) {
            if (l_CurLength > l_LengthRatio) {
                break;
            }

            l_CurLength += l_Spline->m_Length;
            l_Spline++;
        }

        l_Spline--;
        l_LengthRatio -= l_CurLength - l_Spline->m_Length;
        l_CurLength = 0.0f;
        l_Ratio = 0.0f;

        Segment* l_Segment = l_Spline->m_Segments;
        for (S32 i = 0; i < SPLINE_NBSEGMENT; i++, l_Segment++) {
            if (l_CurLength + l_Segment->m_Length > l_LengthRatio) {
                l_Ratio += 0.125f * ((l_LengthRatio - l_CurLength) / l_Segment->m_Length);
                break;
            }
            l_Ratio += 0.125f;
            l_CurLength += l_Segment->m_Length;
        }
    }

    Vec3f l_Pt0 = m_Points[l_Spline->m_PointIds[0]];
    Vec3f l_Pt1 = m_Points[l_Spline->m_TgtIds[0]];
    Vec3f l_Pt2 = m_Points[l_Spline->m_TgtIds[1]];
    Vec3f l_Pt3 = m_Points[l_Spline->m_PointIds[1]];
    Vec3f l_Pt0x3 = l_Pt0 * 3.0f;
    Vec3f l_Pt1x3 = l_Pt1 * 3.0f;
    Vec3f l_Pt2x3 = l_Pt2 * 3.0f;

    Vec3f l_A = -l_Pt0 + l_Pt1x3 - l_Pt2x3 + l_Pt3;
    Vec3f l_B = l_Pt0x3 - (l_Pt1x3 + l_Pt1x3) + l_Pt2x3;
    Vec3f l_C = -l_Pt0x3 + l_Pt1x3;

    o_Pos = l_Pt0 + l_Ratio * (l_C + l_Ratio * (l_B + l_Ratio * l_A));
    return l_LengthRatio;
}

Float Spline_Z::GetValueOnTrack(const Node_ZHdl& i_NodeHdl, Float i_Ratio, Vec3f& i_Pos) {
    return 0.0f;
}

S32 Spline_Z::GetSplineId(Float i_Ratio) {
    return 0;
}

Float Spline_Z::CalcSplineLen(Vec3f& i_PosA, Vec3f& i_PosB, Vec3f& i_PosC, Vec3f& i_PosD, Float i_Ratio) {
    return 0.0f;
}

Float Spline_Z::GetValueOutTrack(Float i_Ratio, Vec3f& i_Pos) {
    return 0.0f;
}
