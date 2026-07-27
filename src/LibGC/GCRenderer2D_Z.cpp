#include "GCRenderer_Z.h"
#include "Assert_Z.h"

static void WriteFloat(Float i_Value, Float& o_Destination) {
    U8* l_Source = (U8*)&i_Value;
    U8* l_Destination = (U8*)&o_Destination;
    l_Destination[0] = l_Source[0];
    l_Destination[1] = l_Source[1];
    l_Destination[2] = l_Source[2];
    l_Destination[3] = l_Source[3];
}

void GCRenderer_Z::DrawQuad(Vec2f& i_TopLeft, Vec2f& i_BottomRight, Color& i_Color, Float i_Z) {
    Float l_Z = Max(i_Z, Renderer_ZCurrentNear);

    U8* l_VertexData = m_Draw2D.Request(NULL, i_Color.a < 1.0f ? TRUE : FALSE, 4);

    Draw2D_Z::GCVertex2DStream* l_Vertex = (Draw2D_Z::GCVertex2DStream*)l_VertexData;

    Float l_VertexZ = -l_Z;

    GXColor l_VertexColor;
    l_VertexColor.r = (U8)(255.0f * i_Color.r);
    l_VertexColor.b = (U8)(255.0f * i_Color.b);
    l_VertexColor.g = (U8)(255.0f * i_Color.g);
    l_VertexColor.a = (U8)(255.0f * i_Color.a);

    WriteFloat(l_VertexZ, l_Vertex[0].m_Position.z);
    l_Vertex[0].m_Color = l_VertexColor;

    WriteFloat(l_VertexZ, l_Vertex[1].m_Position.z);
    l_Vertex[1].m_Color = l_VertexColor;

    WriteFloat(l_VertexZ, l_Vertex[2].m_Position.z);
    l_Vertex[2].m_Color = l_VertexColor;

    WriteFloat(l_VertexZ, l_Vertex[3].m_Position.z);
    l_Vertex[3].m_Color = l_VertexColor;

    WriteFloat(i_TopLeft.x, l_Vertex[0].m_Position.x);
    WriteFloat(i_TopLeft.y, l_Vertex[0].m_Position.y);

    WriteFloat(i_BottomRight.x, l_Vertex[1].m_Position.x);
    WriteFloat(i_TopLeft.y, l_Vertex[1].m_Position.y);

    WriteFloat(i_TopLeft.x, l_Vertex[2].m_Position.x);
    WriteFloat(i_BottomRight.y, l_Vertex[2].m_Position.y);

    WriteFloat(i_BottomRight.x, l_Vertex[3].m_Position.x);
    WriteFloat(i_BottomRight.y, l_Vertex[3].m_Position.y);

    m_Draw2D.CloseRequest();
}

