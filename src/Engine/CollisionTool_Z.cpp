#include "CollisionTool_Z.h"
#include "MathTools_Z.h"

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
            Vec4f l_OverlapNormal = -l_Offset;
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
        Vec4f l_Normal = o_Report.m_Intersection - i_Sphere.Center;
        o_Report.m_Normal = NormalizeCollisionNormal(l_Normal);
        return TRUE;
    }
    return FALSE;
}

Bool SegmentVsCylindre(const Segment_Z& i_Segment, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report) {
    Vec3f l_Top = i_Cylindre.Seg.Origin + i_Cylindre.Seg.Direction * i_Cylindre.Seg.Length;
    Float l_BaseD = -(i_Cylindre.Seg.Direction * i_Cylindre.Seg.Origin);
    Float l_TopD = -(i_Cylindre.Seg.Direction * l_Top);
    Vec3f l_End = i_Segment.Origin + i_Segment.Length * i_Segment.Direction;
    Vec3f l_ProjectedStart = i_Segment.Origin - ((i_Segment.Origin - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction) * i_Cylindre.Seg.Direction;
    Vec3f l_ProjectedEnd = l_End - ((l_End - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction) * i_Cylindre.Seg.Direction;
    Segment_Z l_ProjectedSegment(l_ProjectedStart, l_ProjectedEnd);
    Float l_Projection = (i_Cylindre.Seg.Origin - l_ProjectedSegment.Origin) * l_ProjectedSegment.Direction;
    Vec3f l_Orthogonal = l_Projection * l_ProjectedSegment.Direction + l_ProjectedSegment.Origin - i_Cylindre.Seg.Origin;
    Float l_Orthogonal2 = l_Orthogonal * l_Orthogonal, l_Radius2 = i_Cylindre.Radius * i_Cylindre.Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }
    if (i_Segment.Direction * i_Cylindre.Seg.Direction > 0.f) {
        Float l_PlaneDistance = i_Cylindre.Seg.Direction * i_Segment.Origin + l_BaseD;
        if (l_PlaneDistance < 0.f) {
            Float l_HitDistance = -(l_PlaneDistance / (i_Cylindre.Seg.Direction * i_Segment.Direction));
            if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > i_Segment.Length) {
                return FALSE;
            }
            Vec3f l_Hit = i_Segment.Direction * l_HitDistance + i_Segment.Origin;
            if ((l_Hit - i_Cylindre.Seg.Origin).GetNorm2() < l_Radius2) {
                o_Report.m_CollisionDistance = l_HitDistance;
                o_Report.m_Intersection = l_Hit;
                o_Report.m_Normal = -i_Cylindre.Seg.Direction;
                return TRUE;
            }
        }
    }
    else {
        Float l_PlaneDistance = i_Cylindre.Seg.Direction * i_Segment.Origin + l_TopD;
        if (l_PlaneDistance > 0.f) {
            Float l_HitDistance = -(l_PlaneDistance / (i_Cylindre.Seg.Direction * i_Segment.Direction));
            if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > i_Segment.Length) {
                return FALSE;
            }
            Vec3f l_Hit = i_Segment.Direction * l_HitDistance + i_Segment.Origin;
            if ((l_Hit - l_Top).GetNorm2() < l_Radius2) {
                o_Report.m_CollisionDistance = l_HitDistance;
                o_Report.m_Intersection = l_Hit;
                o_Report.m_Normal = i_Cylindre.Seg.Direction;
                return TRUE;
            }
        }
    }
    Float l_ProjectedDistance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    Float l_HitDistance = l_ProjectedDistance * i_Segment.Length / l_ProjectedSegment.Length;
    if (l_HitDistance > o_Report.m_CollisionDistance || l_HitDistance < 0.f || l_HitDistance > i_Segment.Length) {
        return FALSE;
    }
    Vec3f l_Hit = i_Segment.Origin + l_HitDistance * i_Segment.Direction;
    Float l_HitHeight = (l_Hit - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction;
    if (l_HitHeight < 0.f || l_HitHeight > i_Cylindre.Seg.Length) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_HitDistance;
    o_Report.m_Intersection = l_Hit;
    Vec3f l_Normal = l_ProjectedSegment.Origin + l_ProjectedDistance * l_ProjectedSegment.Direction - i_Cylindre.Seg.Origin;
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
    Double l_Z, l_U, l_V, l_Shift;
    Double l_A, l_B, l_C, l_D;
    Double l_A2, l_P, l_Q, l_R;
    int i, l_Count;

    l_A = i_Coefficients[3] / i_Coefficients[4];
    l_B = i_Coefficients[2] / i_Coefficients[4];
    l_C = i_Coefficients[1] / i_Coefficients[4];
    l_D = i_Coefficients[0] / i_Coefficients[4];
    l_A2 = l_A * l_A;
    l_P = -3.0 / 8 * l_A2 + l_B;
    l_Q = 1.0 / 8 * l_A2 * l_A - 1.0 / 2 * l_A * l_B + l_C;
    l_R = -3.0 / 256 * l_A2 * l_A2 + 1.0 / 16 * l_A2 * l_B - 1.0 / 4 * l_A * l_C + l_D;
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
        l_Z = o_Roots[0];
        l_U = l_Z * l_Z - l_R;
        l_V = 2 * l_Z - l_P;
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
    l_Shift = 1.0 / 4 * l_A;
    for (i = 0; i < l_Count; ++i) {
        o_Roots[i] -= l_Shift;
    }
    return l_Count;
}

int inttor(Vec3f i_RayOrigin, Vec3f i_RayDirection, Float i_Radius, Float i_PlaneRadius, Float i_NormalRadius, int* o_HitCount, Double* o_Hits) {
    Vec3f l_Origin, l_Direction;
    Float l_Rho, l_A0, l_B0;
    Float l_F, l_L, l_T, l_G, l_Q, l_M, l_U;
    Double l_Coefficients[5];

    *o_HitCount = 0;
    l_Origin = i_RayOrigin;
    l_Direction = i_RayDirection;
    l_Rho = i_PlaneRadius * i_PlaneRadius / (i_NormalRadius * i_NormalRadius);
    l_A0 = 4.f * i_Radius * i_Radius;
    l_B0 = i_Radius * i_Radius - i_PlaneRadius * i_PlaneRadius;
    l_F = 1.f - l_Direction.y * l_Direction.y;
    l_L = 2.f * (l_Origin.x * l_Direction.x + l_Origin.z * l_Direction.z);
    l_T = l_Origin.x * l_Origin.x + l_Origin.z * l_Origin.z;
    l_G = l_F + l_Rho * l_Direction.y * l_Direction.y;
    l_Q = l_A0 / (l_G * l_G);
    l_M = (l_L + 2.f * l_Rho * l_Direction.y * l_Origin.y) / l_G;
    l_U = (l_T + l_Rho * l_Origin.y * l_Origin.y + l_B0) / l_G;
    l_Coefficients[4] = 1.f;
    l_Coefficients[3] = 2.f * l_M;
    l_Coefficients[2] = l_M * l_M + 2.f * l_U - l_Q * l_F;
    l_Coefficients[1] = 2.f * l_M * l_U - l_Q * l_L;
    l_Coefficients[0] = l_U * l_U - l_Q * l_T;
    *o_HitCount = SolveQuartic(l_Coefficients, o_Hits);
    l_M = Float(o_Hits[0]);
    l_U = Float(o_Hits[1]);
    o_Hits[0] = l_U;
    o_Hits[1] = l_M;
    l_M = Float(o_Hits[2]);
    l_U = Float(o_Hits[3]);
    o_Hits[2] = l_U;
    o_Hits[3] = l_M;
    return *o_HitCount != 0;
}

