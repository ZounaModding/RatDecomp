#include "CollisionTool_Z.h"

Bool SegmentVsSphere(const Segment_Z& i_Seg, const Sphere_Z& i_Sph, CollisionReport_Z& o_Report) {
    Float l_Radius2 = i_Sph.Radius * i_Sph.Radius;
    Vec3f l_Offset = i_Sph.Center - i_Seg.Origin;
    Float l_Distance = l_Offset * i_Seg.Direction;
    Vec3f l_Delta = i_Seg.Origin + i_Seg.Direction * l_Distance;
    l_Delta -= i_Sph.Center;
    Float l_Distance2 = l_Delta.GetNorm2();
    if (l_Distance2 > l_Radius2) {
        return FALSE;
    }
    l_Distance -= Sqrt(l_Radius2 - l_Distance2);
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Seg.Length) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = i_Seg.Origin + i_Seg.Direction * l_Distance;
    Vec4f l_Normal = o_Report.m_Intersection - i_Sph.Center;
    o_Report.m_Normal = NormalizeCollisionNormal(l_Normal);
    return TRUE;
}

Bool SphereVsSphere(const Sphere_Z& i_Sphere, const Sphere_Z& i_Other, CollisionReport_Z& o_Report) {
    Vec3f l_Normal = i_Sphere.Center - i_Other.Center;
    Float l_Radius = i_Sphere.Radius + i_Other.Radius;
    Float l_Distance2 = l_Normal.GetNorm2();
    if (l_Distance2 < l_Radius * l_Radius) {
        Float l_Length = Sqrt(l_Distance2);
        Float l_Distance = l_Length - i_Other.Radius;
        if (l_Distance > o_Report.m_CollisionDistance) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = l_Distance;
        o_Report.m_Normal = l_Normal / l_Length;
        Vec3f l_UnitNormal = o_Report.m_Normal;
        o_Report.m_Intersection = i_Other.Center + l_UnitNormal * i_Other.Radius;
        return TRUE;
    }
    return FALSE;
}

Bool MovingSphereVsSphere(const Capsule_Z& i_Capsule, const Sphere_Z& i_Sphere, CollisionReport_Z& o_Report) {
    Vec4f l_Normal;
    Vec4f l_OverlapNormal;
    Vec3f l_Offset = i_Sphere.Center - i_Capsule.Origin;
    Vec3f l_Origin = i_Capsule.Origin;
    Float l_Distance = l_Offset * i_Capsule.Direction;
    if (l_Distance < 0.f) {
        return FALSE;
    }
    Vec3f l_Delta = i_Capsule.Direction * l_Distance + l_Origin - i_Sphere.Center;
    Float l_Radius = i_Capsule.Radius + i_Sphere.Radius;
    Float l_Radius2 = l_Radius * l_Radius;
    Float l_Distance2 = l_Delta.GetNorm2();
    if (l_Distance2 > l_Radius2) {
        return FALSE;
    }
    Float l_OriginDistance2 = l_Offset.GetNorm2();
    if (l_OriginDistance2 < l_Radius2) {
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = l_Origin;
        if (l_OriginDistance2 != 0.f) {
            l_OverlapNormal = -l_Offset;
            o_Report.m_Normal = NormalizeCollisionNormal(l_OverlapNormal);
        }
        else {
            o_Report.m_Normal.Set(0.f, 1.f, 0.f, 1.f);
        }
        return TRUE;
    }
    l_Distance -= Sqrt(l_Radius2 - l_Distance2);
    if (l_Distance < i_Capsule.Length) {
        if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = l_Distance;
        o_Report.m_Intersection = i_Capsule.Origin + i_Capsule.Direction * l_Distance;
        l_Normal = o_Report.m_Intersection - i_Sphere.Center;
        o_Report.m_Normal = NormalizeCollisionNormal(l_Normal);
        return TRUE;
    }
    return FALSE;
}