void GCRenderer_Z::DrawQuad(
    Vec2f& i_TopLeft,
    Vec2f& i_BottomRight,
    Vec2f& i_UvTopLeft,
    Vec2f& i_UvBottomRight,
    Vec3f& i_Color,
    Float i_Z
) {
    Float l_Z = Max(i_Z, Renderer_ZCurrentNear);

    Float l_Opacity = 1.0f;
    U32 l_RenderFlags = 0;

    if (m_ActiveMaterial) {
        l_Opacity = Min(1.0f, m_ActiveMaterial->GetOpacity());
        l_RenderFlags = m_ActiveMaterial->GetRenderFlag();
    }

    Bool l_Transparent = FALSE;

    if (
        l_Opacity < 1.0f || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)
    ) {
        l_Transparent = TRUE;
    }

    U8* l_VertexData = m_Draw2D.Request(m_ActiveMaterial, l_Transparent, 4);

    Draw2D_Z::GCVertex2DStream* l_Vertex = (Draw2D_Z::GCVertex2DStream*)l_VertexData;

    Float l_ColorScale = 255.0f;
    Float l_VertexZ = -l_Z;

    U8 l_Red = (U8)(l_ColorScale * i_Color.x);
    U8 l_Green = (U8)(l_ColorScale * i_Color.y);
    U8 l_Blue = (U8)(l_ColorScale * i_Color.z);
    U8 l_Alpha = 0xff;

    l_Vertex[0].m_Color.r = l_Red;
    l_Vertex[0].m_Color.g = l_Green;
    l_Vertex[0].m_Color.b = l_Blue;
    l_Vertex[0].m_Color.a = l_Alpha;
    WriteFloat(l_VertexZ, l_Vertex[0].m_Position.z);

    l_Vertex[1].m_Color.r = l_Red;
    l_Vertex[1].m_Color.g = l_Green;
    l_Vertex[1].m_Color.b = l_Blue;
    l_Vertex[1].m_Color.a = l_Alpha;
    WriteFloat(l_VertexZ, l_Vertex[1].m_Position.z);

    l_Vertex[2].m_Color.r = l_Red;
    l_Vertex[2].m_Color.g = l_Green;
    l_Vertex[2].m_Color.b = l_Blue;
    l_Vertex[2].m_Color.a = l_Alpha;
    WriteFloat(l_VertexZ, l_Vertex[2].m_Position.z);

    l_Vertex[3].m_Color.r = l_Red;
    l_Vertex[3].m_Color.g = l_Green;
    l_Vertex[3].m_Color.b = l_Blue;
    l_Vertex[3].m_Color.a = l_Alpha;
    WriteFloat(l_VertexZ, l_Vertex[3].m_Position.z);

    WriteFloat(i_TopLeft.x, l_Vertex[0].m_Position.x);
    WriteFloat(i_TopLeft.y, l_Vertex[0].m_Position.y);

    WriteFloat(i_BottomRight.x, l_Vertex[1].m_Position.x);
    WriteFloat(i_TopLeft.y, l_Vertex[1].m_Position.y);

    WriteFloat(i_TopLeft.x, l_Vertex[2].m_Position.x);
    WriteFloat(i_BottomRight.y, l_Vertex[2].m_Position.y);

    WriteFloat(i_BottomRight.x, l_Vertex[3].m_Position.x);
    WriteFloat(i_BottomRight.y, l_Vertex[3].m_Position.y);

    WriteFloat(
        i_UvTopLeft.x,
        l_Vertex[0].m_TextureCoordinates.x
    );
    WriteFloat(
        i_UvTopLeft.y,
        l_Vertex[0].m_TextureCoordinates.y
    );

    WriteFloat(
        i_UvBottomRight.x,
        l_Vertex[1].m_TextureCoordinates.x
    );
    WriteFloat(
        i_UvTopLeft.y,
        l_Vertex[1].m_TextureCoordinates.y
    );

    WriteFloat(
        i_UvTopLeft.x,
        l_Vertex[2].m_TextureCoordinates.x
    );
    WriteFloat(
        i_UvBottomRight.y,
        l_Vertex[2].m_TextureCoordinates.y
    );

    WriteFloat(
        i_UvBottomRight.x,
        l_Vertex[3].m_TextureCoordinates.x
    );
    WriteFloat(
        i_UvBottomRight.y,
        l_Vertex[3].m_TextureCoordinates.y
    );

    m_Draw2D.CloseRequest();
}

void GCRenderer_Z::DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Color& a4, Float a5) {
}

