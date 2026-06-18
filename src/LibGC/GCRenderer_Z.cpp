#include "GCRenderer_Z.h"
#include "TextureManager_Z.h"
#include "SkinGC_Z.h"
#include "SystemDatas_Z.h"
#include "Types_Z.h"
#include "SurfaceGC_Z.h"
#include "MathGC_Z.h"

#include <vi.h>

S32 RetraceCount;

Bool GCRenderer_Z::Init(S32 i_SizeX, S32 i_SizeY) {
    if (!Renderer_Z::Init(i_SizeX, i_SizeY)) {
        return FALSE;
    }

    m_FifoBuffer = (U8*)AllocAlignCL_Z(FIFO_BUFFER_SIZE, "FIFO Rendu", 60, GC_BUFFER_ALIGN);
    m_FifoObject = GXInit(m_FifoBuffer, FIFO_BUFFER_SIZE);

    VIInit();

    switch (VIGetTvFormat()) {
        case VI_NTSC:
            GXAdjustForOverscan(
                &GXNtsc480IntDf,
                &m_RenderModeObj,
                (U16)((640 - m_SizeX) / 2),
                (U16)((480 - m_SizeY) / 2)
            );
            m_RenderModeObj.viWidth = 640;
            m_RenderModeObj.viXOrigin = 40;
            break;

        case VI_PAL:
            GXAdjustForOverscan(
                &GXPal528IntDf,
                &m_RenderModeObj,
                (U16)((640 - m_SizeX) / 2),
                (U16)((528 - m_SizeY) / 2)
            );
            m_RenderModeObj.viWidth = 640;
            m_RenderModeObj.viXOrigin = 40;
            break;

        case VI_MPAL:
            GXAdjustForOverscan(
                &GXMpal480IntDf,
                &m_RenderModeObj,
                (U16)((640 - m_SizeX) / 2),
                (U16)((480 - m_SizeY) / 2)
            );
            m_RenderModeObj.viWidth = 640;
            m_RenderModeObj.viXOrigin = 40;
            break;
    }

    VIConfigure(&m_RenderModeObj);

    GXSetScissor(0, 0, m_RenderModeObj.fbWidth, m_RenderModeObj.efbHeight);
    GXSetDispCopySrc(0, 0, m_RenderModeObj.fbWidth, m_RenderModeObj.efbHeight);

    U32 l_DispCopyYScale = GXSetDispCopyYScale((Float)m_RenderModeObj.xfbHeight / (Float)m_RenderModeObj.efbHeight);
    GXSetDispCopyDst((U16)m_SizeX, (U16)l_DispCopyYScale);

    GXSetCopyFilter(
        m_RenderModeObj.aa,
        m_RenderModeObj.sample_pattern,
        TRUE,
        m_RenderModeObj.vfilter
    );

    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    GXSetDither(TRUE);
    GXSetFieldMode(m_RenderModeObj.field_rendering, m_RenderModeObj.xfbHeight < m_RenderModeObj.viHeight);

    U16 l_FrameBufferWidth = AlignUp_Z((U16)m_SizeX, 16);
    U32 l_FrameBufferSize = l_FrameBufferWidth * l_DispCopyYScale * 2;

    m_FrameBuffers[0] = (U8*)AllocAlignCL_Z(l_FrameBufferSize, "FrameBuffer A", 131, GC_BUFFER_ALIGN);
    m_FrameBuffers[1] = (U8*)AllocAlignCL_Z(l_FrameBufferSize, "FrameBuffer B", 132, GC_BUFFER_ALIGN);

    ClearAFrameBuffer(m_FrameBuffers[0], l_FrameBufferSize);
    ClearAFrameBuffer(m_FrameBuffers[1], l_FrameBufferSize);

    for (S32 i = 0; i < 32; i++) {
        m_VizQueryDisplayLists[i].Alloc(96);
    }

    m_FrameBufferTextureSize = GXGetTexBufferSize((U16)m_SizeX, (U16)m_SizeY, GX_TF_RGBA8, FALSE, 0);
    m_FrameBufferTextureData = (U8*)AllocAlignCL_Z(m_FrameBufferTextureSize, "FrameBuffer Texture", 142, GC_BUFFER_ALIGN);

    DCInvalidateRange(m_FrameBufferTextureData, m_FrameBufferTextureSize);

    InitRenderStates();

    m_CurFrameBuffer = m_FrameBuffers[0];
    m_FrameBufferIdx = 0;
    VISetNextFrameBuffer(m_CurFrameBuffer);

    m_SkipFrameBufferEffects = TRUE;

    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();

    m_CurFrameBuffer = m_FrameBuffers[1];
    m_FrameBufferIdx = 1;

    m_Draw2D.Init();
    m_FontString.Init();

    return TRUE;
}

