#include "Camera_Z.h"
#include "Occluder_Z.h"
#include "Program_Z.h"

void Camera_Z::UpdateObject(Node_Z* a1, ObjectDatas_Z* a2) {
}

void Camera_Z::SetOccludedFarClip(Float i_Far) {
    ASSERTLE_Z(i_Far > 0.0f, "Invalid Occluded Far Value", 146, "_Far>0.f");

    m_Frustum.m_OccludedFarClip = i_Far;
    m_Frustum.m_FrustumFarClip = m_Frustum.m_FarClip;
    m_Frustum.m_ViewPlanes.BuildPlane(m_WorldPos, m_Frustum.m_NearClip, i_Far);
    m_Frustum.m_SecondaryPlanes.BuildPlane(m_WorldPos, m_Frustum.m_NearClip, i_Far);
    return;
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

void Camera_Z::NoOcclusion() {
}

void Camera_Z::Load(void** i_Data) {
}

void Camera_Z::EndLoad() {
}