Bool MovingSphereVsCylindre(const Capsule_Z& i_Capsule, const Cylindre_Z& i_Cylindre, CollisionReport_Z& o_Report) {
    Vec3f l_CylinderEnd = i_Cylindre.Seg.Origin + i_Cylindre.Seg.Direction * i_Cylindre.Seg.Length;
    Float l_BasePlane = -(i_Cylindre.Seg.Direction * i_Cylindre.Seg.Origin);
    Float l_EndPlane = -(i_Cylindre.Seg.Direction * l_CylinderEnd);
    Vec3f l_CapsuleEnd = i_Capsule.Origin + i_Capsule.Length * i_Capsule.Direction;
    Vec3f l_ProjectedStart = i_Capsule.Origin - ((i_Capsule.Origin - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction) * i_Cylindre.Seg.Direction;
    Vec3f l_ProjectedEnd = l_CapsuleEnd - ((l_CapsuleEnd - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction) * i_Cylindre.Seg.Direction;
    Segment_Z l_ProjectedSegment(l_ProjectedStart, l_ProjectedEnd);
    Float l_Projection = (i_Cylindre.Seg.Origin - l_ProjectedSegment.Origin) * l_ProjectedSegment.Direction;
    Vec3f l_Orthogonal = l_Projection * l_ProjectedSegment.Direction + l_ProjectedSegment.Origin - i_Cylindre.Seg.Origin;
    Float l_Radius = i_Cylindre.Radius + i_Capsule.Radius;
    Float l_Orthogonal2 = l_Orthogonal * l_Orthogonal;
    Float l_Radius2 = l_Radius * l_Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }

    Float l_Distance;
    Vec3f l_Intersection;
    if (i_Capsule.Direction * i_Cylindre.Seg.Direction > 0.f) {
        Float l_PlaneDistance = i_Cylindre.Seg.Direction * i_Capsule.Origin + l_BasePlane + i_Capsule.Radius;
        if (l_PlaneDistance < 0.f) {
            l_Distance = -(l_PlaneDistance / (i_Cylindre.Seg.Direction * i_Capsule.Direction));
            if (l_Distance < o_Report.m_CollisionDistance && l_Distance >= 0.f && l_Distance <= i_Capsule.Length) {
                l_Intersection = i_Capsule.Direction * l_Distance + i_Capsule.Origin;
                if ((l_Intersection - (i_Cylindre.Seg.Origin - i_Capsule.Radius * i_Cylindre.Seg.Direction)).GetNorm2() < i_Cylindre.Radius * i_Cylindre.Radius) {
                    o_Report.m_CollisionDistance = l_Distance;
                    o_Report.m_Intersection = l_Intersection;
                    o_Report.m_Normal = -i_Cylindre.Seg.Direction;
                    return TRUE;
                }
            }
        }
    }
    else {
        Float l_PlaneDistance = i_Cylindre.Seg.Direction * i_Capsule.Origin + l_EndPlane - i_Capsule.Radius;
        if (l_PlaneDistance > 0.f) {
            l_Distance = -(l_PlaneDistance / (i_Cylindre.Seg.Direction * i_Capsule.Direction));
            if (l_Distance < o_Report.m_CollisionDistance && l_Distance >= 0.f && l_Distance <= i_Capsule.Length) {
                l_Intersection = i_Capsule.Direction * l_Distance + i_Capsule.Origin;
                if ((l_Intersection - (l_CylinderEnd + i_Capsule.Radius * i_Cylindre.Seg.Direction)).GetNorm2() < i_Cylindre.Radius * i_Cylindre.Radius) {
                    o_Report.m_CollisionDistance = l_Distance;
                    o_Report.m_Intersection = l_Intersection;
                    o_Report.m_Normal = i_Cylindre.Seg.Direction;
                    return TRUE;
                }
            }
        }
    }

    Float l_SurfaceDistance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    l_ProjectedSegment.Length = l_SurfaceDistance * i_Capsule.Length / l_ProjectedSegment.Length;
    if (l_ProjectedSegment.Length < o_Report.m_CollisionDistance && l_ProjectedSegment.Length >= 0.f && l_ProjectedSegment.Length <= i_Capsule.Length) {
        l_Intersection = i_Capsule.Origin + l_ProjectedSegment.Length * i_Capsule.Direction;
        Float l_Height = (l_Intersection - i_Cylindre.Seg.Origin) * i_Cylindre.Seg.Direction;
        if (l_Height > 0.f && l_Height < i_Cylindre.Seg.Length) {
            o_Report.m_CollisionDistance = l_ProjectedSegment.Length;
            o_Report.m_Intersection = l_Intersection;
            o_Report.m_Normal = (l_ProjectedSegment.Origin + l_SurfaceDistance * l_ProjectedSegment.Direction - i_Cylindre.Seg.Origin).Normalize();
            return TRUE;
        }
    }

    Mat4x4 l_Matrix;
    Vec3f l_XAxis, l_YAxis, l_ZAxis;
    l_YAxis = i_Cylindre.Seg.Direction;
    l_XAxis = l_YAxis ^ Vec3f(0.f, 0.f, 1.f);
    Float l_XLength = l_XAxis.GetNorm();
    if (l_XLength < Float_Eps) {
        l_XAxis = l_YAxis ^ Vec3f(1.f, 0.f, 0.f);
        l_XAxis.Normalize();
        l_ZAxis = l_XAxis ^ l_YAxis;
    }
    else {
        l_XAxis /= l_XLength;
        l_ZAxis = l_XAxis ^ l_YAxis;
    }
    l_Matrix.SetIdentity();
    l_Matrix.m[0][0] = l_XAxis.x;
    l_Matrix.m[0][1] = l_XAxis.y;
    l_Matrix.m[0][2] = l_XAxis.z;
    l_Matrix.m[1][0] = l_YAxis.x;
    l_Matrix.m[1][1] = l_YAxis.y;
    l_Matrix.m[1][2] = l_YAxis.z;
    l_Matrix.m[2][0] = l_ZAxis.x;
    l_Matrix.m[2][1] = l_ZAxis.y;
    l_Matrix.m[2][2] = l_ZAxis.z;
    l_Matrix.m[3][0] = i_Cylindre.Seg.Origin.x;
    l_Matrix.m[3][1] = i_Cylindre.Seg.Origin.y;
    l_Matrix.m[3][2] = i_Cylindre.Seg.Origin.z;

    Mat4x4 l_Inverse;
    Inverse2(l_Matrix, l_Inverse);
    Bool l_Hit = FALSE;
    int l_HitCount;
    Double l_Hits[4];
    Capsule_Z l_LocalCapsule = l_Inverse * i_Capsule;
    if (inttor(l_LocalCapsule.Origin, l_LocalCapsule.Direction, i_Cylindre.Radius, i_Capsule.Radius, i_Capsule.Radius, &l_HitCount, l_Hits)) {
        if (l_Hits[0] > 0.0 && l_Hits[0] < i_Capsule.Length) {
            l_Distance = Float(l_Hits[0]);
            if (l_Distance < o_Report.m_CollisionDistance && l_Distance >= 0.f && l_Distance <= i_Capsule.Length) {
                Vec3f l_LocalIntersection = l_LocalCapsule.Direction * l_Distance + l_LocalCapsule.Origin;
                Vec4f l_HorizontalNormal = l_LocalIntersection;
                l_HorizontalNormal.HNormalize();
                l_HorizontalNormal *= i_Cylindre.Radius;
                l_Hit = TRUE;
                o_Report.m_CollisionDistance = l_Distance;
                o_Report.m_Intersection = i_Capsule.Direction * l_Distance + i_Capsule.Origin;
                Vec4f l_LocalNormal = (Vec4f(l_LocalIntersection) - l_HorizontalNormal).Normalize();
                o_Report.m_Normal.x = l_Matrix.m[0][0] * l_LocalNormal.x + l_Matrix.m[1][0] * l_LocalNormal.y + l_Matrix.m[2][0] * l_LocalNormal.z;
                o_Report.m_Normal.y = l_Matrix.m[0][1] * l_LocalNormal.x + l_Matrix.m[1][1] * l_LocalNormal.y + l_Matrix.m[2][1] * l_LocalNormal.z;
                o_Report.m_Normal.z = l_Matrix.m[0][2] * l_LocalNormal.x + l_Matrix.m[1][2] * l_LocalNormal.y + l_Matrix.m[2][2] * l_LocalNormal.z;
            }
        }
    }

    l_Inverse.m[3][0] = -l_CylinderEnd.x;
    l_Inverse.m[3][1] = -l_CylinderEnd.y;
    l_Inverse.m[3][2] = -l_CylinderEnd.z;
    l_Matrix.m[3][0] = l_CylinderEnd.x;
    l_Matrix.m[3][1] = l_CylinderEnd.y;
    l_Matrix.m[3][2] = l_CylinderEnd.z;
    l_LocalCapsule = l_Inverse * i_Capsule;
    if (inttor(l_LocalCapsule.Origin, l_LocalCapsule.Direction, i_Cylindre.Radius, i_Capsule.Radius, i_Capsule.Radius, &l_HitCount, l_Hits)) {
        if (l_Hits[0] > 0.0 && l_Hits[0] < i_Capsule.Length) {
            l_Distance = Float(l_Hits[0]);
            if (l_Distance < o_Report.m_CollisionDistance && l_Distance >= 0.f && l_Distance <= i_Capsule.Length) {
                Vec3f l_LocalIntersection = l_LocalCapsule.Direction * l_Distance + l_LocalCapsule.Origin;
                Vec4f l_HorizontalNormal = l_LocalIntersection;
                l_HorizontalNormal.HNormalize();
                l_HorizontalNormal *= i_Cylindre.Radius;
                o_Report.m_CollisionDistance = l_Distance;
                o_Report.m_Intersection = i_Capsule.Direction * l_Distance + i_Capsule.Origin;
                Vec4f l_LocalNormal = (Vec4f(l_LocalIntersection) - l_HorizontalNormal).Normalize();
                o_Report.m_Normal.x = l_Matrix.m[0][0] * l_LocalNormal.x + l_Matrix.m[1][0] * l_LocalNormal.y + l_Matrix.m[2][0] * l_LocalNormal.z;
                o_Report.m_Normal.y = l_Matrix.m[0][1] * l_LocalNormal.x + l_Matrix.m[1][1] * l_LocalNormal.y + l_Matrix.m[2][1] * l_LocalNormal.z;
                o_Report.m_Normal.z = l_Matrix.m[0][2] * l_LocalNormal.x + l_Matrix.m[1][2] * l_LocalNormal.y + l_Matrix.m[2][2] * l_LocalNormal.z;
                return TRUE;
            }
        }
    }
    return l_Hit;
}

Bool CylinderSphereVsSegment(const Sphere_Z& i_Sphere, const Segment_Z& i_Segment) {
    Float l_X = i_Sphere.Center.x - i_Segment.Origin.x;
    Float l_Z = i_Sphere.Center.z - i_Segment.Origin.z;
    Float l_Radius2 = i_Sphere.Radius * i_Sphere.Radius;
    Float l_Projection = l_Z * i_Segment.Direction.z + l_X * i_Segment.Direction.x;
    Float l_OrthogonalX = i_Segment.Direction.x * l_Projection;
    Float l_OrthogonalZ = i_Segment.Direction.z * l_Projection;
    l_OrthogonalX -= l_X;
    l_OrthogonalZ -= l_Z;
    if (l_OrthogonalX * l_OrthogonalX + l_OrthogonalZ * l_OrthogonalZ > l_Radius2) {
        return FALSE;
    }
    if (l_Projection < 0.f) {
        l_X = i_Segment.Origin.x - i_Sphere.Center.x;
        l_Z = i_Segment.Origin.z - i_Sphere.Center.z;
        return l_X * l_X + l_Z * l_Z < l_Radius2;
    }
    if (l_Projection > i_Segment.Length) {
        l_X = i_Segment.Direction.x * i_Segment.Length;
        l_Z = i_Segment.Direction.z * i_Segment.Length;
        l_X += i_Segment.Origin.x;
        l_Z += i_Segment.Origin.z;
        l_X -= i_Sphere.Center.x;
        l_Z -= i_Sphere.Center.z;
        return l_X * l_X + l_Z * l_Z < l_Radius2;
    }
    return TRUE;
}

Bool SphereVsCapsule(const Sphere_Z& i_Sphere, const Capsule_Z& i_Capsule) {
    Float l_Projection;
    Vec3f l_Orthogonal;
    Float l_Radius2;
    l_Projection = (i_Sphere.Center - i_Capsule.Origin) * i_Capsule.Direction;
    l_Orthogonal = l_Projection * i_Capsule.Direction + i_Capsule.Origin - i_Sphere.Center;
    l_Radius2 = (i_Sphere.Radius + i_Capsule.Radius) * (i_Sphere.Radius + i_Capsule.Radius);
    if (l_Orthogonal * l_Orthogonal > l_Radius2) {
        return FALSE;
    }
    if (l_Projection < 0.f) {
        return (i_Capsule.Origin - i_Sphere.Center).GetNorm2() < l_Radius2;
    }
    if (l_Projection > i_Capsule.Length) {
        return ((i_Capsule.Origin + i_Capsule.Direction * i_Capsule.Length) - i_Sphere.Center).GetNorm2() < l_Radius2;
    }
    return TRUE;
}

Bool CylinderSphereVsCapsule(const Sphere_Z& i_Sphere, const Capsule_Z& i_Capsule) {
    Float l_Projection = Vec2f(i_Sphere.Center.z - i_Capsule.Origin.z, i_Sphere.Center.x - i_Capsule.Origin.x) * Vec2f(i_Capsule.Direction.z, i_Capsule.Direction.x);
    Vec2f l_Orthogonal = l_Projection * Vec2f(i_Capsule.Direction.z, i_Capsule.Direction.x) + Vec2f(i_Capsule.Origin.z, i_Capsule.Origin.x) - Vec2f(i_Sphere.Center.z, i_Sphere.Center.x);
    Float l_Radius2 = (i_Sphere.Radius + i_Capsule.Radius) * (i_Sphere.Radius + i_Capsule.Radius);
    if (l_Orthogonal * l_Orthogonal > l_Radius2) {
        return FALSE;
    }
    if (l_Projection < 0.f) {
        return (Vec2f(i_Capsule.Origin.z, i_Capsule.Origin.x) - Vec2f(i_Sphere.Center.z, i_Sphere.Center.x)).GetNorm2() < l_Radius2;
    }
    if (l_Projection > i_Capsule.Length) {
        return ((Vec2f(i_Capsule.Origin.z, i_Capsule.Origin.x) + Vec2f(i_Capsule.Direction.z, i_Capsule.Direction.x) * i_Capsule.Length) - Vec2f(i_Sphere.Center.z, i_Sphere.Center.x)).GetNorm2() < l_Radius2;
    }
    return TRUE;
}

Bool SphereVsEdge(const Sphere_Z& i_Sphere, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
    static Vec4f DirUp(0.0f, 1.0f, 0.0f, 1.0f);
    Vec4f l_Direction = i_V1 - i_V0;
    Vec4f l_HorizontalDirection(l_Direction.x, 0.f, l_Direction.z, 1.f);
    Float l_Projection = (Vec4f(i_Sphere.Center) - i_V0) * l_HorizontalDirection / (l_HorizontalDirection * l_HorizontalDirection);
    Float l_Height = -(l_Projection * l_Direction.y - i_Sphere.Center.y) - i_V0.y;
    Float l_ClampedProjection = l_Projection;
    if (l_Projection < 0.f) {
        l_ClampedProjection = 0.f;
    }
    else if (l_Projection > 1.f) {
        l_ClampedProjection = 1.f;
    }
    Float l_ClampedHeight = l_Height;
    if (l_Height < 0.f) {
        l_ClampedHeight = 0.f;
    }
    Vec4f l_Difference = Vec4f(i_Sphere.Center) - (l_Direction * l_ClampedProjection + DirUp * l_ClampedHeight + i_V0);
    Float l_Distance2 = l_Difference.GetNorm2();
    if (l_Distance2 > i_Sphere.Radius * i_Sphere.Radius) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = Sqrt(l_Distance2);
    o_Report.m_Intersection = Vec4f(i_Sphere.Center) - l_Difference;
    o_Report.m_Normal = Vec4f(i_Sphere.Center) - (l_Direction * l_Projection + DirUp * l_Height + i_V0);
    Float l_Normal2 = o_Report.m_Normal.GetNorm2();
    if (l_Normal2 > Float_Eps) {
        o_Report.m_Normal *= InvSqrt(l_Normal2);
    }
    return TRUE;
}

Bool MovingSphereVsEdge(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
    Vec4f l_EdgeDirection = i_V1 - i_V0;
    Float l_EdgeLength = Sqrt(l_EdgeDirection.GetNorm2());
    if (l_EdgeLength > Float_Eps) {
        l_EdgeDirection /= l_EdgeLength;
    }
    Vec4f l_CapsuleOrigin(i_Capsule.Origin);
    Vec4f l_CapsuleDirection(i_Capsule.Direction);
    Vec4f l_CapsuleEnd = l_CapsuleOrigin + i_Capsule.Length * l_CapsuleDirection;
    Vec4f l_ProjectedStart = l_CapsuleOrigin - ((l_CapsuleOrigin - i_V0) * l_EdgeDirection) * l_EdgeDirection;
    Vec4f l_ProjectedEnd = l_CapsuleEnd - ((l_CapsuleEnd - i_V0) * l_EdgeDirection) * l_EdgeDirection;
    Vec4f l_ProjectedDirection = l_ProjectedEnd - l_ProjectedStart;
    Float l_ProjectedLength = Sqrt(l_ProjectedDirection.GetNorm2());
    if (l_ProjectedLength > Float_Eps) {
        l_ProjectedDirection /= l_ProjectedLength;
    }
    Float l_Exit = (l_ProjectedStart - i_V0) * l_ProjectedDirection;
    if (l_Exit > 0.f) {
        return FALSE;
    }
    Float l_Projection = (i_V0 - l_ProjectedStart) * l_ProjectedDirection;
    Vec4f l_Orthogonal = l_Projection * l_ProjectedDirection + l_ProjectedStart - i_V0;
    Float l_Orthogonal2 = l_Orthogonal.GetNorm2();
    Float l_Radius2 = i_Capsule.Radius * i_Capsule.Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }
    Vec4f l_OriginDifference = l_ProjectedStart - i_V0;
    if (l_OriginDifference.GetNorm2() < l_Radius2) {
        Float l_EdgeProjection = (l_CapsuleOrigin - i_V0) * l_EdgeDirection;
        if (l_EdgeProjection < 0.f || l_EdgeProjection > l_EdgeLength) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = l_CapsuleOrigin;
        Float l_InvNormal = InvSqrt(l_OriginDifference.GetNorm2());
        o_Report.m_Normal.x = l_OriginDifference.x * l_InvNormal;
        o_Report.m_Normal.y = l_OriginDifference.y * l_InvNormal;
        o_Report.m_Normal.z = l_OriginDifference.z * l_InvNormal;
        o_Report.m_Normal.w = 1.f;
        return TRUE;
    }
    Float l_SurfaceDistance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    Float l_Distance = l_SurfaceDistance * i_Capsule.Length / l_ProjectedLength;
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec4f l_Intersection = l_CapsuleOrigin + l_Distance * l_CapsuleDirection;
    Float l_EdgeProjection = (l_Intersection - i_V0) * l_EdgeDirection;
    if (l_EdgeProjection < 0.f || l_EdgeProjection > l_EdgeLength) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    Vec4f l_Normal = l_ProjectedStart + l_SurfaceDistance * l_ProjectedDirection - i_V0;
    Float l_InvNormal = InvSqrt(l_Normal.GetNorm2());
    o_Report.m_Normal.x = l_Normal.x * l_InvNormal;
    o_Report.m_Normal.y = l_Normal.y * l_InvNormal;
    o_Report.m_Normal.z = l_Normal.z * l_InvNormal;
    o_Report.m_Normal.w = 1.f;
    return TRUE;
}

