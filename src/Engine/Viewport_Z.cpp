#include "Viewport_Z.h"
#include "World_Z.h"
#include "Manipulator_Z.h"
#include "Renderer_Z.h"
#include "Node_Z.h"
#include "Camera_Z.h"

Viewport_Z::Viewport_Z() {
    Init();
}

Viewport_Z::~Viewport_Z() { }

void Viewport_Z::Init() {
    m_Renderer = NULL;
    m_VCenter = -1.0f;
    m_HCenter = -1.0f;
    m_VSize = -1.0f;
    m_HSize = -1.0f;
    m_VRatio = -1.0f;
    m_HRatio = -1.0f;
    m_Ratio = -1.0f;
    m_DFov = -1.0f;
    m_CameraMatrixId = -1;
}

void Viewport_Z::SetPosAndSize(S32 i_StartX, S32 i_StartY, S32 i_SizeX, S32 i_SizeY) {
    m_StartX = i_StartX;
    m_StartY = i_StartY;
    m_SizeX = i_SizeX;
    m_SizeY = i_SizeY;
    m_HCenter = m_StartX + m_SizeX * 0.5f;
    m_VCenter = m_StartY + m_SizeY * 0.5f;
}

void Viewport_Z::GetPosAndSize(S32& o_StartX, S32& o_StartY, S32& o_SizeX, S32& o_SizeY) const {
    o_StartX = m_StartX;
    o_StartY = m_StartY;
    o_SizeX = m_SizeX;
    o_SizeY = m_SizeY;
}

void Viewport_Z::SetWorld(const World_ZHdl& i_WorldHdl) {
    m_WorldHdl = i_WorldHdl;
    m_OccluderHdl = HANDLE_NULL;
    if (m_WorldHdl.IsValid() && m_WorldHdl->GetNbOccluder() != 0) {
        m_OccluderHdl = m_WorldHdl->GetOccluder(0);
    }
}