Bool SegmentVsCylindre(const Segment_Z& i_Segment, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report) {
    Float l_Length = i_Segment.Length;
    Vec3f l_Origin = i_Segment.Origin;
    Vec3f l_Direction = i_Segment.Direction;
    Vec3f l_End = i_Segment.Origin + i_Segment.Direction * i_Segment.Length;
    Vec3f l_Center = i_Cylindre.Seg.Origin;
    Vec3f l_Axis = i_Cylindre.Seg.Direction;
    Float l_Height = i_Cylindre.Seg.Length;
    Vec3f l_Top = l_Center + l_Axis * l_Height;
    Float l_StartHeight = (l_Origin - l_Center) * l_Axis;
    Float l_EndHeight = (l_End - l_Center) * l_Axis;
    Float l_BaseD = -(l_Axis * l_Center);
    Float l_TopD = -(l_Axis * l_Top);
    Vec3f l_ProjectedOrigin = l_Origin - l_Axis * l_StartHeight;
    Vec3f l_ProjectedEnd = l_End - l_Axis * l_EndHeight;
    Vec3f l_ProjectedDirection = l_ProjectedEnd - l_ProjectedOrigin;
    Float l_ProjectedLength = l_ProjectedDirection.GetNorm();
    if (l_ProjectedLength > Float_Eps) {
        l_ProjectedDirection /= l_ProjectedLength;
    }
    Float l_Radius2 = i_Cylindre.Radius * i_Cylindre.Radius;
    Float l_Distance = (l_Center - l_ProjectedOrigin) * l_ProjectedDirection;
    Vec3f l_Delta = l_ProjectedDirection * l_Distance + l_ProjectedOrigin - l_Center;
    Float l_Distance2 = l_Delta.GetNorm2();
    if (l_Distance2 > l_Radius2) {
        return FALSE;
    }
    if (l_Direction * l_Axis > 0.f) {
        Float l_PlaneDistance = l_BaseD + l_Axis * l_Origin;
        if (l_PlaneDistance < 0.f) {
            Float l_HitDistance = -(l_PlaneDistance / (l_Axis * l_Direction));
            if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > l_Length) {
                return FALSE;
            }
            Vec3f l_Hit = l_Direction * l_HitDistance + l_Origin;
            if ((l_Hit - l_Center).GetNorm2() < l_Radius2) {
                o_Report.m_CollisionDistance = l_HitDistance;
                o_Report.m_Intersection = l_Hit;
                o_Report.m_Normal = -i_Cylindre.Seg.Direction;
                return TRUE;
            }
        }
    }
    else {
        Float l_PlaneDistance = l_TopD + l_Axis * l_Origin;
        if (l_PlaneDistance > 0.f) {
            Float l_HitDistance = -(l_PlaneDistance / (l_Axis * l_Direction));
            if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > l_Length) {
                return FALSE;
            }
            Vec3f l_Hit = l_Direction * l_HitDistance + l_Origin;
            if ((l_Hit - l_Top).GetNorm2() < l_Radius2) {
                o_Report.m_CollisionDistance = l_HitDistance;
                o_Report.m_Intersection = l_Hit;
                o_Report.m_Normal = i_Cylindre.Seg.Direction;
                return TRUE;
            }
        }
    }
    l_Distance -= Sqrt(l_Radius2 - l_Distance2);
    Float l_HitDistance = (l_Distance * l_Length) / l_ProjectedLength;
    if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > l_Length) {
        return FALSE;
    }
    Vec3f l_Hit = l_Origin + l_Direction * l_HitDistance;
    Float l_HitHeight = (l_Hit - l_Center) * l_Axis;
    if (l_HitHeight < 0.f || l_HitHeight > l_Height) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_HitDistance;
    o_Report.m_Intersection = l_Hit;
    Vec3f l_Normal = l_ProjectedOrigin + l_ProjectedDirection * l_Distance - i_Cylindre.Seg.Origin;
    o_Report.m_Normal = l_Normal.Normalize();
    return TRUE;
}