Bool MovingSphereVsDirEdge(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_Direction, CollisionReport_Z& o_Report) {
    Vec4f l_CapsuleOrigin(i_Capsule.Origin);
    Vec4f l_OriginToEdge = l_CapsuleOrigin - i_V0;
    Float l_MaxDistance = i_Direction.w + i_Capsule.Length + i_Capsule.Radius;
    if (l_OriginToEdge.GetNorm2() > l_MaxDistance * l_MaxDistance) {
        return FALSE;
    }
    Vec4f l_CapsuleDirection(i_Capsule.Direction);
    Vec4f l_CapsuleEnd = l_CapsuleOrigin + i_Capsule.Length * l_CapsuleDirection;
    Float l_OriginProjection = l_OriginToEdge * i_Direction;
    Float l_EndProjection = (l_CapsuleEnd - i_V0) * i_Direction;
    Vec4f l_ProjectedStart = l_CapsuleOrigin - l_OriginProjection * i_Direction;
    Vec4f l_ProjectedEnd = l_CapsuleEnd - l_EndProjection * i_Direction;
    Vec4f l_ProjectedDirection = l_ProjectedEnd - l_ProjectedStart;
    Float l_ProjectedLength = Sqrt(l_ProjectedDirection.GetNorm2());
    if (l_ProjectedLength > Float_Eps) {
        l_ProjectedDirection /= l_ProjectedLength;
    }
    Float l_Projection = (i_V0 - l_ProjectedStart) * l_ProjectedDirection;
    if (l_Projection < 0.f) {
        return FALSE;
    }
    Vec4f l_Orthogonal = l_Projection * l_ProjectedDirection + l_ProjectedStart - i_V0;
    Float l_Orthogonal2 = l_Orthogonal.GetNorm2();
    Float l_Radius2 = i_Capsule.Radius * i_Capsule.Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }
    Vec4f l_OriginDifference = l_ProjectedStart - i_V0;
    if (l_OriginDifference.GetNorm2() < l_Radius2) {
        if (l_OriginProjection < 0.f || l_OriginProjection > i_Direction.w) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = l_CapsuleOrigin;
        Float l_InvNormal = InvSqrt(l_OriginDifference.GetNorm2());
        o_Report.m_Normal.x = l_OriginDifference.x * l_InvNormal;
        o_Report.m_Normal.y = l_OriginDifference.y * l_InvNormal;
        o_Report.m_Normal.z = l_OriginDifference.z * l_InvNormal;
        o_Report.m_Normal.w = 1.f;
        return TRUE;
    }
    Float l_SurfaceDistance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    Float l_Distance = l_SurfaceDistance * i_Capsule.Length / l_ProjectedLength;
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec4f l_Intersection = l_CapsuleOrigin + l_Distance * l_CapsuleDirection;
    Float l_EdgeProjection = (l_Intersection - i_V0) * i_Direction;
    if (l_EdgeProjection < 0.f || l_EdgeProjection > i_Direction.w) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    Vec4f l_Normal = l_ProjectedStart + l_SurfaceDistance * l_ProjectedDirection - i_V0;
    Float l_InvNormal = InvSqrt(l_Normal.GetNorm2());
    o_Report.m_Normal.x = l_Normal.x * l_InvNormal;
    o_Report.m_Normal.y = l_Normal.y * l_InvNormal;
    o_Report.m_Normal.z = l_Normal.z * l_InvNormal;
    o_Report.m_Normal.w = 1.f;
    return TRUE;
}

