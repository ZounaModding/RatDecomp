#include "SystemObject_Z.h"

Sphere_Z operator*(const Mat4x4& i_Mat, const Sphere_Z& i_Sphere) {
    Sphere_Z l_Sphere;
    Vec3f l_Scale(i_Mat.m[0][0], i_Mat.m[0][1], i_Mat.m[0][2]);
    l_Sphere.Center = i_Mat * i_Sphere.Center;
    l_Sphere.Radius = l_Scale.GetNorm() * i_Sphere.Radius;
    return l_Sphere;
}

Bool CullCone_Z::IsCulled(const Vec4f& i_Position) {
    if (m_Length == DEFAULT_NOCULLED) {
        return FALSE;
    }
    Vec3f l_Point;
    l_Point.x = i_Position.x - m_Origin.x;
    l_Point.y = i_Position.y - m_Origin.y;
    l_Point.z = i_Position.z - m_Origin.z;
    Float l_Value = l_Point * m_Direction;
    if (l_Value < m_Length) {
        return FALSE;
    }
    l_Value *= InvSqrt(l_Point.GetNorm2(), 1.f);
    return l_Value >= m_AngleCos;
}
