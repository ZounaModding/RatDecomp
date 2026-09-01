#include "Camera_Z.h"
#include "Node_Z.h"
#include "Occluder_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"

Camera_Z::Camera_Z()
    : m_Target(0.0f, 0.0f, 3.0f) {
    m_OccludedFrustum.m_PointNb = 0;
    m_OccludedFrustum.m_IsOccluded = FALSE;

    SetGeometryType(CAMERA_Z);
    m_BSphereLocal.Set(VEC3F_NULL, 1.0f);
    m_Roll = 0.0f;
    SetFov(DegToRad(50.0f));
    m_FovEdition = DegToRad(40.0f);
    SetNearClip(0.1f);
    SetFarClip(60.0f);

    EnableFlag(FL_OBJECT_NO_DISPLAY | FL_OBJECT_NO_SEADCOLLIDE);
    EnableFlag(FL_OBJECT_NO_SEADDISPLAY);

    m_WorldMatrix.SetIdentity();

    m_OccludedZonesBA.SetSize(2);
    m_OccludedZonesBA.ClearBit(0);
    m_OccludedZonesBA.SetBit(1);

    m_UnkFloat_0x10d0 = 0.18f;
    m_UnkFloat_0x10d4 = 1.0f;
    m_UnkFloat_0x10d8 = 0.1f;
    m_UnkFloat_0x10dc = 100.0f;
}

void Camera_Z::SetNearClip(Float i_Near) {
    ASSERTLE_Z(i_Near > 0.0f, "Invalid Near Value", 51, "_Near>0.f");
    m_Frustum.m_NearClip = i_Near;
}

void Camera_Z::SetFov(Float i_RadFov) {
    ASSERTLE_Z(i_RadFov > 0.0f, "Invalid Fov Value", 57, "_RadFov>0.f");
    ASSERTLE_Z(i_RadFov < DegToRad(120.0f), "Overflow 120 Degree Fov Value", 58, "_RadFov<DegToRad(120)");
    m_Fov = i_RadFov;
}

void Camera_Z::SetFarClip(Float i_Far) {
    ASSERTLE_Z(i_Far > 0.0f, "Invalid Far Value", 63, "_Far>0.f");
    m_Frustum.m_FarClip = i_Far;
    m_Frustum.m_OccludedFarClip = i_Far;
    m_Frustum.m_FarPlaneData[2] = m_Frustum.m_FarClip;
}

void Camera_Z::SetTarget(const Vec3f& i_Target) {
    m_Target = i_Target;
}

void Camera_Z::UpdateObject(Node_Z* a1, ObjectDatas_Z* a2) {
}

void Camera_Z::SetOccludedFarClip(Float i_Far) {
    ASSERTLE_Z(i_Far > 0.0f, "Invalid Occluded Far Value", 146, "_Far>0.f");

    m_Frustum.m_OccludedFarClip = i_Far;
    m_Frustum.m_FarPlaneData[2] = m_Frustum.m_FarClip;
    m_Frustum.m_ViewPlanes.BuildPlane(m_WorldPos, m_Frustum.m_NearClip, i_Far);
    m_Frustum.m_SecondaryPlanes.BuildPlane(m_WorldPos, m_Frustum.m_NearClip, i_Far);
    return;
}

void Camera_Z::GetFrustrum2D(Frustrum2D_Z& o_Frustrum) const {
}

void Camera_Z::UpdateInverseWorldMatrix(Node_Z* i_Node) {
}

void Camera_Z::DoOcclusion(const Occluder_ZHdl& i_OccluderHdl) {
    Occluder_Z* l_Occluder = i_OccluderHdl;
    m_OccludedZonesBA.SetSize(l_Occluder->GetNbZone() + 2);
    m_OccluderHdl = HANDLE_NULL;
    m_OccludedFrustum.m_IsOccluded = FALSE;
    if (gData.IsEngineFlag(FL_OCCLUDER_ON)) {
        Float l_OccludedFarClip = l_Occluder->Occlusion(GetHandle(), m_OccludedFrustum, m_OccludedZonesBA);
        m_OccludedZonesBA.SetBit(1);
        if (l_OccludedFarClip > 0.0f) {
            if (!(l_OccludedFarClip > m_Frustum.m_FarClip)) {
                SetOccludedFarClip(l_OccludedFarClip);
            }
            Vec2f l_Min = Vec2f(m_OccludedFrustum.m_Points[0].x, m_OccludedFrustum.m_Points[0].y);
            Vec2f l_Max = Vec2f(l_Min.x, l_Min.y);
            for (S32 i = 1; i < m_OccludedFrustum.m_PointNb; i++) {
                l_Min.x = Min(l_Min.x, m_OccludedFrustum.m_Points[i].x);
                l_Min.y = Min(l_Min.y, m_OccludedFrustum.m_Points[i].y);
                l_Max.x = Max(l_Max.x, m_OccludedFrustum.m_Points[i].x);
                l_Max.y = Max(l_Max.y, m_OccludedFrustum.m_Points[i].y);
            }
            m_Frustum.m_TopViewBoundsMin = l_Min;
            m_Frustum.m_TopViewBoundsMax = l_Max;
            m_OccluderHdl = i_OccluderHdl;
        }
    }
}

void Camera_Z::GetOccluded(DrawInfo_Z& io_DrawInfo) {
    io_DrawInfo.m_IsOccluded = m_OccludedFrustum.m_IsOccluded;
    io_DrawInfo.m_Occluder = m_OccluderHdl;
    if (m_OccluderHdl) {
        io_DrawInfo.m_OccludedZonesBA = &m_OccludedZonesBA;
    }
    else {
        io_DrawInfo.m_OccludedZonesBA = NULL;
    }
}

void Camera_Z::NoOcclusion() {
}

void Camera_Z::Load(void** i_Data) {
}

void Camera_Z::EndLoad() {
}

// Should be called GetPtsOnLineZ
Bool OccludedFrustum_Z::GetPtsOnLineY(Float i_Z, FloatDA& o_IntersectionsX) const {
    return FALSE;
}

Bool OccludedFrustum_Z::GetPtsOnLineX(Float i_X, FloatDA& o_IntersectionsZ) const {
    return FALSE;
}