Bool MovingSphereVsEdgeInfinyUp(const Capsule_Z& i_Capsule, const Segment_Z& i_Edge, CollisionReport_Z& o_Report) {
    Float l_X = i_Capsule.Origin.x - i_Edge.Origin.x;
    Float l_Z = i_Capsule.Origin.z - i_Edge.Origin.z;
    Float l_MaxDistance = i_Edge.Length + i_Capsule.Length + i_Capsule.Radius;
    if (l_X * l_X + l_Z * l_Z > l_MaxDistance * l_MaxDistance) {
        return FALSE;
    }
    Vec3f l_CapsuleEnd = i_Capsule.Origin + i_Capsule.Length * i_Capsule.Direction;
    Vec3f l_ProjectedStart = i_Capsule.Origin - ((i_Capsule.Origin - i_Edge.Origin) * i_Edge.Direction) * i_Edge.Direction;
    Vec3f l_ProjectedEnd = l_CapsuleEnd - ((l_CapsuleEnd - i_Edge.Origin) * i_Edge.Direction) * i_Edge.Direction;
    Segment_Z l_ProjectedSegment(l_ProjectedStart, l_ProjectedEnd);
    Float l_Projection = (i_Edge.Origin - l_ProjectedSegment.Origin) * l_ProjectedSegment.Direction;
    if (l_Projection < 0.f) {
        return FALSE;
    }
    Vec3f l_Orthogonal = l_Projection * l_ProjectedSegment.Direction + l_ProjectedSegment.Origin - i_Edge.Origin;
    Float l_Orthogonal2 = l_Orthogonal.GetNorm2();
    Float l_Radius2 = i_Capsule.Radius * i_Capsule.Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }
    Vec3f l_OriginDifference = l_ProjectedSegment.Origin - i_Edge.Origin;
    if (l_OriginDifference.GetNorm2() < l_Radius2) {
        Float l_EdgeProjection = (i_Capsule.Origin - i_Edge.Origin) * i_Edge.Direction;
        if (l_EdgeProjection < 0.f) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = i_Capsule.Origin;
        Float l_InvNormal = InvSqrt(l_OriginDifference.GetNorm2());
        o_Report.m_Normal.x = l_OriginDifference.x * l_InvNormal;
        o_Report.m_Normal.y = l_OriginDifference.y * l_InvNormal;
        o_Report.m_Normal.z = l_OriginDifference.z * l_InvNormal;
        o_Report.m_Normal.w = 1.f;
        return TRUE;
    }
    Float l_SurfaceDistance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    Float l_Distance = l_SurfaceDistance * i_Capsule.Length / l_ProjectedSegment.Length;
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Capsule.Origin + l_Distance * i_Capsule.Direction;
    if ((l_Intersection - i_Edge.Origin) * i_Edge.Direction < 0.f) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    Vec3f l_Normal = l_ProjectedSegment.Origin + l_SurfaceDistance * l_ProjectedSegment.Direction - i_Edge.Origin;
    Float l_InvNormal = InvSqrt(l_Normal.GetNorm2());
    o_Report.m_Normal.x = l_Normal.x * l_InvNormal;
    o_Report.m_Normal.y = l_Normal.y * l_InvNormal;
    o_Report.m_Normal.z = l_Normal.z * l_InvNormal;
    o_Report.m_Normal.w = 1.f;
    return TRUE;
}