void GCRenderer_Z::Draw2DQuad(
    const Vec2f& i_PosBottomLeft,
    const Vec2f& i_PosTopRight,
    const Vec2f& i_UvBottomLeft,
    const Vec2f& i_UvTopRight,
    const Color& i_ColBottomRight,
    const Color& i_ColTopRight,
    Float i_Z
) {
    Float l_Z = Max(i_Z, Renderer_ZCurrentNear);

    Float l_Opacity = Min(
        i_ColBottomRight.a,
        Min(i_ColTopRight.a, 1.0f)
    );

    U32 l_RenderFlags = 0;

    if (m_ActiveMaterial) {
        l_Opacity = Min(l_Opacity, m_ActiveMaterial->GetOpacity());
        l_RenderFlags = m_ActiveMaterial->GetRenderFlag();
    }

    Bool l_Transparent = FALSE;

    if (l_Opacity < 1.0f || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_Transparent = TRUE;
    }

    U8* l_VertexData = m_Draw2D.Request(m_ActiveMaterial, l_Transparent, 4);

    S32 l_ViewportX;
    S32 l_ViewportY;
    S32 l_ViewportWidth;
    S32 l_ViewportHeight;

    m_Viewports[m_ActiveViewport].GetPosAndSize(
        l_ViewportX,
        l_ViewportY,
        l_ViewportWidth,
        l_ViewportHeight
    );

    Draw2D_Z::GCVertex2DStream* l_Vertex = (Draw2D_Z::GCVertex2DStream*)l_VertexData;

    Float l_VertexZ = -l_Z;

    WriteFloat(l_VertexZ, l_Vertex[0].m_Position.z);
    WriteFloat(l_VertexZ, l_Vertex[1].m_Position.z);
    WriteFloat(l_VertexZ, l_Vertex[2].m_Position.z);
    WriteFloat(l_VertexZ, l_Vertex[3].m_Position.z);

    l_Vertex[0].m_Color.a = (U8)((i_ColBottomRight.a * l_Opacity) * 255.0f);

    l_Vertex[2].m_Color.a = (U8)((i_ColTopRight.a * l_Opacity) * 255.0f);

    WriteFloat(
        i_PosBottomLeft.x + l_ViewportX, l_Vertex[0].m_Position.x
    );
    WriteFloat(
        i_PosBottomLeft.y + l_ViewportY, l_Vertex[0].m_Position.y
    );

    WriteFloat(
        i_PosTopRight.x + l_ViewportX, l_Vertex[1].m_Position.x
    );
    WriteFloat(
        i_PosBottomLeft.y + l_ViewportY, l_Vertex[1].m_Position.y
    );

    WriteFloat(
        i_PosBottomLeft.x + l_ViewportX, l_Vertex[2].m_Position.x
    );
    WriteFloat(
        i_PosTopRight.y + l_ViewportY, l_Vertex[2].m_Position.y
    );

    WriteFloat(
        i_PosTopRight.x + l_ViewportX, l_Vertex[3].m_Position.x
    );
    WriteFloat(
        i_PosTopRight.y + l_ViewportY, l_Vertex[3].m_Position.y
    );

    l_Vertex[0].m_Color.r = (U8)(i_ColBottomRight.r * 255.0f);
    l_Vertex[0].m_Color.g = (U8)(i_ColBottomRight.g * 255.0f);
    l_Vertex[0].m_Color.b = (U8)(i_ColBottomRight.b * 255.0f);

    l_Vertex[1].m_Color = l_Vertex[0].m_Color;

    l_Vertex[2].m_Color.r = (U8)(i_ColTopRight.r * 255.0f);
    l_Vertex[2].m_Color.g = (U8)(i_ColTopRight.g * 255.0f);
    l_Vertex[2].m_Color.b = (U8)(i_ColTopRight.b * 255.0f);

    l_Vertex[3].m_Color = l_Vertex[2].m_Color;

    WriteFloat(i_UvBottomLeft.x, l_Vertex[0].m_TextureCoordinates.x);
    WriteFloat(i_UvBottomLeft.y, l_Vertex[0].m_TextureCoordinates.y);

    WriteFloat(i_UvTopRight.x, l_Vertex[1].m_TextureCoordinates.x);
    WriteFloat(i_UvBottomLeft.y, l_Vertex[1].m_TextureCoordinates.y);

    WriteFloat(i_UvBottomLeft.x, l_Vertex[2].m_TextureCoordinates.x);
    WriteFloat(i_UvTopRight.y, l_Vertex[2].m_TextureCoordinates.y);

    WriteFloat(i_UvTopRight.x, l_Vertex[3].m_TextureCoordinates.x);
    WriteFloat(i_UvTopRight.y, l_Vertex[3].m_TextureCoordinates.y);

    m_Draw2D.CloseRequest();
}

