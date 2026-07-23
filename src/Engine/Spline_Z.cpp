#include "Spline_Z.h"

Spline_Z::Spline_Z() {
    SetGeometryType(SPLINE_Z);
    EnableFlag(FL_OBJECT_NO_SEADCOLLIDE);
    m_Length = 0.0f;
}

void Spline_Z::Clean() {
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