Bool MovingSphereVsBox(const Capsule_Z& i_Capsule, const Box_Z& i_Box, CollisionReport_Z& o_Report) {
    int i;
    Vec3f l_Points[8];
    S32* l_Index = CubeIndex;
    i_Box.GetVtx(l_Points);
    Bool l_Hit = FALSE;
    for (i = 0; i < 8; ++i) {
        if (MovingSphereVsVertex(i_Capsule, Vec4f(l_Points[i]), o_Report)) {
            l_Hit = TRUE;
        }
    }
    for (i = 0; i < 12; ++i) {
        if (MovingSphereVsEdge(i_Capsule, Vec4f(l_Points[CubeEdge[i * 2]]), Vec4f(l_Points[CubeEdge[i * 2 + 1]]), o_Report)) {
            l_Hit = TRUE;
        }
    }
    for (i = 0; i < 6; ++i) {
        if (MovingSphereVsQuad(i_Capsule, Vec4f(l_Points[l_Index[i * 4]]), Vec4f(l_Points[l_Index[i * 4 + 1]]), Vec4f(l_Points[l_Index[i * 4 + 2]]), Vec4f(l_Points[l_Index[i * 4 + 3]]), o_Report)) {
            l_Hit = TRUE;
        }
    }
    return l_Hit;
}

