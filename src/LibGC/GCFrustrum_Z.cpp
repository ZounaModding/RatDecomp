#include "Node_Z.h"
#include "Renderer_Z.h"

inline Bool WSphereVsFrustrum(const Frustrum_Z& i_Frustrum, const Sphere_Z& i_Sphere, U32& o_Flag) {
    o_Flag = 0;
    const Vec4f* l_PlaneDirection = i_Frustrum.m_ViewPlanes.m_PlanesDir;
    const Float* l_PlaneOffset = i_Frustrum.m_ViewPlanes.m_PlaneOffsets;
    for (S32 i = 0; i < 6; i++, l_PlaneDirection++, l_PlaneOffset++) {
        Float l_Distance = l_PlaneDirection->x * i_Sphere.Center.x
                           + l_PlaneDirection->y * i_Sphere.Center.y
                           + l_PlaneDirection->z * i_Sphere.Center.z
                           - *l_PlaneOffset;
        if (l_Distance < -i_Sphere.Radius) {
            return FALSE;
        }
        if (l_Distance < i_Sphere.Radius) {
            if (i == 4) {
                o_Flag |= FL_CLIPPLANE_NEAR;
            }
            else {
                o_Flag |= FL_CLIPPLANE_SIDES;
            }
        }
    }
    return TRUE;
}

Bool SphereVsFrustrum(const Frustrum_Z& i_Frustrum, DrawInfo_Z& io_DrawInfo, Node_Z* i_Node) {
    const Sphere_Z& l_Sphere = i_Node->GetBSphere();

    Bool l_Result = WSphereVsFrustrum(i_Frustrum, l_Sphere, io_DrawInfo.m_ClipSph.Flag);
    if (!l_Result) {
        return FALSE;
    }

    io_DrawInfo.m_Local2Cam = io_DrawInfo.m_World2Cam * i_Node->GetWorldMatrix();
    const Mat4x4& l_Matrix = io_DrawInfo.m_World2Cam;

    // $SABE: In reality this probably would be an operator call, but it's 100% now so fuck it
    io_DrawInfo.m_ClipSph.Sph.Center.x = l_Matrix.m[0][0] * l_Sphere.Center.x
                                         + l_Matrix.m[1][0] * l_Sphere.Center.y
                                         + l_Matrix.m[2][0] * l_Sphere.Center.z
                                         + l_Matrix.m[3][0];
    io_DrawInfo.m_ClipSph.Sph.Center.y = l_Matrix.m[0][1] * l_Sphere.Center.x
                                         + l_Matrix.m[1][1] * l_Sphere.Center.y
                                         + l_Matrix.m[2][1] * l_Sphere.Center.z
                                         + l_Matrix.m[3][1];
    io_DrawInfo.m_ClipSph.Sph.Center.z = l_Matrix.m[0][2] * l_Sphere.Center.x
                                         + l_Matrix.m[1][2] * l_Sphere.Center.y
                                         + l_Matrix.m[2][2] * l_Sphere.Center.z
                                         + l_Matrix.m[3][2];
    io_DrawInfo.m_ClipSph.Sph.Radius = l_Sphere.Radius;
    return TRUE;
}