Bool SphereVsCylindre(const Sphere_Z& i_Sphere, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report) {
    Float l_Height = i_Cylindre.Seg.Direction * (i_Sphere.Center - i_Cylindre.Seg.Origin);
    if (l_Height < 0.f) {
        Vec3f l_Point = i_Sphere.Center - (i_Cylindre.Seg.Direction * l_Height);
        Vec3f l_Radial = l_Point - i_Cylindre.Seg.Origin;
        Float l_RadialLength = l_Radial.GetNorm();
        if (l_RadialLength < Float_Eps)
            l_Radial = Vec3f(0.f, 0.f, 0.f);
        else {
            l_Radial /= l_RadialLength;
            if (l_RadialLength > i_Cylindre.Radius) {
                l_Radial *= i_Cylindre.Radius;
                l_Point = i_Cylindre.Seg.Origin + l_Radial;
            }
        }
        Vec3f l_Normal = i_Sphere.Center - l_Point;
        Float l_Distance = l_Normal.GetNorm();
        if (l_Distance < i_Sphere.Radius) {
            if (l_Distance > o_Report.m_CollisionDistance) return FALSE;
            o_Report.m_CollisionDistance = l_Distance;
            l_Normal /= l_Distance;
            o_Report.m_Intersection = l_Point;
            o_Report.m_Normal = l_Normal;
            return TRUE;
        }
    }
    else if (l_Height > i_Cylindre.Seg.Length) {
        Vec3f l_Point = i_Sphere.Center + (i_Cylindre.Seg.Direction * (i_Cylindre.Seg.Length - l_Height));
        Vec3f l_Top = i_Cylindre.Seg.Origin + i_Cylindre.Seg.Length * i_Cylindre.Seg.Direction;
        Vec3f l_Radial = l_Point - l_Top;
        Float l_RadialLength = l_Radial.GetNorm();
        if (l_RadialLength < Float_Eps)
            l_Radial = Vec3f(0.f, 0.f, 0.f);
        else {
            l_Radial /= l_RadialLength;
            if (l_RadialLength > i_Cylindre.Radius) {
                l_Radial *= i_Cylindre.Radius;
                l_Point = l_Top + l_Radial;
            }
        }
        Vec3f l_Normal = i_Sphere.Center - l_Point;
        Float l_Distance = l_Normal.GetNorm();
        if (l_Distance < i_Sphere.Radius) {
            if (l_Distance > o_Report.m_CollisionDistance) return FALSE;
            o_Report.m_CollisionDistance = l_Distance;
            l_Normal /= l_Distance;
            o_Report.m_Intersection = l_Point;
            o_Report.m_Normal = l_Normal;
            return TRUE;
        }
    }
    else {
        Vec3f l_Point = i_Cylindre.Seg.Origin + i_Cylindre.Seg.Direction * l_Height;
        Vec3f l_Radial = i_Sphere.Center - l_Point;
        Float l_RadialLength = l_Radial.GetNorm();
        if (l_RadialLength < i_Sphere.Radius + i_Cylindre.Radius) {
            Float l_Radius = i_Cylindre.Radius;
            if (l_RadialLength - l_Radius > o_Report.m_CollisionDistance) return FALSE;
            o_Report.m_CollisionDistance = l_RadialLength - l_Radius;
            l_Radial /= l_RadialLength;
            o_Report.m_Intersection = l_Point + l_Radial * l_Radius;
            o_Report.m_Normal = l_Radial;
            return TRUE;
        }
    }
    return FALSE;
}

int SolveCubic(Double* i_Coefficients, Double* o_Roots) {
    Double l_A = i_Coefficients[2] / i_Coefficients[3];
    Double l_B = i_Coefficients[1] / i_Coefficients[3];
    Double l_C = i_Coefficients[0] / i_Coefficients[3];
    Double l_A2 = l_A * l_A;
    Double l_P = 1.0 / 3 * (-1.0 / 3 * l_A2 + l_B);
    Double l_Q = 1.0 / 2 * (2.0 / 27 * l_A * l_A2 - 1.0 / 3 * l_A * l_B + l_C);
    Double l_P3 = l_P * l_P * l_P;
    Double l_Discriminant = l_Q * l_Q + l_P3;
    int l_Count;
    if (l_Discriminant > -1.e-9f && l_Discriminant < 1.e-9f) {
        if (l_Q > -1.e-9f && l_Q < 1.e-9f) {
            o_Roots[0] = 0;
            l_Count = 1;
        }
        else {
            Double l_U = CollisionCubeRoot(-l_Q);
            o_Roots[0] = 2 * l_U;
            o_Roots[1] = -l_U;
            l_Count = 2;
        }
    }
    else if (l_Discriminant < 0) {
        Double l_Phi = 1.0 / 3 * acos(-l_Q / sqrt(-l_P3));
        Double l_T = 2 * sqrt(-l_P);
        o_Roots[0] = l_T * O_Cos(l_Phi);
        o_Roots[1] = -l_T * O_Cos(l_Phi + Pi / 3);
        o_Roots[2] = -l_T * O_Cos(l_Phi - Pi / 3);
        l_Count = 3;
    }
    else {
        Double l_Sqrt = sqrt(l_Discriminant);
        Double l_U = CollisionCubeRoot(l_Sqrt - l_Q);
        Double l_V = -CollisionCubeRoot(l_Sqrt + l_Q);
        o_Roots[0] = l_U + l_V;
        l_Count = 1;
    }
    Double l_Shift = 1.0 / 3 * l_A;
    for (int i = 0; i < l_Count; ++i) {
        o_Roots[i] -= l_Shift;
    }
    return l_Count;
}

