#include "GCRenderer_Z.h"

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

void GCRenderer_Z::NoOmnis() {
    m_CurOmniLightMask = GX_LIGHT_NULL;
    m_CurOmniLightCount = 0;
}