Bool SphereVsFrustrum(
    const Frustrum_Z& i_Frustrum,
    const Mat4x4& i_Matrix,
    const Sphere_Z& i_Sphere,
    ClipSphere_Z& o_ClipSphere,
    Float i_Scale
) {
    Float l_CenterY = i_Sphere.Center.y;
    Float l_CenterX = i_Sphere.Center.x;
    Float l_CenterZ = i_Sphere.Center.z;
    Float l_NearDistance = i_Frustrum.m_NearClip;
    Float l_FarDistance = i_Frustrum.m_OccludedFarClip;

    o_ClipSphere.Sph.Center.z = i_Matrix.m[0][2] * l_CenterX + i_Matrix.m[1][2] * l_CenterY + i_Matrix.m[2][2] * l_CenterZ + i_Matrix.m[3][2];
    o_ClipSphere.Sph.Radius = i_Sphere.Radius * i_Scale;
    Float l_TransformedZ = o_ClipSphere.Sph.Center.z;
    Float l_TransformedRadius = o_ClipSphere.Sph.Radius;
    l_NearDistance -= l_TransformedZ;
    l_FarDistance -= l_TransformedZ;
    if (l_NearDistance > l_TransformedRadius || l_FarDistance < -l_TransformedRadius) {
        return FALSE;
    }

    o_ClipSphere.Sph.Center.x = i_Matrix.m[0][0] * l_CenterX + i_Matrix.m[1][0] * l_CenterY + i_Matrix.m[2][0] * l_CenterZ + i_Matrix.m[3][0];
    o_ClipSphere.Sph.Center.y = i_Matrix.m[0][1] * l_CenterX + i_Matrix.m[1][1] * l_CenterY + i_Matrix.m[2][1] * l_CenterZ + i_Matrix.m[3][1];

    Vec4f l_Distances;
    l_Distances = i_Frustrum.m_ViewPlanes.m_PlaneX * o_ClipSphere.Sph.Center.x;
    l_Distances += i_Frustrum.m_ViewPlanes.m_PlaneY * o_ClipSphere.Sph.Center.y;
    l_Distances += i_Frustrum.m_ViewPlanes.m_PlaneZ * o_ClipSphere.Sph.Center.z;
    Float l_LeftDistance = l_Distances.x;
    Float l_RightDistance = l_Distances.y;
    Float l_BottomDistance = l_Distances.z;
    Float l_TopDistance = l_Distances.w;
    Float l_NegativeRadius = -o_ClipSphere.Sph.Radius;
    if (l_LeftDistance < l_NegativeRadius || l_RightDistance < l_NegativeRadius || l_BottomDistance < l_NegativeRadius || l_TopDistance < l_NegativeRadius) {
        return FALSE;
    }

    o_ClipSphere.Flag = 0;
    l_TransformedRadius = o_ClipSphere.Sph.Radius;
    if (l_LeftDistance < l_TransformedRadius || l_RightDistance < l_TransformedRadius || l_BottomDistance < l_TransformedRadius || l_TopDistance < l_TransformedRadius) {
        o_ClipSphere.Flag |= FL_CLIPPLANE_SIDES;
    }
    if (l_NearDistance > -o_ClipSphere.Sph.Radius) {
        o_ClipSphere.Flag |= FL_CLIPPLANE_NEAR;
    }
    if (l_FarDistance < o_ClipSphere.Sph.Radius) {
        o_ClipSphere.Flag |= FL_CLIPPLANE_FAR;
    }
    return TRUE;
}

Bool SphereVsFrustrum(const Frustrum_Z& i_Frustrum, const Sphere_Z& i_Sphere, U32& o_Flag) {
    const FrustrumPlane_Z& l_Planes = i_Frustrum.m_ViewPlanes;
    Float l_CenterZ = i_Sphere.Center.z;
    Float l_Radius = i_Sphere.Radius;
    Float l_Near = i_Frustrum.m_NearClip - l_CenterZ;
    Float l_Far = i_Frustrum.m_OccludedFarClip - l_CenterZ;

    o_Flag = 0;

    if (l_Near > l_Radius || l_Far < -l_Radius) {
        return FALSE;
    }

    Vec4f l_ScaledX;
    Vec4f l_ScaledY;
    Vec4f l_ScaledZ;
    Vec4f l_Dots;
    Vec4_Scale(l_ScaledX, i_Sphere.Center.x, l_Planes.m_PlaneX);
    Vec4_Scale(l_ScaledY, i_Sphere.Center.y, l_Planes.m_PlaneY);
    Vec4_Add(l_Dots, l_ScaledX, l_ScaledY);
    Vec4_Scale(l_ScaledZ, l_CenterZ, l_Planes.m_PlaneZ);
    Vec4_Add(l_Dots, l_Dots, l_ScaledZ);

    Float l_D0 = l_Dots.x;
    Float l_D1 = l_Dots.y;
    Float l_D2 = l_Dots.z;
    Float l_D3 = l_Dots.w;

    if (l_D0 < -l_Radius || l_D1 < -l_Radius || l_D2 < -l_Radius || l_D3 < -l_Radius) {
        return FALSE;
    }

    if (l_D0 < l_Radius || l_D1 < l_Radius || l_D2 < l_Radius || l_D3 < l_Radius) {
        o_Flag |= FL_CLIPPLANE_SIDES;
    }

    if (l_Near > -i_Sphere.Radius) {
        o_Flag |= FL_CLIPPLANE_NEAR;
    }

    if (l_Far < i_Sphere.Radius) {
        o_Flag |= FL_CLIPPLANE_FAR;
    }

    return TRUE;
}
