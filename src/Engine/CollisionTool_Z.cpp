#include "CollisionTool_Z.h"

Bool SphereVsEdge(const Sphere_Z& i_Sphere, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
// TODO: KOS is broken with local static initializer currently. Remove this #ifdef when that's fixed
#ifdef GAMECUBE_Z
    static Vec4f DirUp(0.0f, 1.0f, 0.0f, 1.0f);
#endif
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

Bool SegmentVsBox(const Segment_Z& i_Segment, const Box_Z& i_Box, CollisionReport_Z& o_Report) {
    return FALSE;
}

Bool SegmentVsCylindre(const Segment_Z& i_Segment, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report) {
    return FALSE;
}