void GCRenderer_Z::MarkHandles() {
    m_FontString.MarkHandles();
}

void GCRenderer_Z::ClearAFrameBuffer(U8* i_Buffer, S32 i_Size) {
    U32* l_WritePtr = (U32*)i_Buffer;
    U32* l_WriteEnd = (U32*)(i_Buffer + i_Size);
    while (l_WritePtr < l_WriteEnd) {
        *l_WritePtr++ = 0x00800080;
    }
    DCFlushRange(i_Buffer, i_Size);
}

void GCRenderer_Z::InitRenderStates() {
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    GXSetClipMode(GX_CLIP_ENABLE);

    GXSetTexCoordGen2(
        GX_TEXCOORD0,
        GX_TG_MTX3x4,
        GX_TG_TEXCOORD0,
        GX_TEXMTX0,
        FALSE,
        GX_PTIDENTITY
    );

    // Surface vertices (SurfaceGC_Z) and Movies
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_S16, 10);

    // Mesh vertices (MeshGC_Z)
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 10);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX1, GX_TEX_ST, GX_S16, 10);

    // Skinned vertices (SkinGC_Z)
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_POS, GX_POS_XYZ, GX_S16, 12);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);
    SetupSkinnedVertexQuantization();

    // Mesh volume vertices (MeshDataGC_Z::DrawVolume) and VizQuery
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    // RotShapeGC_Z vertices
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_POS, GX_POS_XYZ, GX_S16, 10);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);

    // 2D textured vertices (UI, etc.)
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    // 2D untextured vertices (Debug lines, etc.)
    GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    // FlareGC_Z vertices
    GXSetVtxAttrFmt(GX_VTXFMT6, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT6, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    // Mesh volume vertices (MeshDataGC_Z::DrawVolume) and VizQuery (again, for some reason)
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    m_RenderContextFlag = -1;
    m_VizQueryDisplayListCount = 0;
    for (S32 i = 0; i < 20; i++) {
        m_DrawOrderGroupDrawCallCount[i] = 0;
    }
}

void GCRenderer_Z::Shut() {
    Renderer_Z::Shut();
    GXAbortFrame();
    m_SkipFrameBufferEffects = TRUE;

    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();

    if (m_FrameBuffers[0]) {
        Free_Z(m_FrameBuffers[0]);
    }
    m_FrameBuffers[0] = NULL;
    if (m_FrameBuffers[1]) {
        Free_Z(m_FrameBuffers[1]);
    }
    m_FrameBuffers[1] = NULL;
    if (m_FifoBuffer) {
        Free_Z(m_FifoBuffer);
    }
    m_FifoBuffer = NULL;
    if (m_FrameBufferTextureData) {
        Free_Z(m_FrameBufferTextureData);
    }
    m_FrameBufferTextureData = NULL;
    m_ARamAllocator.Shut();
    m_Draw2D.Shut();
}

void GCRenderer_Z::Reset() { }

void GCRenderer_Z::BeginRender() {
    Renderer_Z::BeginRender();

    if (m_RenderModeObj.field_rendering) {
        GXSetViewportJitter(0.0f, 0.0f, (Float)m_RenderModeObj.fbWidth, (Float)m_RenderModeObj.efbHeight, 0.0f, 1.0f, VIGetNextField());
    }
    else {
        GXSetViewport(0.0f, 0.0f, (Float)m_RenderModeObj.fbWidth, (Float)m_RenderModeObj.efbHeight, 0.0f, 1.0f);
    }

    GXSetScissor(0, 0, (Float)m_RenderModeObj.fbWidth, (Float)m_RenderModeObj.efbHeight);

    m_CurBlendFlags = -1;
    m_RenderContextFlag = -1;

    RetraceCount = VIGetRetraceCount();

    gData.TextureMgr->BeginRender();

    InitRenderStates();

    m_Draw2D.Begin();
}