// TODO: Finish matching
void Viewport_Z::UpdateFrustrum() {
    Node_Z* l_CameraNode = m_CameraNodeHdl;
    if (!l_CameraNode) {
        return;
    }

    Camera_Z* l_Camera = (Camera_Z*)l_CameraNode->GetObjectA();

    m_Tangent = O_Tan(l_Camera->GetFov() / 2.0f);
    m_InvDiagTangent = O_Tan(l_Camera->GetFovEdition() / 2.0f) / m_Tangent;
    m_Ratio = ((Float)m_SizeX / (Float)m_SizeY) / RATIO_SCREEN_STANDARD;
    m_DFov = m_SizeX / (m_Tangent * 2.0f);

    Float l_XBias = (m_Tangent * 4000.0f) / m_SizeX;
    Float l_XScale = gData.MainRdr->GetScreenRatio() * gData.MainRdr->GetXScaleFactor();

    if (m_SizeX > m_SizeY) {
        m_HRatio = l_XScale * m_Tangent * m_Ratio;
        m_VRatio = m_Tangent / RATIO_SCREEN_STANDARD;
        m_HSize = m_DFov / (m_Ratio * l_XScale);
        m_VSize = m_DFov / m_Ratio;
        l_XScale = l_XScale * l_XBias * m_Ratio;
    }
    else {
        m_HRatio = 2.0f * m_Tangent * m_Ratio;
        m_VRatio = m_Tangent / (RATIO_SCREEN_STANDARD / l_XScale);
        m_HSize = (m_DFov / 2.0f) / m_Ratio;
        m_VSize = (m_DFov / 2.0f) / (m_Ratio / l_XScale);
        l_XScale = 2.0f * l_XBias * m_Ratio;
    }

    Mat4x4& l_CameraMatrix = l_CameraNode->GetWorldMatrix();
    l_Camera->GetFrustrum().m_WorldTranslation = l_CameraMatrix.GetMatrixTrans();
    l_Camera->GetFrustrum().m_WorldMatrix = l_CameraMatrix;
    l_Camera->GetFrustrum().m_Direction = l_Camera->GetDir();
    Vec3f l_Right = l_Camera->GetFrustrum().m_Direction ^ VEC3F_UP;
    l_Right.CNormalize();
    l_Camera->GetFrustrum().m_Up = l_Right ^ l_Camera->GetFrustrum().m_Direction;
    l_Camera->GetFrustrum().m_Up.CNormalize();
    l_Camera->GetFrustrum().m_ViewPlanes.BuildFrustrum(
        l_CameraMatrix,
        l_Camera->GetFrustrum().m_WorldTranslation,
        m_HRatio,
        m_VRatio,
        l_Camera->GetFrustrum().m_NearClip,
        l_Camera->GetFrustrum().m_FarClip
    );
    l_Camera->GetFrustrum().m_SecondaryPlanes.BuildFrustrum(
        l_CameraMatrix,
        l_Camera->GetFrustrum().m_WorldTranslation,
        l_XScale,
        l_XBias / (RATIO_SCREEN_STANDARD / l_XScale),
        l_Camera->GetFrustrum().m_NearClip,
        l_Camera->GetFrustrum().m_FarClip
    );

    l_Camera->SetOccludedFarClip(l_Camera->GetFrustrum().m_FarClip);

    m_PlaneDirL = l_Camera->GetFrustrum().m_ViewPlanes.m_PlaneNormals[0].xyz();
    m_PlaneDirR = l_Camera->GetFrustrum().m_ViewPlanes.m_PlaneNormals[1].xyz();
    m_PlaneDirB = l_Camera->GetFrustrum().m_ViewPlanes.m_PlaneNormals[2].xyz();
    m_PlaneDirT = l_Camera->GetFrustrum().m_ViewPlanes.m_PlaneNormals[3].xyz();

    Vec3f l_Corner0(-m_HRatio, m_VRatio, 1.0f);
    Vec3f l_Corner1(-m_HRatio, -m_VRatio, 1.0f);
    Vec3f l_Corner2(m_HRatio, -m_VRatio, 1.0f);
    Vec3f l_Corner3(m_HRatio, m_VRatio, 1.0f);

    l_CameraMatrix.MulWithoutTrans(l_Corner0, l_Corner0);
    l_CameraMatrix.MulWithoutTrans(l_Corner1, l_Corner1);
    l_CameraMatrix.MulWithoutTrans(l_Corner2, l_Corner2);
    l_CameraMatrix.MulWithoutTrans(l_Corner3, l_Corner3);

    l_Camera->GetFrustrum().m_CornerPoints[0] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner0 * l_Camera->GetFrustrum().m_NearClip;
    l_Camera->GetFrustrum().m_CornerPoints[4] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner0 * l_Camera->GetFrustrum().m_FarClip;

    l_Camera->GetFrustrum().m_CornerPoints[1] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner1 * l_Camera->GetFrustrum().m_NearClip;
    l_Camera->GetFrustrum().m_CornerPoints[5] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner1 * l_Camera->GetFrustrum().m_FarClip;

    l_Camera->GetFrustrum().m_CornerPoints[2] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner2 * l_Camera->GetFrustrum().m_NearClip;
    l_Camera->GetFrustrum().m_CornerPoints[6] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner2 * l_Camera->GetFrustrum().m_FarClip;

    l_Camera->GetFrustrum().m_CornerPoints[3] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner3 * l_Camera->GetFrustrum().m_NearClip;
    l_Camera->GetFrustrum().m_CornerPoints[7] = l_Camera->GetFrustrum().m_WorldTranslation + l_Corner3 * l_Camera->GetFrustrum().m_FarClip;

    l_Camera->GetFrustrum().m_FarPlaneHalfWidth = ((l_Camera->GetFrustrum().m_CornerPoints[4] - l_Camera->GetFrustrum().m_CornerPoints[7]).GetNorm()) * 0.5f;
    l_Camera->GetFrustrum().m_FarPlaneHalfHeight = ((l_Camera->GetFrustrum().m_CornerPoints[5] - l_Camera->GetFrustrum().m_CornerPoints[6]).GetNorm()) * 0.5f;

    l_Camera->GetFrustrum().m_FrustumFarClip = l_Camera->GetFrustrum().m_FarClip;

    m_FrustumBoundsMin = l_Camera->GetFrustrum().m_CornerPoints[0];
    m_FrustumBoundsMax = m_FrustumBoundsMin;

    for (S32 i = 1; i < 8; ++i) {
        const Vec3f& l_Point = l_Camera->GetFrustrum().m_CornerPoints[i];

        m_FrustumBoundsMin.x = Min(l_Point.x, m_FrustumBoundsMin.x);
        m_FrustumBoundsMin.y = Min(l_Point.y, m_FrustumBoundsMin.y);
        m_FrustumBoundsMin.z = Min(l_Point.z, m_FrustumBoundsMin.z);

        m_FrustumBoundsMax.x = Max(l_Point.x, m_FrustumBoundsMax.x);
        m_FrustumBoundsMax.y = Max(l_Point.y, m_FrustumBoundsMax.y);
        m_FrustumBoundsMax.z = Max(l_Point.z, m_FrustumBoundsMax.z);
    }

    l_Camera->GetFrustrum().m_BoundsMin = m_FrustumBoundsMin;
    l_Camera->GetFrustrum().m_BoundsMax = m_FrustumBoundsMax;
    l_Camera->GetFrustrum().m_BoundsSphereCenter = (m_FrustumBoundsMin + m_FrustumBoundsMax) / 2.0f;

    l_Camera->GetFrustrum().m_BoundsSphereRadiusSq = (l_Camera->GetFrustrum().m_BoundsSphereCenter - l_Camera->GetFrustrum().m_CornerPoints[0]).GetNorm2();

    for (S32 i = 1; i < 8; ++i) {
        Float l_DistSq = (l_Camera->GetFrustrum().m_BoundsSphereCenter - l_Camera->GetFrustrum().m_CornerPoints[i]).GetNorm2();
        l_Camera->GetFrustrum().m_BoundsSphereRadiusSq = Max(l_Camera->GetFrustrum().m_BoundsSphereRadiusSq, l_DistSq);
    }

    l_Camera->GetFrustrum().m_TopViewBoundsMin.Set(m_FrustumBoundsMin.x, m_FrustumBoundsMin.z);
    l_Camera->GetFrustrum().m_TopViewBoundsMax.Set(m_FrustumBoundsMax.x, m_FrustumBoundsMax.z);
}