Bool MovingSphereVsQuad(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, const Vec4f& i_V3, CollisionReport_Z& o_Report) {
    Vec3f l_Normal;
    l_Normal.x = i_V0.y * (i_V1.z - i_V2.z) + i_V1.y * (i_V2.z - i_V0.z) + i_V2.y * (i_V0.z - i_V1.z);
    l_Normal.y = i_V0.z * (i_V1.x - i_V2.x) + i_V1.z * (i_V2.x - i_V0.x) + i_V2.z * (i_V0.x - i_V1.x);
    l_Normal.z = i_V0.x * (i_V1.y - i_V2.y) + i_V1.x * (i_V2.y - i_V0.y) + i_V2.x * (i_V0.y - i_V1.y);
    Float l_Norm = Sqrt(l_Normal.GetNorm2());
    if (l_Norm <= Float_Eps) {
        return FALSE;
    }
    Float l_InvNorm = 1.f / l_Norm;
    l_Normal.x *= l_InvNorm;
    l_Normal.y *= l_InvNorm;
    l_Normal.z *= l_InvNorm;
    Float l_Exit = i_Capsule.Direction * l_Normal;
    if (l_Exit >= 0.f) {
        return FALSE;
    }
    Float l_PlaneDistance = l_Normal.x * (i_Capsule.Origin.x - i_V0.x) + l_Normal.y * (i_Capsule.Origin.y - i_V0.y) + l_Normal.z * (i_Capsule.Origin.z - i_V0.z);
    if (l_PlaneDistance <= 0.f) {
        return FALSE;
    }
    if (l_PlaneDistance <= i_Capsule.Radius) {
        Vec3f l_Intersection = i_Capsule.Origin;
        if (CollisionEdgeSide(i_V0, i_V1, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        if (CollisionEdgeSide(i_V1, i_V2, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        if (CollisionEdgeSide(i_V2, i_V3, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        if (CollisionEdgeSide(i_V3, i_V0, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = l_Intersection;
        o_Report.m_Normal = l_Normal;
        return TRUE;
    }
    Float l_Distance = -((l_PlaneDistance - i_Capsule.Radius) / l_Exit);
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Capsule.Origin + l_Distance * i_Capsule.Direction;
    if (CollisionEdgeSide(i_V0, i_V1, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V1, i_V2, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V2, i_V3, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V3, i_V0, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    o_Report.m_Normal = l_Normal;
    return TRUE;
}

Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report) {
    Vec4f l_V0, l_V1, l_V2;
    Vec3f_S16_Z::Get(i_V0, i_V1, i_V2, l_V0, l_V1, l_V2);
    return SegmentVsTri(i_Segment, l_V0, l_V1, l_V2, o_Report);
}

Bool SegmentVsTri(const Segment_Z& i_Segment, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, CollisionReport_Z& o_Report) {
    Vec3f l_Normal;
    l_Normal.x = (i_V1.y - i_V0.y) * (i_V2.z - i_V0.z) - (i_V1.z - i_V0.z) * (i_V2.y - i_V0.y);
    l_Normal.y = (i_V1.z - i_V0.z) * (i_V2.x - i_V0.x) - (i_V1.x - i_V0.x) * (i_V2.z - i_V0.z);
    l_Normal.z = (i_V1.x - i_V0.x) * (i_V2.y - i_V0.y) - (i_V1.y - i_V0.y) * (i_V2.x - i_V0.x);
    Float l_Norm = Sqrt(l_Normal.GetNorm2());
    if (l_Norm <= Float_Eps) {
        return FALSE;
    }
    Float l_InvNorm = 1.f / l_Norm;
    l_Normal.x *= l_InvNorm;
    l_Normal.y *= l_InvNorm;
    l_Normal.z *= l_InvNorm;
    Float l_PlaneDistance = l_Normal.x * (i_Segment.Origin.x - i_V0.x) + l_Normal.y * (i_Segment.Origin.y - i_V0.y) + l_Normal.z * (i_Segment.Origin.z - i_V0.z);
    if (l_PlaneDistance <= 0.f) {
        return FALSE;
    }
    Float l_Distance = -(l_PlaneDistance / (l_Normal * i_Segment.Direction));
    if (l_Distance < 0.f || l_Distance > i_Segment.Length || l_Distance > o_Report.m_CollisionDistance) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Segment.Origin + l_Distance * i_Segment.Direction;
    if (CollisionEdgeSide(i_V0, i_V1, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V1, i_V2, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V2, i_V0, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    o_Report.m_Normal = l_Normal;
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    return TRUE;
}

Bool MovingSphereVsTri(const Capsule_Z& i_Capsule, const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, CollisionReport_Z& o_Report) {
    Vec4f l_V0, l_V1, l_V2;
    i_V0.Get(l_V0);
    i_V1.Get(l_V1);
    i_V2.Get(l_V2);
    return MovingSphereVsTri(i_Capsule, l_V0, l_V1, l_V2, o_Report);
}

Bool MovingSphereVsTri(const Capsule_Z& i_Capsule, const Vec4f& i_V0, const Vec4f& i_V1, const Vec4f& i_V2, CollisionReport_Z& o_Report) {
    Vec3f l_Normal;
    l_Normal.x = i_V0.y * (i_V1.z - i_V2.z) + i_V1.y * (i_V2.z - i_V0.z) + i_V2.y * (i_V0.z - i_V1.z);
    l_Normal.y = i_V0.z * (i_V1.x - i_V2.x) + i_V1.z * (i_V2.x - i_V0.x) + i_V2.z * (i_V0.x - i_V1.x);
    l_Normal.z = i_V0.x * (i_V1.y - i_V2.y) + i_V1.x * (i_V2.y - i_V0.y) + i_V2.x * (i_V0.y - i_V1.y);
    Float l_Norm = Sqrt(l_Normal.GetNorm2());
    if (l_Norm <= Float_Eps) {
        return FALSE;
    }
    Float l_InvNorm = 1.f / l_Norm;
    l_Normal.x *= l_InvNorm;
    l_Normal.y *= l_InvNorm;
    l_Normal.z *= l_InvNorm;
    Float l_Exit = i_Capsule.Direction * l_Normal;
    if (l_Exit >= 0.f) {
        return FALSE;
    }
    Float l_PlaneDistance = l_Normal.x * (i_Capsule.Origin.x - i_V0.x) + l_Normal.y * (i_Capsule.Origin.y - i_V0.y) + l_Normal.z * (i_Capsule.Origin.z - i_V0.z);
    if (l_PlaneDistance <= 0.f) {
        return FALSE;
    }
    if (l_PlaneDistance <= i_Capsule.Radius) {
        Vec3f l_Intersection = i_Capsule.Origin;
        if (CollisionEdgeSide(i_V0, i_V1, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        if (CollisionEdgeSide(i_V1, i_V2, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        if (CollisionEdgeSide(i_V2, i_V0, l_Intersection, l_Normal) < -1.e-4f) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = l_Intersection;
        o_Report.m_Normal = l_Normal;
        return TRUE;
    }
    Float l_Distance = (i_Capsule.Radius - l_PlaneDistance) / l_Exit;
    if (l_Distance < 0.f || l_Distance > i_Capsule.Length || l_Distance > o_Report.m_CollisionDistance) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Capsule.Origin + l_Distance * i_Capsule.Direction;
    if (CollisionEdgeSide(i_V0, i_V1, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V1, i_V2, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionEdgeSide(i_V2, i_V0, l_Intersection, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    o_Report.m_Normal = l_Normal;
    return TRUE;
}

Bool MovingSphereVsVertex(const Capsule_Z& i_Capsule, const Vec4f& i_Vertex, CollisionReport_Z& o_Report) {
    Vec3f l_ToVertex(i_Vertex.x - i_Capsule.Origin.x, i_Vertex.y - i_Capsule.Origin.y, i_Vertex.z - i_Capsule.Origin.z);
    Float l_Projection = l_ToVertex * i_Capsule.Direction;
    if (l_Projection < 0.f) {
        return FALSE;
    }
    Vec3f l_Orthogonal = l_Projection * i_Capsule.Direction + i_Capsule.Origin - Vec3f(i_Vertex);
    Float l_Orthogonal2 = l_Orthogonal.GetNorm2();
    Float l_Radius2 = i_Capsule.Radius * i_Capsule.Radius;
    if (l_Orthogonal2 > l_Radius2) {
        return FALSE;
    }
    Vec3f l_OriginDifference = i_Capsule.Origin - Vec3f(i_Vertex);
    if (l_OriginDifference.GetNorm2() < l_Radius2) {
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = i_Capsule.Origin;
        Float l_InvNormal = 1.f / Sqrt(l_OriginDifference.GetNorm2());
        o_Report.m_Normal.x = l_OriginDifference.x * l_InvNormal;
        o_Report.m_Normal.y = l_OriginDifference.y * l_InvNormal;
        o_Report.m_Normal.z = l_OriginDifference.z * l_InvNormal;
        o_Report.m_Normal.w = 1.f;
        return TRUE;
    }
    Float l_Distance = l_Projection - Sqrt(l_Radius2 - l_Orthogonal2);
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Capsule.Origin + l_Distance * i_Capsule.Direction;
    Vec3f l_Normal = l_Intersection - Vec3f(i_Vertex);
    Float l_InvNormal = 1.f / Sqrt(l_Normal.GetNorm2());
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    o_Report.m_Normal.x = l_Normal.x * l_InvNormal;
    o_Report.m_Normal.y = l_Normal.y * l_InvNormal;
    o_Report.m_Normal.z = l_Normal.z * l_InvNormal;
    o_Report.m_Normal.w = 1.f;
    return TRUE;
}

Bool MovingSphereVsFaceInfinyUp(const Capsule_Z& i_Capsule, Vec4f& io_V0, Vec4f& io_V1, const Vec4f& i_Up, CollisionReport_Z& o_Report) {
    Vec3f l_Edge(io_V1.x - io_V0.x, io_V1.y - io_V0.y, io_V1.z - io_V0.z);
    Vec3f l_Up(i_Up);
    Vec3f l_Normal = l_Edge ^ l_Up;
    Float l_Norm = Sqrt(l_Normal.GetNorm2());
    if (l_Norm <= Float_Eps) {
        return FALSE;
    }
    l_Normal /= l_Norm;
    Float l_PlaneConstant = -(l_Normal.x * io_V0.x + l_Normal.y * io_V0.y + l_Normal.z * io_V0.z);
    if (i_Capsule.Origin * l_Normal + l_PlaneConstant < 0.f) {
        l_Normal = -l_Normal;
        l_PlaneConstant = -l_PlaneConstant;
        Vec4f l_Temp = io_V0;
        io_V0 = io_V1;
        io_V1 = l_Temp;
        l_Edge = Vec3f(io_V1.x - io_V0.x, io_V1.y - io_V0.y, io_V1.z - io_V0.z);
    }
    Float l_Exit = i_Capsule.Direction * l_Normal;
    if (l_Exit > 0.f) {
        return FALSE;
    }
    Float l_PlaneDistance = i_Capsule.Origin * l_Normal + l_PlaneConstant;
    Vec3f l_FromV0(i_Capsule.Origin.x - io_V0.x, i_Capsule.Origin.y - io_V0.y, i_Capsule.Origin.z - io_V0.z);
    if (l_PlaneDistance <= i_Capsule.Radius) {
        Vec3f l_FromV1(i_Capsule.Origin.x - io_V1.x, i_Capsule.Origin.y - io_V1.y, i_Capsule.Origin.z - io_V1.z);
        if (CollisionCrossDot(l_Edge, l_FromV0, l_Normal) < 0.f) {
            return FALSE;
        }
        if (CollisionCrossDot(l_FromV0, l_Up, l_Normal) < 0.f) {
            return FALSE;
        }
        if (CollisionCrossDot(l_Up, l_FromV1, l_Normal) < 0.f) {
            return FALSE;
        }
        o_Report.m_CollisionDistance = 0.f;
        o_Report.m_Intersection = i_Capsule.Origin;
        o_Report.m_Normal = l_Normal;
        return TRUE;
    }
    Float l_Distance = -((l_PlaneDistance - i_Capsule.Radius) / l_Exit);
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Capsule.Length) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Capsule.Origin + l_Distance * i_Capsule.Direction;
    l_FromV0 = Vec3f(l_Intersection.x - io_V0.x, l_Intersection.y - io_V0.y, l_Intersection.z - io_V0.z);
    Vec3f l_FromV1(l_Intersection.x - io_V1.x, l_Intersection.y - io_V1.y, l_Intersection.z - io_V1.z);
    if (CollisionCrossDot(l_Edge, l_FromV0, l_Normal) < 0.f) {
        return FALSE;
    }
    if (CollisionCrossDot(l_FromV0, l_Up, l_Normal) < 0.f) {
        return FALSE;
    }
    if (CollisionCrossDot(l_Up, l_FromV1, l_Normal) < 0.f) {
        return FALSE;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    o_Report.m_Normal = l_Normal;
    return TRUE;
}

Bool SegmentVsInfinySeg(const Segment_Z& i_Segment, const Vec4f& i_V0, const Vec4f& i_V1, CollisionReport_Z& o_Report) {
    Vec3f l_Edge(i_V1.x - i_V0.x, i_V1.y - i_V0.y, i_V1.z - i_V0.z);
    Vec3f l_Up(0.f, 1.f, 0.f);
    Vec3f l_Normal = l_Edge ^ l_Up;
    Float l_PlaneConstant = -(l_Normal.x * i_V0.x + l_Normal.y * i_V0.y + l_Normal.z * i_V0.z);
    Float l_PlaneDistance = l_PlaneConstant + l_Normal * i_Segment.Origin;
    Float l_Distance = -(l_PlaneDistance / (l_Normal * i_Segment.Direction));
    if (l_Distance > o_Report.m_CollisionDistance || l_Distance < 0.f || l_Distance > i_Segment.Length) {
        return FALSE;
    }
    Vec3f l_Intersection = i_Segment.Origin + l_Distance * i_Segment.Direction;
    Vec3f l_FromV0(l_Intersection.x - i_V0.x, l_Intersection.y - i_V0.y, l_Intersection.z - i_V0.z);
    Vec3f l_FromV1(l_Intersection.x - i_V1.x, l_Intersection.y - i_V1.y, l_Intersection.z - i_V1.z);
    if (CollisionCrossDot(l_Edge, l_FromV0, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionCrossDot(l_FromV0, l_Up, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (CollisionCrossDot(l_Up, l_FromV1, l_Normal) < -1.e-4f) {
        return FALSE;
    }
    if (l_PlaneDistance < 0.f) {
        l_Normal = -l_Normal;
    }
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Intersection = l_Intersection;
    o_Report.m_Normal = l_Normal;
    return TRUE;
}

Bool PointVsBox(const Vec3f& i_Point, const Box_Z& i_Box) {
    Float l_X = i_Point.x - i_Box.Mat.m.m[0][3];
    Float l_Y = i_Point.y - i_Box.Mat.m.m[1][3];
    Float l_Z = i_Point.z - i_Box.Mat.m.m[2][3];
    Float l_LocalX = i_Box.Mat.m.m[0][2] * l_Z + i_Box.Mat.m.m[0][0] * l_X + i_Box.Mat.m.m[0][1] * l_Y;
    Float l_LocalY = i_Box.Mat.m.m[1][2] * l_Z + i_Box.Mat.m.m[1][0] * l_X + i_Box.Mat.m.m[1][1] * l_Y;
    Float l_LocalZ = i_Box.Mat.m.m[2][2] * l_Z + i_Box.Mat.m.m[2][0] * l_X + i_Box.Mat.m.m[2][1] * l_Y;
    if (l_LocalX < -i_Box.Scale.x) {
        return FALSE;
    }
    if (l_LocalX > i_Box.Scale.x) {
        return FALSE;
    }
    if (l_LocalY < -i_Box.Scale.y) {
        return FALSE;
    }
    if (l_LocalY > i_Box.Scale.y) {
        return FALSE;
    }
    if (l_LocalZ < -i_Box.Scale.z) {
        return FALSE;
    }
    if (l_LocalZ > i_Box.Scale.z) {
        return FALSE;
    }
    return TRUE;
}

Bool SphereVsBox(const Sphere_Z& i_Sphere, const Box_Z& i_Box, CollisionReport_Z& o_Report) {
    Float l_LocalCenter[3];
    Float l_Delta[4];
    Float l_X = i_Sphere.Center.x - i_Box.Mat.m.m[0][3];
    Float l_Y = i_Sphere.Center.y - i_Box.Mat.m.m[1][3];
    Float l_Z = i_Sphere.Center.z - i_Box.Mat.m.m[2][3];
    l_LocalCenter[0] = i_Box.Mat.m.m[0][2] * l_Z + i_Box.Mat.m.m[0][0] * l_X + i_Box.Mat.m.m[0][1] * l_Y;
    l_LocalCenter[1] = i_Box.Mat.m.m[1][2] * l_Z + i_Box.Mat.m.m[1][0] * l_X + i_Box.Mat.m.m[1][1] * l_Y;
    l_LocalCenter[2] = i_Box.Mat.m.m[2][2] * l_Z + i_Box.Mat.m.m[2][0] * l_X + i_Box.Mat.m.m[2][1] * l_Y;
    Float l_Distance2 = 0.f;
    for (int i = 0; i < 3; ++i) {
        if (l_LocalCenter[i] < -i_Box.Scale[i]) {
            l_Delta[i] = l_LocalCenter[i] + i_Box.Scale[i];
            l_Distance2 += l_Delta[i] * l_Delta[i];
        }
        else if (l_LocalCenter[i] > i_Box.Scale[i]) {
            l_Delta[i] = l_LocalCenter[i] - i_Box.Scale[i];
            l_Distance2 += l_Delta[i] * l_Delta[i];
        }
        else {
            l_Delta[i] = 0.f;
        }
        if (l_Distance2 > i_Sphere.Radius * i_Sphere.Radius) {
            return FALSE;
        }
    }
    if (l_Distance2 <= 1.e-5f) {
        Float l_LocalNormal[3] = { 0.f, 0.f, 0.f };
        Float l_MinDistance = 1000000.f;
        Float* l_NormalAxis = l_LocalNormal;
        for (int i = 0; i < 3; ++i) {
            Float l_FaceDistance;
            Float l_Sign;
            if (l_LocalCenter[i] <= 0.f) {
                l_FaceDistance = i_Box.Scale[i] + l_LocalCenter[i];
                l_Sign = -1.f;
            }
            else {
                l_FaceDistance = i_Box.Scale[i] - l_LocalCenter[i];
                l_Sign = 1.f;
            }
            if (l_FaceDistance < l_MinDistance) {
                l_MinDistance = l_FaceDistance;
                l_NormalAxis = l_LocalNormal + i;
                *l_NormalAxis = l_Sign;
            }
        }
        Float l_Distance = l_MinDistance + i_Sphere.Radius;
        if (l_Distance > o_Report.m_CollisionDistance) {
            return FALSE;
        }
        o_Report.m_Normal.x = i_Box.Mat.m.m[2][0] * l_LocalNormal[2] + i_Box.Mat.m.m[0][0] * l_LocalNormal[0] + i_Box.Mat.m.m[1][0] * l_LocalNormal[1];
        o_Report.m_Normal.y = i_Box.Mat.m.m[2][1] * l_LocalNormal[2] + i_Box.Mat.m.m[0][1] * l_LocalNormal[0] + i_Box.Mat.m.m[1][1] * l_LocalNormal[1];
        o_Report.m_Normal.z = i_Box.Mat.m.m[2][2] * l_LocalNormal[2] + i_Box.Mat.m.m[0][2] * l_LocalNormal[0] + i_Box.Mat.m.m[1][2] * l_LocalNormal[1];
        o_Report.m_Normal.w = 1.f;
        Float l_IntersectionDistance = -0.5f * (i_Sphere.Radius + l_Distance);
        o_Report.m_Intersection.x = i_Sphere.Center.x + o_Report.m_Normal.x * l_IntersectionDistance;
        o_Report.m_Intersection.y = i_Sphere.Center.y + o_Report.m_Normal.y * l_IntersectionDistance;
        o_Report.m_Intersection.z = i_Sphere.Center.z + o_Report.m_Normal.z * l_IntersectionDistance;
        o_Report.m_Intersection.w = 1.f;
        o_Report.m_CollisionDistance = l_Distance;
        return TRUE;
    }
    Float l_Distance = Sqrt(l_Distance2);
    if (l_Distance > o_Report.m_CollisionDistance) {
        return FALSE;
    }
    Float l_InvDistance = 1.f / l_Distance;
    o_Report.m_CollisionDistance = l_Distance;
    o_Report.m_Normal.x = (i_Box.Mat.m.m[2][0] * l_Delta[2] + i_Box.Mat.m.m[0][0] * l_Delta[0] + i_Box.Mat.m.m[1][0] * l_Delta[1]) * l_InvDistance;
    o_Report.m_Normal.y = (i_Box.Mat.m.m[2][1] * l_Delta[2] + i_Box.Mat.m.m[0][1] * l_Delta[0] + i_Box.Mat.m.m[1][1] * l_Delta[1]) * l_InvDistance;
    o_Report.m_Normal.z = (i_Box.Mat.m.m[2][2] * l_Delta[2] + i_Box.Mat.m.m[0][2] * l_Delta[0] + i_Box.Mat.m.m[1][2] * l_Delta[1]) * l_InvDistance;
    o_Report.m_Normal.w = 1.f;
    o_Report.m_Intersection.x = i_Sphere.Center.x - o_Report.m_Normal.x * l_Distance;
    o_Report.m_Intersection.y = i_Sphere.Center.y - o_Report.m_Normal.y * l_Distance;
    o_Report.m_Intersection.z = i_Sphere.Center.z - o_Report.m_Normal.z * l_Distance;
    o_Report.m_Intersection.w = 1.f;
    return TRUE;
}

static int intersectRectQuad(Float* i_Rectangle, Float* i_Quad, Float* o_Intersection) {
    int l_QuadPointCount = 4;
    int l_IntersectionPointCount;
    Float l_Buffer[16];
    Float* l_Quad = i_Quad;
    Float* l_Intersection = o_Intersection;
    for (int l_Direction = 0; l_Direction <= 1; ++l_Direction) {
        for (Float l_Sign = -1; l_Sign <= 1; l_Sign += 2) {
            Float* l_QuadPoint = l_Quad;
            Float* l_IntersectionPoint = l_Intersection;
            l_IntersectionPointCount = 0;
            for (int i = l_QuadPointCount; i > 0; --i) {
                if (l_Sign * l_QuadPoint[l_Direction] < i_Rectangle[l_Direction]) {
                    l_IntersectionPoint[0] = l_QuadPoint[0];
                    l_IntersectionPoint[1] = l_QuadPoint[1];
                    l_IntersectionPoint += 2;
                    ++l_IntersectionPointCount;
                    if (l_IntersectionPointCount & 8) {
                        l_Quad = l_Intersection;
                        goto done;
                    }
                }
                Float* l_NextQuadPoint = i > 1 ? l_QuadPoint + 2 : l_Quad;
                if ((l_Sign * l_QuadPoint[l_Direction] < i_Rectangle[l_Direction]) ^ (l_Sign * l_NextQuadPoint[l_Direction] < i_Rectangle[l_Direction])) {
                    l_IntersectionPoint[1 - l_Direction] = (l_Sign * i_Rectangle[l_Direction] - l_QuadPoint[l_Direction]) * ((l_NextQuadPoint[1 - l_Direction] - l_QuadPoint[1 - l_Direction]) / (l_NextQuadPoint[l_Direction] - l_QuadPoint[l_Direction])) + l_QuadPoint[1 - l_Direction];
                    l_IntersectionPoint[l_Direction] = l_Sign * i_Rectangle[l_Direction];
                    l_IntersectionPoint += 2;
                    ++l_IntersectionPointCount;
                    if (l_IntersectionPointCount & 8) {
                        l_Quad = l_Intersection;
                        goto done;
                    }
                }
                l_QuadPoint += 2;
            }
            l_Quad = l_Intersection;
            l_Intersection = l_Quad == o_Intersection ? l_Buffer : o_Intersection;
            l_QuadPointCount = l_IntersectionPointCount;
        }
    }
done:
    if (l_Quad != o_Intersection) {
        for (int i = 0; i < l_IntersectionPointCount; ++i) {
            o_Intersection[0] = l_Quad[0];
            o_Intersection[1] = l_Quad[1];
            o_Intersection += 2;
            l_Quad += 2;
        }
    }
    return l_IntersectionPointCount;
}

void cullPoints(int i_PointCount, Float* i_Points, int i_MaxPoints, int i_FirstPoint, int* o_Indices) {
    int i;
    int j;
    Float l_CenterX;
    Float l_CenterY;
    if (i_PointCount == 1) {
        l_CenterX = i_Points[0];
        l_CenterY = i_Points[1];
    }
    else if (i_PointCount == 2) {
        l_CenterX = 0.5f * (i_Points[0] + i_Points[2]);
        l_CenterY = 0.5f * (i_Points[1] + i_Points[3]);
    }
    else {
        Float l_Area = 0.f;
        Float l_CentroidX = 0.f;
        Float l_CentroidY = 0.f;
        for (i = 0; i < i_PointCount - 1; ++i) {
            Float l_Determinant = i_Points[i * 2] * i_Points[i * 2 + 3] - i_Points[i * 2 + 2] * i_Points[i * 2 + 1];
            l_Area += l_Determinant;
            l_CentroidX += l_Determinant * (i_Points[i * 2] + i_Points[i * 2 + 2]);
            l_CentroidY += l_Determinant * (i_Points[i * 2 + 1] + i_Points[i * 2 + 3]);
        }
        Float l_Determinant = i_Points[i_PointCount * 2 - 2] * i_Points[1] - i_Points[0] * i_Points[i_PointCount * 2 - 1];
        Float l_Factor = 1.f / (3.f * (l_Area + l_Determinant));
        l_CenterX = l_Factor * (l_CentroidX + l_Determinant * (i_Points[i_PointCount * 2 - 2] + i_Points[0]));
        l_CenterY = l_Factor * (l_CentroidY + l_Determinant * (i_Points[i_PointCount * 2 - 1] + i_Points[1]));
    }

    Float l_Angles[8];
    for (i = 0; i < i_PointCount; ++i) {
        l_Angles[i] = O_Atan2(i_Points[i * 2 + 1] - l_CenterY, i_Points[i * 2] - l_CenterX);
    }

    int l_Available[8];
    for (i = 0; i < i_PointCount; ++i) {
        l_Available[i] = 1;
    }
    l_Available[i_FirstPoint] = 0;
    o_Indices[0] = i_FirstPoint;
    ++o_Indices;
    for (j = 1; j < i_MaxPoints; ++j) {
        Float l_Angle = Float(j) * (6.283185307179586 / Double(i_MaxPoints)) + l_Angles[i_FirstPoint];
        if (l_Angle > 3.141592653589793) {
            l_Angle -= 6.283185307179586;
        }
        Float l_MinDifference = 1000000000.f;
        for (i = 0; i < i_PointCount; ++i) {
            if (l_Available[i]) {
                Float l_Difference = Abs(l_Angles[i] - l_Angle);
                if (l_Difference > 3.141592653589793) {
                    l_Difference = 6.283185307179586 - l_Difference;
                }
                if (l_Difference < l_MinDifference) {
                    l_MinDifference = l_Difference;
                    *o_Indices = i;
                }
            }
        }
        l_Available[*o_Indices] = 0;
        ++o_Indices;
    }
}

void dLineClosestApproach(const Float* i_PointA, const Float* i_DirectionA, const Float* i_PointB, const Float* i_DirectionB, Float* o_Alpha, Float* o_Beta) {
    Float l_Delta[3];
    l_Delta[0] = i_PointB[0] - i_PointA[0];
    l_Delta[1] = i_PointB[1] - i_PointA[1];
    l_Delta[2] = i_PointB[2] - i_PointA[2];
    Float l_Dot = CollisionDot(i_DirectionA, i_DirectionB);
    Float l_A = CollisionDot(i_DirectionA, l_Delta);
    Float l_B = -CollisionDot(i_DirectionB, l_Delta);
    Float l_Denominator = 1 - l_Dot * l_Dot;
    if (l_Denominator <= 0.0001f) {
        *o_Alpha = 0.f;
        *o_Beta = 0.f;
    }
    else {
        l_Denominator = 1.f / l_Denominator;
        *o_Alpha = (l_A + l_Dot * l_B) * l_Denominator;
        *o_Beta = (l_Dot * l_A + l_B) * l_Denominator;
    }
}

Vec4f _vBestNormal;
static Float fBestDepth;