void GCRenderer_Z::EndRender(Float i_DeltaTime) {
    GXColor l_ClearColor = { 0, 0, 0, 0xff };

    m_Draw2D.End();
    GXFlush();

    m_Draw3D.EndRender();

    gData.SurfaceCache->GetState(m_PatchCacheState);

    S32 l_TotalTextureNb;
    S32 l_TextureInUseNb;
    gData.TextureMgr->EndRender(l_TotalTextureNb, l_TextureInUseNb);

    m_LightCacheState2.m_TotalNb = l_TotalTextureNb;
    m_LightCacheState2.m_InUseNb = m_LightCacheState2.m_MaxInUseNb = l_TextureInUseNb;

    Renderer_Z::EndRender(i_DeltaTime);

    GXSetCopyClear(l_ClearColor, 0x00ffffff);

    DrawState(ds_opaque);

    if (!m_CopyFrameBufferAlpha) {
        DrawState(ds_opaque_no_alpha_write);
    }

    GXCopyDisp(m_CurFrameBuffer, TRUE);
    GXDrawDone();

    if (m_FrameBufferIdx >= 2 || !m_SkipFrameBufferEffects) {
        m_SkipFrameBufferEffects = FALSE;
        VISetBlack(FALSE);
        VISetNextFrameBuffer(m_CurFrameBuffer);
        VIFlush();
    }

    if (gData.m_GameFlag & FL_GAME_2_FRAMES) {
        while (RetraceCount == VIGetRetraceCount()) {
        }
    }

    if (m_EffectFlag & FL_EFFECT_VSYNC) {
        S32 l_RetraceDelta = VIGetRetraceCount() - RetraceCount;
        S32 l_TargetRetraceDelta = (gData.m_GameFlag & FL_GAME_2_FRAMES) == FL_GAME_2_FRAMES;

        if (l_RetraceDelta <= l_TargetRetraceDelta) {
            VIWaitForRetrace();
        }
    }

    VIFlush();

    m_FrameBufferIdx++;

    if (m_FrameBufferIdx & 1) {
        m_CurFrameBuffer = m_FrameBuffers[1];
    }
    else {
        m_CurFrameBuffer = m_FrameBuffers[0];
    }
}

void GCRenderer_Z::ClearZBuffer(S32 i_X, S32 i_Y, S32 i_Width, S32 i_Height) {
    GXSetZScaleOffset(0.0f, 1.0f);

    NoOmnis();
    DisableFog();

    SetActiveMaterial(NULL);

    SetRenderContext(FL_RDR_CONTEXT_TEX_MODULATE);
    DrawState(ds_zwrite);

    S32 l_X = i_X;
    S32 l_Y = i_Y;
    S32 l_Right = i_X + i_Width;
    S32 l_Bottom = i_Y + i_Height;
    S32 l_Width = i_Width;
    S32 l_Height = i_Height;

    Vec2f l_UVMin;
    l_UVMin.x = 0.0f;
    l_UVMin.y = 0.0f;

    Vec2f l_UVMax;
    l_UVMax.x = 1.0f;
    l_UVMax.y = 1.0f;

    Vec2f l_PosMin;
    l_PosMin.x = (Float)l_X;
    l_PosMin.y = (Float)l_Y;

    Vec2f l_PosMax;
    l_PosMax.x = (Float)l_Right;
    l_PosMax.y = (Float)l_Bottom;

    Vec2f l_Size;
    l_Size.x = (Float)l_Width;
    l_Size.y = (Float)l_Height;

    Color l_Color;
    l_Color.r = 1.0f;
    l_Color.g = 1.0f;
    l_Color.b = 0.0f;
    l_Color.a = 1.0f;

    ImmediatQuad(l_UVMin, l_UVMax, l_PosMin, l_PosMax, l_Size, l_Color, 0.9f);

    GXSetZScaleOffset(1.0f, 0.0f);
}

void GCRenderer_Z::FlushActiveViewport() {
    m_Draw2D.End();
}