void GCRenderer_Z::Draw2DQuad(
    Vec2f* i_Positions,
    Vec3f* i_Colors,
    Vec2f* i_TextureCoordinates,
    Float i_Z,
    Float i_Alpha
) {
    Float l_Z = Max(i_Z, Renderer_ZCurrentNear);
    Float l_Opacity = Min(i_Alpha, 1.0f);
    U32 l_RenderFlags = 0;
    if (m_ActiveMaterial) {
        l_Opacity = Min(l_Opacity, m_ActiveMaterial->GetOpacity());
        l_RenderFlags = m_ActiveMaterial->GetRenderFlag();
    }

    Bool l_Transparent = FALSE;
    if (l_Opacity < 1.0f || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_Transparent = TRUE;
    }
    Material_Z* l_RequestMaterial = m_ActiveMaterial;
    U8* l_VertexData = m_Draw2D.Request(l_RequestMaterial, l_Transparent, 4);

    S32 l_Alpha = (S32)(255.0f * i_Alpha);
    S32 l_ViewportX;
    S32 l_ViewportY;
    S32 l_ViewportWidth;
    S32 l_ViewportHeight;
    m_Viewports[m_ActiveViewport].GetPosAndSize(
        l_ViewportX,
        l_ViewportY,
        l_ViewportWidth,
        l_ViewportHeight
    );

    Draw2D_Z::GCVertex2DStream* l_Vertex = (Draw2D_Z::GCVertex2DStream*)l_VertexData;
    Float l_VertexZ = -l_Z;
    for (S32 i = 0; i < 4; i++) {
        Float l_PositionX = i_Positions[i].x + l_ViewportX;
        WriteFloat(l_PositionX, l_Vertex->m_Position.x);
        WriteFloat(i_Positions[i].y + l_ViewportY, l_Vertex->m_Position.y);
        WriteFloat(i_TextureCoordinates[i].x, l_Vertex->m_TextureCoordinates.x);
        WriteFloat(i_TextureCoordinates[i].y, l_Vertex->m_TextureCoordinates.y);
        WriteFloat(l_VertexZ, l_Vertex->m_Position.z);
        l_Vertex->m_Color.a = l_Alpha;
        l_Vertex->m_Color.r = (U8)(i_Colors[i].x * 255.0f);
        l_Vertex->m_Color.g = (U8)(i_Colors[i].y * 255.0f);
        l_Vertex->m_Color.b = (U8)(i_Colors[i].z * 255.0f);
        l_Vertex++;
    }

    m_Draw2D.CloseRequest();
}

void GCRenderer_Z::DrawStrip(Vec2f* a1, S32 a2, const Color& a3, Float a4) {
}

void GCRenderer_Z::DrawFan(Vec2f* a1, S32 a2, const Color& a3, Float a4) {
}