void Viewport_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    S32 i;

    i_DrawInfo.m_Vp = this;
    i_DrawInfo.m_VpStartX = m_StartX;
    i_DrawInfo.m_VpStartY = m_StartY;
    i_DrawInfo.m_VpSizeX = m_SizeX;
    i_DrawInfo.m_VpSizeY = m_SizeY;
    World_Z* l_World = m_WorldHdl;
    Node_Z* l_CameraNode = m_CameraNodeHdl;
    i_DrawInfo.m_Flag = DrawInfo_Z::FL_DRAWINFO_DO_POST_PROCESS;

    if (l_World && l_CameraNode) {
        Camera_Z* l_Camera = (Camera_Z*)l_CameraNode->GetObjectA();

        if (i_DrawInfo.m_LodPatchDist < 0.0f) {
            i_DrawInfo.m_LodPatchDist = l_Camera->GetFarClip();
        }
        i_DrawInfo.m_Camera = l_Camera;
        i_DrawInfo.m_CameraNode = l_CameraNode;
        i_DrawInfo.m_CameraTranslation = l_CameraNode->GetTranslation();
        i_DrawInfo.m_CameraDirection = l_Camera->GetDir();
        i_DrawInfo.m_World2Cam = l_Camera->GetInverseWorldMatrix();
        i_DrawInfo.m_VpTangent = GetTang();
        i_DrawInfo.m_VpInvDiagTangent = GetInvDTang();
        i_DrawInfo.m_VpDFov = GetDFov();
        i_DrawInfo.m_VpRatio = GetRatio();
        i_DrawInfo.m_VpHRatio = GetHRatio();
        i_DrawInfo.m_VpVRatio = GetVRatio();
        i_DrawInfo.m_FirstPlayerVpId = l_World->GetFirstVp();
        i_DrawInfo.m_VpCount = l_World->GetNbVp();

        i_DrawInfo.m_Vp->m_Renderer->SetViewMatrix(TRUE);
        S32 l_NbSceneDraw = l_World->GetNbManipulatorSceneDraw();
        for (i = 0; i < l_NbSceneDraw; i++) {
            ManipulatorSceneDraw_Z* l_ManipSceneDraw = l_World->GetManipulatorSceneDraw(i);
            if (l_ManipSceneDraw->IsActive()) {
                l_ManipSceneDraw->BeforeDraw(i_DrawInfo);
            }
        }

        l_World->Draw(i_DrawInfo);

        i_DrawInfo.m_Vp->m_Renderer->SetViewMatrix(FALSE);
        for (i = 0; i < l_NbSceneDraw; i++) {
            ManipulatorSceneDraw_Z* l_ManipSceneDraw = l_World->GetManipulatorSceneDraw(i);
            if (l_ManipSceneDraw->IsActive()) {
                l_ManipSceneDraw->Draw(i_DrawInfo);
            }
        }

        i_DrawInfo.m_Vp->m_Renderer->SetViewMatrix(TRUE);
        i_DrawInfo.m_Flag = DrawInfo_Z::FL_DRAWINFO_DO_POST_PROCESS;
    }

    m_Renderer->DrawTransparent(i_DrawInfo);
    m_Renderer->DrawPostRenderEffects(i_DrawInfo);
    i_DrawInfo.m_Flag = DrawInfo_Z::FL_DRAWINFO_NONE;
    i_DrawInfo.m_Vp->m_Renderer->SetViewMatrix(FALSE);

    S32 l_ManipDrawNb = m_ManipDrawHdls.GetSize();
    for (i = 0; i < l_ManipDrawNb; i++) {
        ManipulatorDraw_Z* l_ManipDraw = m_ManipDrawHdls[i];
        if (l_ManipDraw) {
            l_ManipDraw->Draw(i_DrawInfo);
            m_Renderer->DrawTransparent(i_DrawInfo);
        }
        if (l_ManipDrawNb > m_ManipDrawHdls.GetSize()) {
            break;
        }
    }
}