void GCRenderer_Z::Draw(S32 i_ViewportId, Float i_DeltaTime) {
    Viewport_Z& l_Viewport = m_Viewports[i_ViewportId];

    m_VSize = -m_VSize;

    S32 l_X;
    S32 l_Y;
    S32 l_SizeX;
    S32 l_SizeY;
    l_Viewport.GetPosAndSize(l_X, l_Y, l_SizeX, l_SizeY);

    Float l_Near = Renderer_ZDefaultNear - 0.001f;
    Float l_Far = 100.0f;

    if (l_Viewport.GetCamera().IsValid()) {
        Camera_Z* l_Camera = (Camera_Z*)l_Viewport.GetCamera()->GetObjectA();

        for (S32 i = 0; i < i_ViewportId; i++) {
            S32 l_OtherX;
            S32 l_OtherY;
            S32 l_OtherSizeX;
            S32 l_OtherSizeY;
            m_Viewports[i].GetPosAndSize(l_OtherX, l_OtherY, l_OtherSizeX, l_OtherSizeY);

            S32 l_InterRight = Min<S32>(l_X + l_SizeX, l_OtherX + l_OtherSizeX);
            S32 l_InterLeft = Max<S32>(l_X, l_OtherX);
            S32 l_InterWidth = l_InterRight - l_InterLeft;

            S32 l_InterBottom = Min<S32>(l_Y + l_SizeY, l_OtherY + l_OtherSizeY);
            S32 l_InterTop = Max<S32>(l_Y, l_OtherY);
            S32 l_InterHeight = l_InterBottom - l_InterTop;

            if (l_InterWidth > 0 && l_InterHeight > 0) {
                ClearZBuffer(l_X, l_Y, l_SizeX, l_SizeY);
                break;
            }
        }

        l_Near = l_Camera->GetNearClip();
        l_Far = l_Camera->GetFarClip();

        if (l_Near < Renderer_ZDefaultNear) {
            l_Near = Renderer_ZDefaultNear;
        }

        l_Near -= 0.001f;

        SetWorldToCam(l_Camera->GetInverseWorldMatrix());

        Float l_FarCoef = l_Far / (l_Far - l_Near);
        Float l_NearFarCoef = l_FarCoef * l_Near;

        memset((void*)&m_ProjMatrix, 0, sizeof(m_ProjMatrix));

        (*(Mtx44*)&m_ProjMatrix)[0][0] = 1.0f / l_Viewport.GetHRatio();
        (*(Mtx44*)&m_ProjMatrix)[1][1] = 1.0f / l_Viewport.GetVRatio();

        (*(Mtx44*)&m_ProjMatrix)[2][2] = 1.0f - l_FarCoef;
        (*(Mtx44*)&m_ProjMatrix)[2][3] = -l_NearFarCoef;
        (*(Mtx44*)&m_ProjMatrix)[3][2] = -1.0f;
    }
    else {
        l_X = 0;
        l_Y = 0;
        l_SizeX = m_SizeX;
        l_SizeY = m_SizeY;

        Float l_Far = 100.0f;
        Float l_FarCoef = l_Far / (l_Far - l_Near);
        memset((void*)&m_ProjMatrix, 0, sizeof(m_ProjMatrix));

        (*(Mtx44*)&m_ProjMatrix)[0][0] = 1.0f;
        (*(Mtx44*)&m_ProjMatrix)[1][1] = 1.0f;
        (*(Mtx44*)&m_ProjMatrix)[2][2] = 1.0f - l_FarCoef;
        (*(Mtx44*)&m_ProjMatrix)[2][3] = -(l_FarCoef * l_Near);
        (*(Mtx44*)&m_ProjMatrix)[3][2] = -1.0f;
    }

    C_MTXOrtho(
        *(Mtx44*)&m_OrthoMatrix,
        l_Y,
        l_SizeY,
        l_X,
        l_SizeX,
        l_Near,
        l_Far
    );

    Renderer_ZCurrentNear = l_Near += 0.001f;

    GXSetProjection(*(Mtx44*)&m_ProjMatrix, GX_PERSPECTIVE);

    GXSetViewport(
        l_X,
        l_Y,
        l_SizeX,
        l_SizeY,
        0.0f,
        1.0f
    );

    GXSetScissor(l_X, l_Y, l_SizeX, l_SizeY);

    NoFog();
    NoOmnis();

    GXFogAdjTable l_FogAdjTable;
    GXInitFogAdjTable(&l_FogAdjTable, (U16)l_SizeX, *(Mtx44*)&m_ProjMatrix);

    S32 l_FogCenter = l_X + (l_SizeX / 2);
    GXSetFogRangeAdj(TRUE, (U16)l_FogCenter, &l_FogAdjTable);

    m_CurDrawOrderGroup = do_null;
    PushDo(do_none);

    m_CurDrawState = 0xff70;
    DrawState(ds_opaque);

    m_CurMtxKey = -1;

    Renderer_Z::Draw(i_ViewportId, i_DeltaTime);
}