void GCRenderer_Z::DrawString(
    const Vec2f& i_TopLeft,
    const Char* i_String,
    const Color& i_Color,
    Float i_Z,
    Float i_Scale
) {
    const Char* l_String = i_String;
    Material_Z* l_ActiveMaterial = m_ActiveMaterial;

    SetActiveMaterial(m_FontString.m_MaterialHdl);
    DrawState(ds_opaque);

    Float l_CurX = i_TopLeft.x;
    Float l_TopLeftX = l_CurX;

    Vec3f l_Color(i_Color.r, i_Color.g, i_Color.b);

    Float l_YOffset = i_Scale * 10.0f;
    Float l_CurY = i_TopLeft.y;

    for (; *l_String != '\0'; l_String++) {
        S32 l_Char = *l_String;
        if (l_Char == '\n' || l_Char == '~') {
            l_CurX = l_TopLeftX;
            l_CurY += l_YOffset;
            continue;
        }

        ScanCode_Z& l_ScanCode = m_FontString.m_Characters[l_Char];

        if (l_Char == ' ' || !l_ScanCode.m_Valid) {
            l_CurX += l_ScanCode.m_SizeX * i_Scale;
            continue;
        }

        Float l_RightX = i_Scale * l_ScanCode.m_SizeX + l_CurX;

        Float l_BottomY = i_Scale * l_ScanCode.m_SizeY + l_CurY;

        Vec2f l_TopLeft(l_CurX, l_CurY);
        Vec2f l_BottomRight(l_RightX, l_BottomY);

        DrawQuad(
            l_TopLeft,
            l_BottomRight,
            *(Vec2f*)&l_ScanCode.m_U1,
            *(Vec2f*)&l_ScanCode.m_U2,
            l_Color,
            i_Z
        );

        l_CurX = l_RightX;
    }

    SetActiveMaterial(l_ActiveMaterial);
}

void GCRenderer_Z::DrawString(const Vec3f& a1, const Char* a2, Bool a3) {
}

void GCRenderer_Z::DrawString(const Vec3f& a1, const Char* a2, const Color& a3, Bool a4) {
}

U8* Draw2D_Z::Request(Material_Z* i_Material, Bool i_Transparent, S32 i_VertexCount) {
    S32 l_RequestedVertexCount = i_VertexCount + 2;
    U16 l_DrawState = ds_zonly;
    U32 l_RenderFlags = 0;

    if (i_Material) {
        l_RenderFlags = i_Material->GetRenderFlag();
    }
    if (i_Transparent || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_RenderFlags |= FL_MTL_RDR_IS_ALPHABLENDED;
        l_DrawState |= ds_ablend;
        l_DrawState &= ~ds_zwrite;
    }
    if (!i_Material || i_Material->GetCode() != FL_MTL_CODE_ZONLY) {
        l_DrawState |= ds_cwrite;
    }
    if (l_RenderFlags & FL_MTL_RDR_TWO_SIDE) {
        l_DrawState &= ~ds_cw;
    }

    if (m_RemainingVertexCount < l_RequestedVertexCount || m_CurMaterial != i_Material || m_CurDrawState != l_DrawState || m_CurRenderFlags != l_RenderFlags) {
        UnLock();
        GCListVertex2D& l_List = m_Vtx2DBufferDA.AddRef(m_CurDisplayListIdx);
        l_List.m_PrimType = GX_TRIANGLESTRIP | GX_VTXFMT4;
        l_List.m_VertexCount = 0;
        m_CurWritePtr = l_List.m_VertexData;
        Lock(i_Material, l_DrawState, l_RenderFlags);
        m_PreviousWritePtr = NULL;
    }
    else {
        m_PreviousWritePtr = m_CurWritePtr;
    }

    U8* l_Result = m_CurWritePtr;
    m_CurWritePtr += l_RequestedVertexCount * sizeof(GCVertex2DStream);
    m_RemainingVertexCount -= l_RequestedVertexCount;
    m_Vtx2DBufferDA[m_CurDisplayListIdx].m_VertexCount += i_VertexCount;
    return l_Result;
}

void Draw2D_Z::CloseRequest() {
    GCVertex2DStream* l_CurrentVertex = (GCVertex2DStream*)m_PreviousWritePtr;
    if (!l_CurrentVertex) {
        return;
    }

    l_CurrentVertex[-1].CopyFrom(l_CurrentVertex[0]);
    l_CurrentVertex[-2].CopyFrom(l_CurrentVertex[-3]);
    m_Vtx2DBufferDA[m_CurDisplayListIdx].m_VertexCount += 2;
}

