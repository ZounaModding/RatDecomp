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

// TODO: Finish matching
void Camera_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
    Mat4x4 l_RollMatrix;
    Vec2f l_SinCos;

    memset(&l_RollMatrix, 0, sizeof(l_RollMatrix));

    l_RollMatrix.m[0][0] = 1.0f;
    l_RollMatrix.m[1][1] = 1.0f;
    l_RollMatrix.m[2][2] = 1.0f;
    l_RollMatrix.m[3][3] = 1.0f;

    O_SinCos(l_SinCos, m_Roll);

    l_RollMatrix.m[0][1] = l_SinCos.x;
    l_RollMatrix.m[0][0] = l_SinCos.y;
    l_RollMatrix.m[1][0] = -l_SinCos.x;
    l_RollMatrix.m[1][1] = l_SinCos.y;

    m_WorldPos = i_Node->GetWorldMatrix().GetMatrixTrans();

    Node_Z* l_TargetNode = m_NodeTargetHdl;

    if (l_TargetNode) {
        m_Target = l_TargetNode->GetWorldMatrix().GetMatrixTrans();
    }

    m_Direction = m_Target - m_WorldPos;

    if (!m_Direction.CNormalize()) {
        m_Direction = VEC3F_FRONT;
    }

    BuildLookAtMatrix(m_Direction, Vec3f(0.0f, 1.0f, 0.0f), m_WorldMatrix);

    m_WorldMatrix.m[3][0] = m_WorldPos.x;
    m_WorldMatrix.m[3][1] = m_WorldPos.y;
    m_WorldMatrix.m[3][2] = m_WorldPos.z;
    m_WorldMatrix.m[3][3] = 1.0f;

    m_WorldMatrix *= l_RollMatrix;

    UpdateInverseWorldMatrix(i_Node);

    Object_Z::UpdateObject(i_Node, i_Data);
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
    i_Node->SetWorldMatrixPtr();
    i_Node->GetWorldMatrix() = m_WorldMatrix;
    i_Node->EnableFlag(FL_NODE_INVALIDMAT);

    i_Node->GetRotInWorld() = Quat(m_WorldMatrix);
    i_Node->GetRotInWorld().GetMatrix(i_Node->GetRotInWorldMatrix());
    i_Node->EnableFlag(FL_NODE_INVALIDROT);

    Inverse2(m_WorldMatrix, GetInverseWorldMatrix());
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
    const Vec2f* l_Cur = m_Points;
    const Vec2f* l_Next = l_Cur + 1;

    o_IntersectionsX.Empty();

    S32 l_Nb = m_PointNb;

    for (S32 i = 0; i < l_Nb; i++, l_Cur++, l_Next++) {
        if (i == l_Nb - 1) {
            l_Next = m_Points;
        }

        if ((l_Cur->y < i_Z && l_Next->y >= i_Z) || (l_Cur->y >= i_Z && l_Next->y < i_Z)) {
            Float l_X = (i_Z - l_Cur->y) / (l_Next->y - l_Cur->y) * (l_Next->x - l_Cur->x) + l_Cur->x;

            if (!o_IntersectionsX.GetSize()) {
                o_IntersectionsX.Add(l_X);
            }
            else if (l_X <= o_IntersectionsX[0]) {
                o_IntersectionsX.Insert(0, l_X);
            }
            else if (l_X > o_IntersectionsX[o_IntersectionsX.GetSize() - 1]) {
                o_IntersectionsX.Add(l_X);
            }
            else {
                for (S32 j = 0; j < o_IntersectionsX.GetSize() - 1; j++) {
                    if (l_X > o_IntersectionsX[j] && l_X <= o_IntersectionsX[j + 1]) {
                        o_IntersectionsX.Insert(j + 1, l_X);
                        break;
                    }
                }
            }
        }
    }

    if (o_IntersectionsX.GetSize()) {
        if (o_IntersectionsX.GetSize() & 1) {
            ASSERTLE_Z(FALSE, "Nombre de point impair dans intersection occluder Y", 516, "FALSE");
        }

        return TRUE;
    }

    return FALSE;
}

Bool OccludedFrustum_Z::GetPtsOnLineX(Float i_X, FloatDA& o_IntersectionsZ) const {
    const Vec2f* l_Cur = m_Points;
    const Vec2f* l_Next = l_Cur + 1;

    o_IntersectionsZ.Empty();

    S32 l_Nb = m_PointNb;

    for (S32 i = 0; i < l_Nb; i++, l_Cur++, l_Next++) {
        if (i == l_Nb - 1) {
            l_Next = m_Points;
        }

        if ((l_Cur->x < i_X && l_Next->x >= i_X) || (l_Cur->x >= i_X && l_Next->x < i_X)) {
            Float l_Z = (i_X - l_Cur->x) / (l_Next->x - l_Cur->x) * (l_Next->y - l_Cur->y) + l_Cur->y;

            if (!o_IntersectionsZ.GetSize()) {
                o_IntersectionsZ.Add(l_Z);
            }
            else if (l_Z <= o_IntersectionsZ[0]) {
                o_IntersectionsZ.Insert(0, l_Z);
            }
            else if (l_Z > o_IntersectionsZ[o_IntersectionsZ.GetSize() - 1]) {
                o_IntersectionsZ.Add(l_Z);
            }
            else {
                for (S32 j = 0; j < o_IntersectionsZ.GetSize() - 1; j++) {
                    if (l_Z > o_IntersectionsZ[j] && l_Z <= o_IntersectionsZ[j + 1]) {
                        o_IntersectionsZ.Insert(j + 1, l_Z);
                        break;
                    }
                }
            }
        }
    }

    if (o_IntersectionsZ.GetSize()) {
        if (o_IntersectionsZ.GetSize() & 1) {
            ASSERTLE_Z(FALSE, "Nombre de point impair dans intersection occluder X", 570, "FALSE");
        }

        return TRUE;
    }

    return FALSE;
}