int SolveQuartic(Double* i_Coefficients, Double* o_Roots) {
    Double l_Coefficients[4];
    Double l_A = i_Coefficients[3] / i_Coefficients[4];
    Double l_B = i_Coefficients[2] / i_Coefficients[4];
    Double l_C = i_Coefficients[1] / i_Coefficients[4];
    Double l_D = i_Coefficients[0] / i_Coefficients[4];
    Double l_A2 = l_A * l_A;
    Double l_P = -3.0 / 8 * l_A2 + l_B;
    Double l_Q = 1.0 / 8 * l_A2 * l_A - 1.0 / 2 * l_A * l_B + l_C;
    Double l_R = -3.0 / 256 * l_A2 * l_A2 + 1.0 / 16 * l_A2 * l_B - 1.0 / 4 * l_A * l_C + l_D;
    int l_Count;
    if (l_R > -1.e-9f && l_R < 1.e-9f) {
        l_Coefficients[0] = l_Q;
        l_Coefficients[1] = l_P;
        l_Coefficients[2] = 0;
        l_Coefficients[3] = 1;
        l_Count = SolveCubic(l_Coefficients, o_Roots);
        o_Roots[l_Count++] = 0;
    }
    else {
        l_Coefficients[0] = 1.0 / 2 * l_R * l_P - 1.0 / 8 * l_Q * l_Q;
        l_Coefficients[1] = -l_R;
        l_Coefficients[2] = -1.0 / 2 * l_P;
        l_Coefficients[3] = 1;
        SolveCubic(l_Coefficients, o_Roots);
        Double l_Z = o_Roots[0];
        Double l_U = l_Z * l_Z - l_R;
        Double l_V = 2 * l_Z - l_P;
        if (l_U > -1.e-9f && l_U < 1.e-9f) {
            l_U = 0;
        }
        else if (l_U > 0) {
            l_U = sqrt(l_U);
        }
        else {
            return 0;
        }
        if (l_V > -1.e-9f && l_V < 1.e-9f) {
            l_V = 0;
        }
        else if (l_V > 0) {
            l_V = sqrt(l_V);
        }
        else {
            return 0;
        }
        l_Coefficients[0] = l_Z - l_U;
        l_Coefficients[1] = l_Q < 0 ? -l_V : l_V;
        l_Coefficients[2] = 1;
        l_Count = SolveQuadric(l_Coefficients, o_Roots);
        l_Coefficients[0] = l_Z + l_U;
        l_Coefficients[1] = l_Q < 0 ? l_V : -l_V;
        l_Coefficients[2] = 1;
        l_Count += SolveQuadric(l_Coefficients, o_Roots + l_Count);
    }
    Double l_Shift = 1.0 / 4 * l_A;
    for (int i = 0; i < l_Count; ++i) {
        o_Roots[i] -= l_Shift;
    }
    return l_Count;
}

Bool SphereVsEdge(const Sphere_Z& i_Sphere, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
    static Vec4f DirUp(0.0f, 1.0f, 0.0f, 1.0f);
    return FALSE;
}

Vec4f _vBestNormal;
static Float fBestDepth;

Bool CylinderSphereVsSegment(const Sphere_Z& i_Sphere, const Segment_Z& i_Segment) {
    return FALSE;
}

Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report) {
    return FALSE;
}

Bool SegmentVsBox(const Segment_Z& i_Segment, const Box_Z& i_Box, CollisionReport_Z& o_Report) {
    return FALSE;
}
