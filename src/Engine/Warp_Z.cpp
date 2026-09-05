#include "Warp_Z.h"
#include "AnimFrame_Z.h"
#include "Camera_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "SystemDatas_Z.h"
#include "Viewport_Z.h"

Warp_Z::Warp_Z() {
    m_Flag = 0;
    m_UnkVec3f_0xb4.z = 50.0f;
    m_UnkVec3f_0xb4.x = 1.0f;
    m_UnkVec3f_0xb4.y = 0.0f;
}

Bool Warp_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    S32 i;
    for (i = 0; i < MAX_WARP_FACE; i++) {
        MarkValidHandle_Z(m_MaterialAnimHdls[i]);
    }
    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        MarkValidHandle_Z(m_AnimFrameHdls[i]);
    }
    MarkValidHandle_Z(m_RootNodeHdl);
    return TRUE;
}

void Warp_Z::Update() {
    Node_Z* l_RootNode = m_RootNodeHdl;
    if (l_RootNode) {
        if (l_RootNode->IsFlagEnable(FL_NODE_UPDATE_LIGHTING)) {
            l_RootNode->DisableFlag(FL_NODE_UPDATE_LIGHTING);
            LightData_Z* l_DefaultLight = gData.SystemDatas->GetDefaultLight();
            l_RootNode->SetLighting(l_DefaultLight, NULL, FALSE);
        }
        if (l_RootNode->IsFlagEnable(FL_NODE_SCAN)) {
            l_RootNode->Update();
            m_ChildNodeHdls.Empty();
            l_RootNode->GetAllNodes(m_ChildNodeHdls);
        }
    }
}

// TODO: Finish matching
void Warp_Z::DrawScene(DrawInfo_Z& i_DrawInfo) {
    U32 l_DrawObject;
    U32 l_ObjectDatasHideFlag;
    U32 l_OldDrawInfoFlag = i_DrawInfo.m_Flag;
    i_DrawInfo.m_Flag = DrawInfo_Z::FL_DRAWINFO_NO_HFOG;
    i_DrawInfo.m_OmniFrustPtrSA.Empty();

    Camera_Z* l_Camera = i_DrawInfo.m_Camera;
    Float l_NearClip = l_Camera->GetFrustrum().m_NearClip;
    Float l_FarClip = l_Camera->GetFrustrum().m_FarClip;
    Float l_OccludedFarClip = l_Camera->GetFrustrum().m_OccludedFarClip;
    Float l_TopViewBoundsMinX = l_Camera->GetFrustrum().m_TopViewBoundsMin.x;
    Float l_TopViewBoundsMinY = l_Camera->GetFrustrum().m_TopViewBoundsMin.y;
    Float l_TopViewBoundsMaxX = l_Camera->GetFrustrum().m_TopViewBoundsMax.x;
    Float l_TopViewBoundsMaxY = l_Camera->GetFrustrum().m_TopViewBoundsMax.y;

    l_Camera->SetNearClip(1.0f);
    i_DrawInfo.m_Camera->SetFarClip(300.0f);
    i_DrawInfo.m_Vp->UpdateFrustrum();

    if (!(m_Flag & FL_WARP_HIDE_SCENE)) {
        S32 l_ViewportCount = i_DrawInfo.m_VpCount;
        U32 l_ViewportId = i_DrawInfo.m_VpId;
        const Frustrum_Z& l_DrawFrustrum = i_DrawInfo.m_Camera->GetFrustrum();
        U32 l_NodeHideFlag = (FL_NODE_1VP_HIDE << (l_ViewportCount - 1)) | FL_NODE_HIDE;
        l_ObjectDatasHideFlag = (FL_OBJECTDATAS_VP0_HIDE << l_ViewportId) | FL_OBJECTDATAS_HIDE;
        U32 l_DrawObjectDatas;
        S32 i;
        for (i = 0; i < m_ChildNodeHdls.GetSize(); i++) {
            Node_Z* l_Node = m_ChildNodeHdls[i];
            if (!(l_Node->GetFlag() & l_NodeHideFlag)) {
                Object_Z* l_Object = l_Node->GetObjectA();
                if (l_Object) {
                    ObjectDatas_Z* l_ObjectDatas = l_Node->GetObjectDatas();
                    if (l_ObjectDatas) {
                        if (l_ObjectDatas->GetFlag() & l_ObjectDatasHideFlag) {
                            continue;
                        }
                        l_DrawObjectDatas = l_ObjectDatas->GetFlag() & FL_OBJECTDATAS_DISPLAY;
                    }
                    else {
                        l_DrawObjectDatas = 0;
                    }
                    l_DrawObject = l_Object->GetFlag() & FL_OBJECT_DISPLAY;
                    if (l_DrawObject | l_DrawObjectDatas) {
                        i_DrawInfo.m_Local2Cam = i_DrawInfo.m_World2Cam * l_Node->GetWorldMatrix();
                        if (SphereVsFrustrum(l_DrawFrustrum, i_DrawInfo.m_Local2Cam, l_Object->GetBSphere(), i_DrawInfo.m_ClipSph, l_Node->GetUniformScale())) {
                            i_DrawInfo.m_Node = l_Node;
                            i_DrawInfo.m_LightData = l_Node->GetLight();
                            i_DrawInfo.m_CurrentNodeHFogData = l_Node->GetHFog();
                            i_DrawInfo.m_FadeValue = 1.0f;
                            if (l_DrawObject) {
                                l_Object->Draw(i_DrawInfo, l_ObjectDatas);
                            }
                            if (l_DrawObjectDatas) {
                                l_ObjectDatas->Draw(i_DrawInfo);
                            }
                        }
                    }
                }
            }
        }
    }

    if (GetUser3D(i_DrawInfo.m_VpId)) {
        GetUser3D(i_DrawInfo.m_VpId)->Draw(i_DrawInfo.m_VpId);
    }
    i_DrawInfo.m_Flag = DrawInfo_Z::FL_DRAWINFO_NO_HFOG;
    i_DrawInfo.m_Vp->GetRenderer()->DrawTransparent(i_DrawInfo);
    i_DrawInfo.m_Flag = l_OldDrawInfoFlag;

    i_DrawInfo.m_Camera->SetNearClip(l_NearClip);
    i_DrawInfo.m_Camera->SetFarClip(l_FarClip);
    i_DrawInfo.m_Vp->UpdateFrustrum();
    i_DrawInfo.m_Camera->SetOccludedFarClip(l_OccludedFarClip);
    Frustrum_Z& l_RestoredFrustrum = i_DrawInfo.m_Camera->GetFrustrum();
    l_RestoredFrustrum.m_TopViewBoundsMin.x = l_TopViewBoundsMinX;
    l_RestoredFrustrum.m_TopViewBoundsMin.y = l_TopViewBoundsMinY;
    l_RestoredFrustrum.m_TopViewBoundsMax.x = l_TopViewBoundsMaxX;
    l_RestoredFrustrum.m_TopViewBoundsMax.y = l_TopViewBoundsMaxY;
}

void Warp_Z::UpdateAnims(Float i_DeltaTime) {
    S32 i;
    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        m_AnimFrameHdls[i]->UpdateMessage(i_DeltaTime);
    }
    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        m_AnimFrameHdls[i]->Update();
    }
}
