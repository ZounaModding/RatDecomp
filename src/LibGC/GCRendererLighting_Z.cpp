#include "GCRenderer_Z.h"
#include "Camera_Z.h"
#include "HFog_Z.h"

Bool GCRenderer_Z::SetLights(const DrawInfo_Z& i_DrawInfo, const Color& i_Color) {
    SetOmnis(i_DrawInfo);
    return SetLight(i_DrawInfo, i_Color);
}

Bool GCRenderer_Z::SetLight(const DrawInfo_Z& i_DrawInfo, const Color& i_Color) {
    return FALSE;
}

void GCRenderer_Z::SetRadiosity(Node_Z* i_Node) {
    m_ActiveBitmaps[BITMAP_RADIOSITY] = i_Node->GetRadiosityBmap();
    Bitmap_Z* l_Bitmap;
    if (m_ActiveBitmaps[BITMAP_RADIOSITY]) {
        l_Bitmap = (Bitmap_Z*)GETPTR(m_ActiveBitmaps[BITMAP_RADIOSITY]->GetHandle());
    }
    else {
        l_Bitmap = NULL;
    }
    SetTexture(l_Bitmap, GX_CLAMP, GX_CLAMP, GX_TEXMAP1);
}

void GCRenderer_Z::SetFog(const DrawInfo_Z& i_DrawInfo) {
    if (!i_DrawInfo.m_CurrentNodeHFogData || (i_DrawInfo.m_Flag & DrawInfo_Z::FL_DRAWINFO_NO_HFOG) || !i_DrawInfo.m_CurrentNodeHFogData->IsActive()) {
        DisableFog();
    }
    else {
        if (m_CurMainFog != i_DrawInfo.m_MainHFogData && !m_CurMainFog) {
            m_FogNear = i_DrawInfo.m_Camera->GetNearClip();
            if (m_FogNear < Renderer_ZDefaultNear) {
                m_FogNear = Renderer_ZDefaultNear;
            }
            m_FogFar = i_DrawInfo.m_Camera->GetFarClip();
            const Mat4x4& l_FogMatrix = i_DrawInfo.m_MainHFogData->GetWorldMatrix();
            Float l_DeltaX = i_DrawInfo.m_CameraTranslation.x - l_FogMatrix.m[3][0];
            Float l_DeltaZ = i_DrawInfo.m_CameraTranslation.z - l_FogMatrix.m[3][2];
            Float l_Distance = Sqrt(l_DeltaX * l_DeltaX + l_DeltaZ * l_DeltaZ);
            Float l_DistanceNorm = Max<Float>(
                Sqrt(
                    i_DrawInfo.m_CameraDirection.x * i_DrawInfo.m_CameraDirection.x + i_DrawInfo.m_CameraDirection.z * i_DrawInfo.m_CameraDirection.z
                ),
                0.1f
            );
            l_DistanceNorm = l_Distance / l_DistanceNorm;
            m_FogStartZ = m_FogNear;
            m_FogEndZ = m_FogStartZ + l_DistanceNorm;
            m_CurEnabledFog = NULL;
            m_CurMainFog = i_DrawInfo.m_MainHFogData;
        }
        EnableFog();
    }
}

#pragma warn_uninitializedvar off

void GCRenderer_Z::DisableFog() {
    if (m_CurEnabledFog) {
        m_CurEnabledFog = NULL;
        GXColor l_NoColor;
        GXSetFog(GX_FOG_NONE, 0.5f, 1.0f, 2.0f, 3.0f, l_NoColor);
    }
}

void GCRenderer_Z::EnableFog() {
    if (!m_CurEnabledFog && m_CurMainFog) {
        m_CurEnabledFog = m_CurMainFog;
        Vec3f& l_FogColor = m_CurMainFog->GetColor();
        GXColor l_Color;
        l_Color.r = (U8)(255.0f * l_FogColor.x);
        l_Color.g = (U8)(255.0f * l_FogColor.y);
        l_Color.b = (U8)(255.0f * l_FogColor.z);
        GXSetFog(GX_FOG_LIN, m_FogStartZ, m_FogEndZ, m_FogNear, m_FogFar, l_Color);
    }
}

void GCRenderer_Z::NoFog() {
    m_CurMainFog = NULL;
    m_CurEnabledFog = NULL;
    m_CurOmniLightCount = 0;
    m_CurOmniLightMask = GX_LIGHT_NULL;

    GXColor l_NoColor;
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, l_NoColor);
}

#pragma warn_uninitializedvar on

void GCRenderer_Z::NoOmnis() {
    m_CurOmniLightMask = GX_LIGHT_NULL;
    m_CurOmniLightCount = 0;
}

void GCRenderer_Z::SetOmnis(const DrawInfo_Z& i_DrawInfo) {
}
