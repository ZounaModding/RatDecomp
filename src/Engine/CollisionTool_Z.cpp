#include "CollisionTool_Z.h"

Bool SphereVsEdge(const Sphere_Z& i_Sphere, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
    static Vec4f DirUp(0.0f, 1.0f, 0.0f, 1.0f);
    return FALSE;
}

Vec4f _vBestNormal;
static Float fBestDepth;

Bool SegmentVsSphere(const Segment_Z& i_Seg, const Sphere_Z& i_Sph, CollisionReport_Z& o_Report) {
    return FALSE;
}

Bool CylinderSphereVsSegment(const Sphere_Z& i_Sphere, const Segment_Z& i_Segment) {
    return FALSE;
}

Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report) {
    return FALSE;
}