void GCRenderer_Z::RestoreViewport() {
    S32 l_X;
    S32 l_Y;
    S32 l_Width;
    S32 l_Height;

    if (m_ActiveViewport != NONE_VIEWPORT) {
        m_Viewports[m_ActiveViewport].GetPosAndSize(l_X, l_Y, l_Width, l_Height);
    }
    else {
        l_X = 0;
        l_Y = 0;
        l_Width = m_SizeX;
        l_Height = m_SizeY;
    }

    GXSetProjection(*(Mtx44*)&m_ProjMatrix, GX_PERSPECTIVE);

    GXSetViewport(
        (Float)l_X,
        (Float)l_Y,
        (Float)l_Width,
        (Float)l_Height,
        0.0f,
        1.0f
    );

    GXSetScissor(l_X, l_Y, l_Width, l_Height);
}

void GCRenderer_Z::SetLocal2Cam(const Mat4x4& i_Local2Cam, U32 i_GXMtxId) {
    if (i_GXMtxId == 0x3c) {
        GXSetCurrentMtx(0x3c);
    }
    else {
        Mtx l_Local2Cam Aligned_Z(32);
        ConvertMtx_Z(i_Local2Cam, l_Local2Cam);
        Reverse_Z(l_Local2Cam);

        PSMTXCopy(l_Local2Cam, *(Mtx44*)&m_Local2Cam);

        GXLoadPosMtxImm(l_Local2Cam, GX_PNMTX0);
        GXLoadTexMtxImm(l_Local2Cam, GX_TEXMTX1, GX_MTX2x4);

        PSMTXInvXpose(l_Local2Cam, *(Mtx44*)&m_InvTransposed);
        GXLoadNrmMtxImm(*(Mtx44*)&m_InvTransposed, GX_PNMTX0);

        GXSetCurrentMtx(GX_PNMTX0);
    }

    m_CurMtxId = i_GXMtxId;
    m_CurMtxKey++;
}

void GCRenderer_Z::SetWorldToCam(const Mat4x4& i_World2Cam) {
    m_CameraMatrix = i_World2Cam;
    m_CameraMatrix.ReverseAxis();
}

void GCRenderer_Z::SetActiveMaterial(Material_Z* i_Material) {
    if (!i_Material) {
        SetBlankMaterial();
        return;
    }
    m_CurObjColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(i_Material, GX_COLOR0A0);
}

