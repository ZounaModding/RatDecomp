#ifndef _COLLISIONTOOL_Z_H_
#define _COLLISIONTOOL_Z_H_
#include "Math_Z.h"
#include "SystemObject_Z.h"

struct CollisionReport_Z {
    Vec4f m_Intersection;
    Vec4f m_Normal;
    Vec2f m_UnkVec2f_0x20;
    Float m_CollisionDistance;
    U32 m_EleIdx;
    U64 m_Flag;
};

Bool SegmentVsSphere(const Segment_Z& i_Segment, const Sphere_Z& i_Sphere, CollisionReport_Z& o_Report);
Bool SegmentVsBox(const Segment_Z& i_Segment, const Box_Z& i_Box, CollisionReport_Z& o_Report);
Bool SegmentVsCylindre(const Segment_Z& i_Segment, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report);
Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report);
Bool CylinderSphereVsSegment(const Sphere_Z& i_Sphere, const Segment_Z& i_Segment);
Bool LineVsPatch(const Segment_Z& i_Segment, Vec4f* i_Cache, CollisionReport_Z& o_Report, S32 i_Lod);
Bool LineVsSplineCollide(const Segment_Z& i_Segment, const Vec3f& i_P0, const Vec3f& i_P1, const Vec3f& i_P2, const Vec3f& i_P3, CollisionReport_Z& o_Report, Float i_Step);

#endif