void Viewport_Z::RegisterManip(const ManipulatorDraw_ZHdl& i_ManipDrawHdl) {
    S32 l_Size = m_ManipDrawHdls.GetSize();
    S32 i = 0;
    while (i < l_Size) {
        if (i_ManipDrawHdl == m_ManipDrawHdls[i]) {
            return;
        }
        i++;
    }
    ActivableGroup_Z l_ManipGroup = i_ManipDrawHdl->GetGroup();
    i = 0;
    while (i < l_Size) {
        if (!m_ManipDrawHdls[i].IsValid()) {
            m_ManipDrawHdls.Remove(i--);
            l_Size--;
        }
        i++;
    }
    i = 0;
    while (i < l_Size) {
        if (l_ManipGroup <= m_ManipDrawHdls[i]->GetGroup()) {
            break;
        }
        i++;
    }
    if (l_Size == 0 || i == l_Size) {
        m_ManipDrawHdls.Add(i_ManipDrawHdl);
    }
    else {
        m_ManipDrawHdls.Insert(i, i_ManipDrawHdl);
    }
}

void Viewport_Z::UnregisterManip(const ManipulatorDraw_ZHdl& i_ManipDrawHdl) {
    S32 i = 0;
    while (i < m_ManipDrawHdls.GetSize()) {
        if (i_ManipDrawHdl == m_ManipDrawHdls[i]) {
            m_ManipDrawHdls.Remove(i);
            return;
        }
        i++;
    }
}