void GCRenderer_Z::SetMaterial(Material_Z* i_Material, GXChannelID i_Channel) {
    Mtx l_UVTransform;
    U32 l_RdrFlag = i_Material->GetRenderFlag();

    if (i_Material != m_ActiveMaterial) {

        Bitmap_ZHdl* l_BitmapHdl;

        if (gData.m_EngineFlag & FL_NO_TEXTURE) {
            l_BitmapHdl = &gData.SystemDatas->GetWhiteBitmap();
        }
        else {
            l_BitmapHdl = &i_Material->GetBitmap(BITMAP_DIFFUSE);
        }

        Bitmap_Z* l_Bitmap = *l_BitmapHdl;
        m_ActiveBitmaps[BITMAP_DIFFUSE] = l_Bitmap;

        GXTexWrapMode l_WrapT = (l_RdrFlag & FL_TEX_ADDRESS_CLAMP_V) ? GX_CLAMP : GX_REPEAT;
        GXTexWrapMode l_WrapS = (l_RdrFlag & FL_TEX_ADDRESS_CLAMP_U) ? GX_CLAMP : GX_REPEAT;

        SetTexture(l_Bitmap, l_WrapS, l_WrapT, GX_TEXMAP0);

        ConvertMtx33_Z(i_Material->GetUVTransform(), l_UVTransform);

        DCStoreRange(l_UVTransform, sizeof(l_UVTransform));
        GXLoadTexMtxImm(l_UVTransform, GX_TEXMTX0, GX_MTX3x4);

        m_ActiveMaterial = i_Material;
        m_ActiveMaterialTextureFlag = m_ActiveMaterial->GetFlag() & FL_TEX_ALL;
    }

    SetRenderBlendOp(l_RdrFlag);

    Float l_DiffuseR = m_CurObjColor.x * i_Material->GetDiffuse().x;
    Float l_DiffuseG = m_CurObjColor.y * i_Material->GetDiffuse().y;
    Float l_DiffuseB = m_CurObjColor.z * i_Material->GetDiffuse().z;
    Float l_DiffuseA = m_CurObjColor.w * i_Material->GetOpacity();

    GXColor l_Color;
    l_Color.r = (U8)(255.0f * l_DiffuseR);
    l_Color.g = (U8)(255.0f * l_DiffuseG);
    l_Color.b = (U8)(255.0f * l_DiffuseB);
    l_Color.a = (U8)(255.0f * l_DiffuseA);

    GXSetChanMatColor(i_Channel, l_Color);

    Float l_EmissiveR = m_CurObjColor.x * i_Material->GetEmissive().x;
    Float l_EmissiveG = m_CurObjColor.y * i_Material->GetEmissive().y;
    Float l_EmissiveB = m_CurObjColor.z * i_Material->GetEmissive().z;

    l_Color.a = 0xff;
    l_Color.r = (U8)(255.0f * l_EmissiveR);
    l_Color.g = (U8)(255.0f * l_EmissiveG);
    l_Color.b = (U8)(255.0f * l_EmissiveB);

    GXSetTevColor((GXTevRegID)1, l_Color);

    if (l_RdrFlag & FL_ADDITIF) {
        DisableFog();
    }
    else {
        EnableFog();
    }
}

void GCRenderer_Z::SetRenderBlendOp(U32 i_Flag) {
    if (m_CurBlendFlags == (i_Flag & FL_RDR_BLEND_MASK)) {
        return;
    }

    switch (i_Flag & (FL_IS_ALPHABLENDED | FL_IS_TRANSPARENT | FL_ADDITIF | FL_SOUSTRACTIF | FL_DESTADDITIF)) {
        case FL_IS_ALPHABLENDED | FL_ADDITIF:
        case FL_ADDITIF:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
            GXSetZCompLoc(TRUE);
            break;

        case FL_DESTADDITIF:
        case FL_IS_ALPHABLENDED | FL_DESTADDITIF:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_CLEAR);
            GXSetZCompLoc(TRUE);
            break;

        case FL_SOUSTRACTIF:
        case FL_IS_ALPHABLENDED | FL_SOUSTRACTIF:
            GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
            GXSetZCompLoc(TRUE);
            break;

        case FL_IS_ALPHABLENDED:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetZCompLoc(TRUE);
            break;

        case FL_IS_TRANSPARENT:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(FALSE);
            break;

        case FL_IS_ALPHABLENDED | FL_IS_TRANSPARENT:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetZCompLoc(FALSE);
            break;

        default:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(TRUE);
            break;
    }

    if (i_Flag & FL_TWO_SIDE) {
        DrawState(m_CurDrawState & ~(ds_cw | ds_ccw));
    }
    else {
        DrawState(m_CurDrawState | ds_cw);
    }

    m_CurBlendFlags = i_Flag & FL_RDR_BLEND_MASK;
}

void GCRenderer_Z::DrawState(U16 i_StateFlag) {
    if (m_CurDrawState == i_StateFlag) {
        return;
    }

    U32 l_NewState = m_CurDrawState ^ i_StateFlag;

    if (l_NewState & ds_ztestwrite) {
        Bool l_ZWrite = (i_StateFlag & ds_zwrite) == ds_zwrite;
        GXCompare l_CmpFunc = (i_StateFlag & ds_ztest) ? GX_LEQUAL : GX_ALWAYS;
        GXSetZMode(TRUE, l_CmpFunc, l_ZWrite);
    }

    if (l_NewState & ds_alpha) {
        GXSetAlphaCompare(
            (i_StateFlag & ds_noatest) ? GX_ALWAYS : GX_GREATER,
            (i_StateFlag & ds_aref128) ? 128 : 0,
            GX_AOP_OR,
            GX_NEVER,
            0
        );
    }

    if (l_NewState & ds_disable_forced_dstalpha) {
        if (i_StateFlag & ds_disable_forced_dstalpha) {
            GXSetDstAlpha(FALSE, 0);
        }
        else {
            GXSetDstAlpha(TRUE, 127);
        }
    }

    if (l_NewState & ds_cull_order) {
        if (i_StateFlag & ds_cw) {
            GXSetCullMode(GX_CULL_BACK);
        }
        else if (i_StateFlag & ds_ccw) {
            GXSetCullMode(GX_CULL_FRONT);
        }
        else {
            GXSetCullMode(GX_CULL_NONE);
        }
    }

    if (l_NewState & ds_cwrite) {
        GXSetColorUpdate((i_StateFlag & ds_cwritergb) == ds_cwritergb);
        GXSetAlphaUpdate((i_StateFlag & ds_cwritea) == ds_cwritea);
    }

    m_CurDrawState = i_StateFlag;
}

