#include "Camera_Z.h"
#include "Viewport_Z.h"

void FrustrumPlane_Z::BuildFrustrum(const Mat4x4& i_WorldMatrix, const Vec3f& i_CameraPosition, Float i_HRatio, Float i_VRatio, Float i_NearClip, Float i_FarClip) {
    Vec3f l_BottomLeft(-i_HRatio, -i_VRatio, 1.0f);
    Vec3f l_TopLeft(-i_HRatio, i_VRatio, 1.0f);
    Vec3f l_TopRight(i_HRatio, i_VRatio, 1.0f);
    Vec3f l_BottomRight(i_HRatio, -i_VRatio, 1.0f);

    m_PlaneNormals[0] = l_TopLeft ^ l_BottomLeft;
    // $SABE: Not sure about this shape, it should be Normalize() but we know that does not do a /=,
    //        it's fine as it is now. These might be something other than Vec4f, like a Plane struct...
    m_PlaneNormals[0].xyz() /= m_PlaneNormals[0].xyz().GetNorm();
    m_PlaneNormals[1] = l_BottomRight ^ l_TopRight;
    m_PlaneNormals[1].xyz() /= m_PlaneNormals[1].xyz().GetNorm();
    m_PlaneNormals[2] = l_BottomLeft ^ l_BottomRight;
    m_PlaneNormals[2].xyz() /= m_PlaneNormals[2].xyz().GetNorm();
    m_PlaneNormals[3] = l_TopRight ^ l_TopLeft;
    m_PlaneNormals[3].xyz() /= m_PlaneNormals[3].xyz().GetNorm();
    m_PlaneNormals[4] = VEC3F_FRONT;
    m_PlaneNormals[5] = VEC3F_BACK;

    m_PlaneX.Set(m_PlaneNormals[0].x, m_PlaneNormals[1].x, m_PlaneNormals[2].x, m_PlaneNormals[3].x);
    m_PlaneY.Set(m_PlaneNormals[0].y, m_PlaneNormals[1].y, m_PlaneNormals[2].y, m_PlaneNormals[3].y);
    m_PlaneZ.Set(m_PlaneNormals[0].z, m_PlaneNormals[1].z, m_PlaneNormals[2].z, m_PlaneNormals[3].z);

    for (S32 i = 0; i < 6; i++) {
        m_PlanesDir[i] = i_WorldMatrix.m3() * m_PlaneNormals[i];
    }

    BuildPlane(i_CameraPosition, i_NearClip, i_FarClip);
}

void FrustrumPlane_Z::BuildPlane(const Vec3f& i_CameraPosition, Float i_NearClip, Float i_FarClip) {
    for (S32 i = 0; i < 4; i++) {
        m_PlaneOffsets[i] = i_CameraPosition * m_PlanesDir[i].xyz();
    }
    m_PlaneOffsets[4] = i_CameraPosition * m_PlanesDir[4].xyz() + i_NearClip;
    m_PlaneOffsets[5] = i_CameraPosition * m_PlanesDir[5].xyz() - i_FarClip;

    m_PlanesLRB[0].Set(m_PlanesDir[0].x, m_PlanesDir[1].x, m_PlanesDir[2].x, 1.0f);
    m_PlanesLRB[1].Set(m_PlanesDir[0].y, m_PlanesDir[1].y, m_PlanesDir[2].y, 1.0f);
    m_PlanesLRB[2].Set(m_PlanesDir[0].z, m_PlanesDir[1].z, m_PlanesDir[2].z, 1.0f);
    m_PlanesTNF[0].Set(m_PlanesDir[3].x, m_PlanesDir[4].x, m_PlanesDir[5].x, 1.0f);
    m_PlanesTNF[1].Set(m_PlanesDir[3].y, m_PlanesDir[4].y, m_PlanesDir[5].y, 1.0f);
    m_PlanesTNF[2].Set(m_PlanesDir[3].z, m_PlanesDir[4].z, m_PlanesDir[5].z, 1.0f);
    m_PlanesLRB[3].Set(m_PlaneOffsets[0], m_PlaneOffsets[1], m_PlaneOffsets[2], 1.0f);
    m_PlanesTNF[3].Set(m_PlaneOffsets[3], m_PlaneOffsets[4], m_PlaneOffsets[5], 1.0f);
}