void Draw2D_Z::Init() {
    ASSERTLE_Z(m_Vtx2DBufferDA.GetSize() == 0, "", 529, "Vtx2DBufferDA.GetSize()==0");
}

void Draw2D_Z::Shut() {
}

void Draw2D_Z::UnLock() {
    m_RemainingVertexCount = -1;
    m_CurWritePtr = NULL;
    m_PreviousWritePtr = NULL;
}

void Draw2D_Z::Lock(Material_Z* i_Material, U16 i_DrawState, S32 i_RenderFlags) {
    S32 l_Index;
    l_Index = m_PrimitiveLists.Add();
    m_PrimitiveLists[l_Index].m_DisplayListIdx = m_CurDisplayListIdx;
    m_PrimitiveLists[l_Index].m_Material = i_Material;
    m_PrimitiveLists[l_Index].m_DrawState = i_DrawState;
    m_PrimitiveLists[l_Index].m_RenderFlags = i_RenderFlags;

    m_CurDrawState = i_DrawState;
    m_CurMaterial = i_Material;
    m_CurRenderFlags = i_RenderFlags;
    m_RemainingVertexCount = DRAW2D_VTXBUFFER_NB;
}

void Draw2D_Z::Begin() {
    m_PreviousWritePtr = NULL;
    m_RemainingVertexCount = 0;
    m_CurMaterial = NULL;
    m_CurDisplayListIdx = -1;
    m_PrimitiveLists.Empty();

    S32 l_Count = m_Vtx2DBufferDA.GetSize();
    for (S32 i = 0; i < l_Count; i++) {
        m_Vtx2DBufferDA[i].m_VertexCount = 0;
    }
}

void Draw2D_Z::Minimize() {
    m_Vtx2DBufferDA.Empty();
    m_PrimitiveLists.Minimize();
}

void Draw2D_Z::End() {
    m_RemainingVertexCount = -1;
    m_CurWritePtr = NULL;
    m_PreviousWritePtr = NULL;

    if (m_PrimitiveLists.GetSize()) {
        GXSetCurrentMtx(GX_IDENTITY);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        GCRenderer_Z* l_Renderer = (GCRenderer_Z*)gData.MainRdr;
        GXSetProjection(*(Mtx44*)&l_Renderer->m_OrthoMatrix, GX_ORTHOGRAPHIC);
        l_Renderer->NoOmnis();
        l_Renderer->DisableFog();
        l_Renderer->SetRenderContext(FL_RDR_CONTEXT_TEX_MODULATE);

        for (S32 i = 0; i < m_PrimitiveLists.GetSize(); i++) {
            l_Renderer->SetActiveMaterial(m_PrimitiveLists[i].m_Material);
            l_Renderer->DrawState(m_PrimitiveLists[i].m_DrawState);
            l_Renderer->SetRenderBlendOp(m_PrimitiveLists[i].m_RenderFlags);

            GCListVertex2D& l_List = m_Vtx2DBufferDA[m_PrimitiveLists[i].m_DisplayListIdx];
            if (!l_List.m_VertexCount) {
                break;
            }

            S32 l_ListSize = l_List.m_VertexCount * sizeof(GCVertex2DStream) + 3;
            l_List.m_DisplayListSize = AlignUp_Z(l_ListSize, 32);
            if (l_List.m_DisplayListSize != l_ListSize) {
                memset((U8*)&l_List + l_ListSize, 0, l_List.m_DisplayListSize - l_ListSize);
            }

            DCStoreRange(&l_List, l_List.m_DisplayListSize);
            GXCallDisplayList(&l_List, l_List.m_DisplayListSize);
        }

        GXDrawDone();
        GXSetProjection(*(Mtx44*)&l_Renderer->m_ProjMatrix, GX_PERSPECTIVE);
    }

    m_PrimitiveLists.Empty();
    m_Vtx2DBufferDA.Empty();
}