void GCRenderer_Z::SetRenderContext(U32 i_Flag) {
    Bitmap_Z* l_LightmapTexture = m_ActiveBitmaps[BITMAP_RADIOSITY];

    U32 l_OmniMask = m_CurOmniLightMask | GX_LIGHT0;
    U32 l_HasLightmapTextureFlag = l_LightmapTexture ? FL_RDR_CONTEXT_HAS_LIGHTMAP_TEXTURE : 0;
    U32 l_HasMaterialTextureFlag = (m_ActiveMaterialTextureFlag & 1) ? FL_RDR_CONTEXT_MATERIAL_TEXTURE : 0;

    U32 l_NewRenderContextFlag = i_Flag | l_OmniMask | l_HasLightmapTextureFlag | l_HasMaterialTextureFlag;

    if (l_NewRenderContextFlag == m_RenderContextFlag) {
        return;
    }

    m_RenderContextFlag = l_NewRenderContextFlag;

    U32 l_Mode = i_Flag & FL_RDR_CONTEXT_MODE_MASK;
    U32 l_TexGenCount = 0;
    S32 l_LastStage;

    switch (l_Mode) {
        case FL_RDR_CONTEXT_LIT_TEX_A:
        case FL_RDR_CONTEXT_LIT_TEX_B: {
            l_TexGenCount++;

            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_TEXMTX0);
            GXSetNumChans(2);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_COLOR1A1, TRUE, GX_SRC_REG, GX_SRC_REG, l_OmniMask, GX_DF_CLAMP, GX_AF_SPOT);

            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            l_LastStage = 2;

            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            if (l_HasLightmapTextureFlag) {
                l_TexGenCount++;
                GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);
                l_LastStage = 3;
                GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            }

            GXSetNumTevStages(l_LastStage + 1);
            break;
        }

        case FL_RDR_CONTEXT_LIT_TEX_KALPHA_A: {
            l_TexGenCount++;

            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_TEXMTX0);
            GXSetNumChans(2);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_COLOR1A1, TRUE, GX_SRC_REG, GX_SRC_REG, l_OmniMask, GX_DF_CLAMP, GX_AF_SPOT);

            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            l_LastStage = 2;

            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXColor l_White;
            l_White.r = l_White.g = l_White.b = l_White.a = 0xff;

            GXSetTevColor(GX_TEVREG2, l_White);

            if (l_HasLightmapTextureFlag) {
                l_TexGenCount++;
                GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);
                l_LastStage = 3;
                GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            }

            GXSetNumTevStages(l_LastStage + 1);
            break;
        }

        case FL_RDR_CONTEXT_LIT_TEX_SIMPLE_A: {
            l_TexGenCount++;

            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_TEXMTX0);
            GXSetNumChans(2);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_COLOR1A1, TRUE, GX_SRC_REG, GX_SRC_REG, l_OmniMask, GX_DF_CLAMP, GX_AF_SPOT);

            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            l_LastStage = 1;

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            if (l_HasLightmapTextureFlag) {
                l_TexGenCount++;
                GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);
                l_LastStage = 2;
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            }

            GXSetNumTevStages(l_LastStage + 1);
            break;
        }

        case FL_RDR_CONTEXT_LIT_TEX_KALPHA_B: {
            l_TexGenCount++;

            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_TEXMTX0);
            GXSetNumChans(2);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_COLOR1A1, TRUE, GX_SRC_REG, GX_SRC_REG, l_OmniMask, GX_DF_CLAMP, GX_AF_SPOT);

            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            l_LastStage = 1;

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXColor l_Color;
            l_Color.r = l_Color.g = l_Color.b = l_Color.a = 255;
            GXSetTevColor(GX_TEVREG1, l_Color);

            if (l_HasLightmapTextureFlag) {
                l_TexGenCount++;
                GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);
                l_LastStage = 2;
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            }

            GXSetNumTevStages(l_LastStage + 1);
            break;
        }

        case FL_RDR_CONTEXT_TEX_MODULATE:
            l_TexGenCount++;
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_IDENTITY);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            break;

        case FL_RDR_CONTEXT_TEX_MODULATE_KALPHA:
            l_TexGenCount++;
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_IDENTITY);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_KONST, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            break;

        case FL_RDR_CONTEXT_TEX_ALPHA_CONST:
            l_TexGenCount++;
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_TEXMTX0);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(2);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_ONE, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            break;

        case FL_RDR_CONTEXT_FLAT_COLOR:
            l_TexGenCount = 0;
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C1);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A1);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            break;

        case FL_RDR_CONTEXT_BLOOM: {
            l_TexGenCount++;

            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_IDENTITY, FALSE, GX_PTIDENTITY);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(3);

            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_KONST, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C2, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            Material_ZHdl& l_BloomMaterialHdl = gData.SystemDatas->GetBloomMaterial();
            Vec3f& l_Color = l_BloomMaterialHdl->GetDiffuse();
            Float l_Opacity = l_BloomMaterialHdl->GetOpacity();

            GXColor l_GXColor;
            l_GXColor.r = (U8)(255.0f * l_Color.x);
            l_GXColor.g = (U8)(255.0f * l_Color.y);
            l_GXColor.b = (U8)(255.0f * l_Color.z);
            l_GXColor.a = 0xff;
            GXSetTevColor(GX_TEVREG1, l_GXColor);

            U8 l_GXOpacity = (U8)(128.0f * l_Opacity);
            l_GXColor.r = l_GXColor.g = l_GXColor.b = l_GXOpacity;
            GXSetTevColor(GX_TEVREG2, l_GXColor);
            break;
        }

        case FL_RDR_CONTEXT_LOW_INTENSITY_CONST: {
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_C1, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_A1, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

            GXColor l_Color;
            l_Color.r = l_Color.g = l_Color.b = l_Color.a = 4;
            GXSetTevColor(GX_TEVREG1, l_Color);
            break;
        }

        case FL_RDR_CONTEXT_VERTEX_COLOR_TEX_ALPHA:
            l_TexGenCount++;
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEXCOORD0, GX_IDENTITY, FALSE, GX_PTIDENTITY);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
            break;

        case FL_RDR_CONTEXT_4TEXGEN_ONLY:
            l_TexGenCount = 4;
            break;

        default:
            ASSERTLE_Z(FALSE, "", 1102, "(Bool) (0==1)");
            break;
    }

    GXSetNumTexGens(l_TexGenCount);
}

void GCRenderer_Z::SetActiveTexture(Bitmap_Z* i_Bitmap, S32 i_TextureType) {
    m_ActiveBitmaps[i_TextureType] = i_Bitmap;
    SetTexture(i_Bitmap, GX_REPEAT, GX_REPEAT);
}

void GCRenderer_Z::SetTexture(Bitmap_Z* i_Bitmap, GXTexWrapMode i_WrapS, GXTexWrapMode i_WrapT, GXTexMapID i_TexMapID) {
    if (!i_Bitmap) {
        return;
    }
    gData.TextureMgr->LoadTexture(i_Bitmap, i_WrapS, i_WrapT, i_TexMapID);
}

void GCRenderer_Z::FreeTexture(S16 i_TexId) {
    gData.TextureMgr->RemoveTexture(i_TexId);
}

U32 GCRenderer_Z::GetTextureSize() {
    return 0;
}

void GCRenderer_Z::MakeScreenShot(Char* i_FilePath) { }

void GCRenderer_Z::SetProfiler(Bool i_Enable) { }

Bool GCRenderer_Z::Minimize() {
    gData.TextureMgr->Minimize();
    m_DrawCalls.Minimize();
    m_Draw2D.Minimize();
    return TRUE;
}
