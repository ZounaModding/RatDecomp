#include "DCRenderer_Z.h"
#include "Material_Z.h"

void DCRenderer_Z::EmitQuad(const Vec2f& i_TopLeft, const Vec2f& i_BottomRight, const Vec2f& i_UvTopLeft, const Vec2f& i_UvBottomRight, const Color& i_ColorTop, const Color& i_ColorBottom, Float i_Z) {
    U32 l_RenderFlags = 0;
    Bool l_Transparent = FALSE;
    Float l_Opacity = Min(i_ColorTop.a, i_ColorBottom.a);
    if (m_ActiveMaterial) {
        l_Opacity = Min(l_Opacity, m_ActiveMaterial->GetOpacity());
        l_RenderFlags = m_ActiveMaterial->GetRenderFlag();
    }
    if (l_Opacity < 1.0f || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_Transparent = TRUE;
    }
    S32 l_ViewportX;
    S32 l_ViewportY;
    S32 l_ViewportSizeX;
    S32 l_ViewportSizeY;
    m_Viewports[m_ActiveViewport].GetPosAndSize(l_ViewportX, l_ViewportY, l_ViewportSizeX, l_ViewportSizeY);

    pvr_vertex_t* l_VertexData = m_Draw2D.Request(m_ActiveMaterial, l_Transparent, 4);
    for (S32 i = 0; i < 4; i++) {
        const Color& l_Color = (i & 2) ? i_ColorBottom : i_ColorTop;
        l_VertexData[i].z = 1.0f / Max(i_Z, Renderer_ZCurrentNear);
        l_VertexData[i].x = ((i & 1) ? i_BottomRight.x : i_TopLeft.x) + l_ViewportX;
        l_VertexData[i].y = ((i & 2) ? i_BottomRight.y : i_TopLeft.y) + l_ViewportY;
        l_VertexData[i].u = (i & 1) ? i_UvBottomRight.x : i_UvTopLeft.x;
        l_VertexData[i].v = (i & 2) ? i_UvBottomRight.y : i_UvTopLeft.y;
        l_VertexData[i].argb = PVR_PACK_COLOR(
            l_Opacity,
            Min(l_Color.r * DC_MODULATE_SCALE, 1.0f),
            Min(l_Color.g * DC_MODULATE_SCALE, 1.0f),
            Min(l_Color.b * DC_MODULATE_SCALE, 1.0f)
        );
        l_VertexData[i].oargb = 0;
    }
}

void DCRenderer_Z::DrawQuad(Vec2f& i_TopLeft, Vec2f& i_BottomRight, Color& i_Color, Float i_Z) {
    Vec2f l_Uv(0.0f, 0.0f);
    EmitQuad(i_TopLeft, i_BottomRight, l_Uv, l_Uv, i_Color, i_Color, i_Z);
}

void DCRenderer_Z::DrawQuad(Vec2f& i_TopLeft, Vec2f& i_BottomRight, Vec2f& i_UvTopLeft, Vec2f& i_UvBottomRight, Vec3f& i_Color, Float i_Z) {
    Color l_Color(i_Color.x, i_Color.y, i_Color.z, 1.0f);
    EmitQuad(i_TopLeft, i_BottomRight, i_UvTopLeft, i_UvBottomRight, l_Color, l_Color, i_Z);
}

void DCRenderer_Z::Draw2DQuad(const Vec2f& i_TopLeft, const Vec2f& i_BottomRight, const Vec2f& i_UvTopLeft, const Vec2f& i_UvBottomRight, const Color& i_ColorTop, const Color& i_ColorBottom, Float i_Z) {
    EmitQuad(i_TopLeft, i_BottomRight, i_UvTopLeft, i_UvBottomRight, i_ColorTop, i_ColorBottom, i_Z);
}

void DCRenderer_Z::Draw2DQuad(Vec2f* i_Positions, Vec3f* i_Colors, Vec2f* i_TextureCoordinates, Float i_Z, Float i_Alpha) {
    U32 l_RenderFlags = 0;
    Bool l_Transparent = FALSE;
    Float l_Opacity = Min(i_Alpha, 1.0f);
    if (m_ActiveMaterial) {
        l_Opacity = Min(l_Opacity, m_ActiveMaterial->GetOpacity());
        l_RenderFlags = m_ActiveMaterial->GetRenderFlag();
    }
    if (l_Opacity < 1.0f || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_Transparent = TRUE;
    }
    S32 l_ViewportX;
    S32 l_ViewportY;
    S32 l_ViewportSizeX;
    S32 l_ViewportSizeY;
    m_Viewports[m_ActiveViewport].GetPosAndSize(l_ViewportX, l_ViewportY, l_ViewportSizeX, l_ViewportSizeY);

    pvr_vertex_t* l_VertexData = m_Draw2D.Request(m_ActiveMaterial, l_Transparent, 4);
    for (S32 i = 0; i < 4; i++) {
        l_VertexData[i].z = 1.0f / Max(i_Z, Renderer_ZCurrentNear);
        l_VertexData[i].x = i_Positions[i].x + l_ViewportX;
        l_VertexData[i].y = i_Positions[i].y + l_ViewportY;
        l_VertexData[i].u = i_TextureCoordinates[i].x;
        l_VertexData[i].v = i_TextureCoordinates[i].y;
        l_VertexData[i].argb = PVR_PACK_COLOR(
            l_Opacity,
            Min(i_Colors[i].x * DC_MODULATE_SCALE, 1.0f),
            Min(i_Colors[i].y * DC_MODULATE_SCALE, 1.0f),
            Min(i_Colors[i].z * DC_MODULATE_SCALE, 1.0f)
        );
        l_VertexData[i].oargb = 0;
    }
}

void DCRenderer_Z::DrawString(const Vec2f& i_Pos, const Char* i_Text, const Color& i_Color, Float i_Z, Float i_Scale) {
    Material_Z* l_SavedMaterial = m_ActiveMaterial;
    Material_Z* l_FontMaterial = m_DebugFontString.m_MaterialHdl;
    SetActiveMaterial(l_FontMaterial);

    Vec3f l_Color(i_Color.r, i_Color.g, i_Color.b);

    Float l_LineAdvance = DEBUG_FONT_LINE_ADVANCE * i_Scale;
    Float l_CursorX = i_Pos.x;
    Float l_CursorY = i_Pos.y;

    for (; *i_Text; i_Text++) {
        S32 l_Char = (U8)*i_Text;

        if (l_Char == '\n' || l_Char == '~') {
            l_CursorY += l_LineAdvance;
            l_CursorX = i_Pos.x;
            continue;
        }

        ScanCode_Z& l_ScanCode = m_DebugFontString.m_Characters[l_Char];

        if (l_Char == ' ' || !l_ScanCode.m_Valid) {
            l_CursorX += l_ScanCode.m_SizeX * i_Scale;
            continue;
        }

        Vec2f l_TopLeft(l_CursorX, l_CursorY);
        Vec2f l_BottomRight(
            l_CursorX + l_ScanCode.m_SizeX * i_Scale,
            l_CursorY + l_ScanCode.m_SizeY * i_Scale
        );
        Vec2f l_UvTopLeft(l_ScanCode.m_U1, l_ScanCode.m_V1);
        Vec2f l_UvBottomRight(l_ScanCode.m_U2, l_ScanCode.m_V2);

        DrawQuad(l_TopLeft, l_BottomRight, l_UvTopLeft, l_UvBottomRight, l_Color, i_Z);

        l_CursorX = l_BottomRight.x;
    }

    SetActiveMaterial(l_SavedMaterial);
}
