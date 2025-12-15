#include "Renderer_Z.h"
#include "Node_ZHdl.h"
#include "SystemObject_Z.h"
#include "Types_Z.h"
#include "Math_Z.h"
#include "Node_Z.h"
#pragma dont_inline on

Renderer_Z::Renderer_Z() {
    m_PerlinArray3D.Generate(0.5f, 1.0f, 16);
}

Renderer_Z::~Renderer_Z() {
}

Bool Renderer_Z::Init(S32 i_SizeX, S32 i_SizeY) {
    SetSize(i_SizeX, i_SizeY);
    SwitchScreen(screen_standard);
    m_MipmapedGaussConvol = Compute_MipmapedGaussConvol(&m_MipmapGC, i_SizeX, i_SizeY, 14.0f, 1.0f);
    return TRUE;
}

void Renderer_Z::Shut() {
    for (S32 i = 0; i < m_VertexBufferPtrDA.GetSize(); i++) {
        m_VertexBufferPtrDA[i]->Release();
        Delete_Z m_VertexBufferPtrDA[i];
    }
    for (S32 i = 0; i < m_IndexBufferPtrDA.GetSize(); i++) {
        m_IndexBufferPtrDA[i]->Release();
        Delete_Z m_IndexBufferPtrDA[i];
    }
    m_VertexBufferPtrDA.Flush();
    m_IndexBufferPtrDA.Flush();
}

void Renderer_Z::SetSize(S32 i_SizeX, S32 i_SizeY) {
    m_SizeX = i_SizeX;
    m_SizeY = i_SizeY;
    m_Viewports[6].SetPosAndSize(0, 0, i_SizeX, i_SizeY);
    m_Viewports[6].SetRenderer(this);
}

void Renderer_Z::InitViewport(U32 i_NbViewports) {
    S32 l_SizeX;
    S32 l_SizeY;
    switch (i_NbViewports) {
        case 1:
            m_NbViewports = 1;
            m_Viewports[0].SetPosAndSize(0, 0, m_SizeX, m_SizeY);
            break;
        case 2:
            m_NbViewports = 2;
            if (GetScreenRatio() == RATIO_SCREEN_WIDESCREEN) {
                l_SizeX = m_SizeX >> 1;
                m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, m_SizeY);
                m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, m_SizeY);
            }
            else {
                S32 l_SizeY = m_SizeY >> 1;
                m_Viewports[0].SetPosAndSize(0, 0, m_SizeX, l_SizeY - 1);
                m_Viewports[1].SetPosAndSize(0, l_SizeY + 1, m_SizeX, l_SizeY - 1);
            }
            break;
        case 3: {
            m_NbViewports = 3;
            S32 l_SizeX = m_SizeX >> 1;
            S32 l_SizeY = m_SizeY >> 1;
            m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[2].SetPosAndSize(0, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
        } break;
        case 4: {
            m_NbViewports = 4;
            S32 l_SizeX = m_SizeX >> 1;
            S32 l_SizeY = m_SizeY >> 1;
            m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[2].SetPosAndSize(0, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[3].SetPosAndSize(l_SizeX + 1, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
            break;
        }
    }
    for (S32 i = 0; i < m_NbViewports; i++) {
        m_Viewports[i].SetRenderer(this);
    }
}

void Renderer_Z::SetActiveViewport(S32 i_ViewportId) {
    SetCurrentFogColor(VEC3F_NULL);
    FlushActiveViewport();
    m_ActiveViewport = i_ViewportId;
    Viewport_Z& l_Viewport = GetViewport(i_ViewportId);
    m_HSize = l_Viewport.GetHSize();
    m_VSize = l_Viewport.GetVSize();
    m_HCenter = l_Viewport.GetHCenter();
    m_VCenter = l_Viewport.GetVCenter();
    Node_Z* l_CameraNode = GetViewport(i_ViewportId).GetCamera();
    if (l_CameraNode) {
        Camera_Z* l_Camera = (Camera_Z*)(l_CameraNode->GetObject(FALSE));
        l_Camera->UpdateInverseWorldMatrix(l_CameraNode);
    }
}

void Renderer_Z::FlushActiveViewport() { }

void Renderer_Z::Draw(S32 i_ViewportId, Float i_DeltaTime) {
    SetActiveViewport(i_ViewportId);
    DrawInfo_Z l_DrawInfo;
}

#pragma dont_inline reset
