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

inline Vec4f& NormalizeCollisionNormal(Vec4f& io_Normal) {
    Float l_InvNorm = 1.f / Sqrt(io_Normal.GetNorm2());
    io_Normal.x *= l_InvNorm;
    io_Normal.y *= l_InvNorm;
    io_Normal.z *= l_InvNorm;
    return io_Normal;
}

inline Float CollisionEdgeSide(const Vec4f& i_V0, const Vec4f& i_V1, const Vec3f& i_Point, const Vec3f& i_Normal) {
    Float l_X0 = i_V0.x - i_Point.x;
    Float l_Y0 = i_V0.y - i_Point.y;
    Float l_Z0 = i_V0.z - i_Point.z;
    Float l_X1 = i_V1.x - i_Point.x;
    Float l_Y1 = i_V1.y - i_Point.y;
    Float l_Z1 = i_V1.z - i_Point.z;
    return (l_X0 * l_Y1 - l_Y0 * l_X1) * i_Normal.z + (l_Y0 * l_Z1 - l_Z0 * l_Y1) * i_Normal.x + (l_Z0 * l_X1 - l_X0 * l_Z1) * i_Normal.y;
}

inline Float CollisionCrossDot(const Vec3f& i_Left, const Vec3f& i_Right, const Vec3f& i_Normal) {
    return (i_Left.y * i_Right.z - i_Left.z * i_Right.y) * i_Normal.x + (i_Left.z * i_Right.x - i_Left.x * i_Right.z) * i_Normal.y + (i_Left.x * i_Right.y - i_Left.y * i_Right.x) * i_Normal.z;
}

inline Double CollisionCubeRoot(Double i_Value) {
    return i_Value > 0.0 ? Float(pow(Float(i_Value), 1.f / 3.f)) : (i_Value < 0.0 ? -Float(pow(Float(-i_Value), 1.f / 3.f)) : 0.0);
}

inline int SolveQuadric(Double* i_Coefficients, Double* o_Roots) {
    Double l_P = i_Coefficients[1] / (2 * i_Coefficients[2]);
    Double l_Q = i_Coefficients[0] / i_Coefficients[2];
    Double l_Discriminant = l_P * l_P - l_Q;
    if (l_Discriminant > -1.e-9f && l_Discriminant < 1.e-9f) {
        o_Roots[0] = -l_P;
        return 1;
    }
    else if (l_Discriminant < 0) {
        return 0;
    }
    else {
        Double l_Sqrt = sqrt(l_Discriminant);
        o_Roots[0] = l_Sqrt - l_P;
        o_Roots[1] = -l_Sqrt - l_P;
        return 2;
    }
}

Bool SegmentVsSphere(const Segment_Z& i_Segment, const Sphere_Z& i_Sphere, CollisionReport_Z& o_Report);
Bool SegmentVsBox(const Segment_Z& i_Segment, const Box_Z& i_Box, CollisionReport_Z& o_Report);
Bool SegmentVsCylindre(const Segment_Z& i_Segment, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report);
Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report);
Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, CollisionReport_Z& o_Report);
Bool CylinderSphereVsSegment(const Sphere_Z& i_Sphere, const Segment_Z& i_Segment);
Bool SphereVsCapsule(const Sphere_Z& i_Sphere, const Capsule_Z& i_Capsule);
Bool CylinderSphereVsCapsule(const Sphere_Z& i_Sphere, const Capsule_Z& i_Capsule);
Bool SphereVsEdge(const Sphere_Z& i_Sphere, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report);
Bool MovingSphereVsEdge(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report);
Bool MovingSphereVsDirEdge(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_Direction, CollisionReport_Z& o_Report);
Bool MovingSphereVsEdgeInfinyUp(const Capsule_Z& i_Capsule, const Segment_Z& i_Edge, CollisionReport_Z& o_Report);
Bool MovingSphereVsBox(const Capsule_Z& i_Capsule, const Box_Z& i_Box, CollisionReport_Z& o_Report);
Bool MovingSphereVsQuad(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, const Vec4f& i_V3, CollisionReport_Z& o_Report);
Bool MovingSphereVsTri(const Capsule_Z& i_Capsule, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report);
Bool MovingSphereVsTri(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, CollisionReport_Z& o_Report);
Bool MovingSphereVsVertex(const Capsule_Z& i_Capsule, const Vec4f& i_Vertex, CollisionReport_Z& o_Report);
Bool MovingSphereVsFaceInfinyUp(const Capsule_Z& i_Capsule, Vec4f& io_V0, Vec4f& io_V1, const Vec4f& i_Up, CollisionReport_Z& o_Report);
Bool SegmentVsInfinySeg(const Segment_Z& i_Segment, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report);
Bool PointVsBox(const Vec3f& i_Point, const Box_Z& i_Box);
Bool LineVsPatch(const Segment_Z& i_Segment, Vec4f* i_Cache, CollisionReport_Z& o_Report, S32 i_Lod);
Bool LineVsSplineCollide(const Segment_Z& i_Segment, const Vec3f& i_P0, const Vec3f& i_P1, const Vec3f& i_P2, const Vec3f& i_P3, CollisionReport_Z& o_Report, Float i_Step);

#endif