// $SABE: This is a mess, finish matching
Bool BoxVsFrustrumAndFlags(const Frustrum_Z& i_Frustrum, const Mat4x4& i_Matrix, const Box_Z& i_Box, U32& o_Flag, Float i_Scale) {
    o_Flag = 0;

    Box_Z l_Box;
    l_Box.MatMulHasScale(i_Box, i_Matrix, i_Scale, 1.0f / i_Scale);

    for (S32 i = 0; i < 4; i++) {
        const Vec4f& l_Plane = i_Frustrum.m_ViewPlanes.m_PlaneNormals[i];
        Vec4f l_BoxPlane(
            l_Box.Mat.m.m[0][0] * l_Plane.x + l_Box.Mat.m.m[0][1] * l_Plane.y + l_Box.Mat.m.m[0][2] * l_Plane.z,
            l_Box.Mat.m.m[1][0] * l_Plane.x + l_Box.Mat.m.m[1][1] * l_Plane.y + l_Box.Mat.m.m[1][2] * l_Plane.z,
            l_Box.Mat.m.m[2][0] * l_Plane.x + l_Box.Mat.m.m[2][1] * l_Plane.y + l_Box.Mat.m.m[2][2] * l_Plane.z,
            0.0f
        );
        Vec4f l_NegativeBoxPlane = l_BoxPlane * -1.0f;
        Vec4f l_MaxPlane(
            Max(l_NegativeBoxPlane.x, l_BoxPlane.x),
            Max(l_NegativeBoxPlane.y, l_BoxPlane.y),
            Max(l_NegativeBoxPlane.z, l_BoxPlane.z),
            Max(l_NegativeBoxPlane.w, l_BoxPlane.w)
        );
        Float l_Radius = l_MaxPlane.xyz() * l_Box.Scale;
        Float l_Distance = l_Box.Mat.m.m[0][3] * l_Plane.x + l_Box.Mat.m.m[1][3] * l_Plane.y + l_Box.Mat.m.m[2][3] * l_Plane.z;
        if (l_Distance < -l_Radius) {
            return FALSE;
        }
        if (l_Distance < l_Radius) {
            o_Flag |= FL_CLIPPLANE_SIDES;
        }
    }

    Float l_AbsFrontX = l_Box.Mat.m.m[0][2] > 0.0f ? l_Box.Mat.m.m[0][2] : -l_Box.Mat.m.m[0][2];
    Float l_AbsFrontY = l_Box.Mat.m.m[1][2] > 0.0f ? l_Box.Mat.m.m[1][2] : -l_Box.Mat.m.m[1][2];
    Float l_AbsFrontZ = l_Box.Mat.m.m[2][2] > 0.0f ? l_Box.Mat.m.m[2][2] : -l_Box.Mat.m.m[2][2];
    Float l_Radius = l_Box.Scale.x * l_AbsFrontX + l_Box.Scale.y * l_AbsFrontY + l_Box.Scale.z * l_AbsFrontZ;
    Float l_BoxZ = l_Box.Mat.m.m[2][3];
    if (l_BoxZ < -l_Radius) {
        return FALSE;
    }
    if (l_BoxZ < l_Radius) {
        o_Flag |= FL_CLIPPLANE_NEAR;
    }
    if (l_BoxZ > i_Frustrum.m_FarClip + l_Radius) {
        return FALSE;
    }

    Vec3f l_FarCenter(-l_Box.Mat.m.m[0][3], -l_Box.Mat.m.m[1][3], i_Frustrum.m_FarClip - l_BoxZ);
    const Vec4f* l_Axis = (const Vec4f*)&l_Box.Mat;
    const Float* l_BoxScale = &l_Box.Scale.x;
    S32 l_AxisCount = 3;
    while (l_AxisCount != 0) {
        Float l_AxisX = l_Axis->x;
        Float l_AxisY = l_Axis->y;
        Float l_AxisZ = l_Axis->z;
        Float l_Distance = l_AxisZ * l_FarCenter.z + l_AxisX * l_FarCenter.x + l_AxisY * l_FarCenter.y;
        S32 l_Sign;
        if (l_Distance == 0.0f) {
            l_Sign = 0;
        }
        else if (l_Distance >= 0.0f) {
            l_Sign = 1;
        }
        else {
            l_Sign = -1;
        }
        l_Distance = l_Distance > 0.0f ? l_Distance : -l_Distance;
        Float l_AbsAxisX = l_AxisX > 0.0f ? l_AxisX : -l_AxisX;
        Float l_AbsAxisY = l_AxisY > 0.0f ? l_AxisY : -l_AxisY;
        Float l_FarRadius = i_Frustrum.m_FarPlaneData[0] * l_AbsAxisX + i_Frustrum.m_FarPlaneData[1] * l_AbsAxisY;
        Float l_FrontRadius = i_Frustrum.m_FarPlaneData[2] * Max(0.0f, (Float)l_Sign * l_AxisZ);
        Float l_TotalRadius = Max(l_FarRadius, l_FrontRadius);
        if (l_Distance > l_TotalRadius + *l_BoxScale) {
            return FALSE;
        }
        l_AxisCount--;
        l_Axis++;
        l_BoxScale++;
    }

    return TRUE;
}
