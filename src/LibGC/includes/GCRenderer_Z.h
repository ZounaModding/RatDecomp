#ifndef _GCRENDERER_Z_H_
#define _GCRENDERER_Z_H_
#include "Renderer_Z.h"
#include "BnkDynArray_Z.h"
#include "ARamXAllocator_Z.h"
#include "DisplayList_Z.h"
#include "StreamList_Z.h"
#include <gx.h>
#include <mtx.h>

#define MAX_OMNI 3
#define FIFO_BUFFER_SIZE (380 * 1024)
#define GC_BUFFER_ALIGN 32
#define VIZ_QUERY_DISPLAY_LIST_SIZE 32

#define FL_RDR_CONTEXT_NONE (0 << 0) // 0x0 - No flags

#define FL_RDR_CONTEXT_LIT_TEX_A (1 << 8)               // 0x100 - Lit textured pass, same setup as 0x400
#define FL_RDR_CONTEXT_LIT_TEX_KALPHA_A (1 << 9)        // 0x200 - Lit textured pass, alpha uses KONST
#define FL_RDR_CONTEXT_LIT_TEX_B (1 << 10)              // 0x400 - Lit textured pass, same setup as 0x100
#define FL_RDR_CONTEXT_LIT_TEX_SIMPLE_A (1 << 11)       // 0x800 - Lit textured pass, fewer TEV stages
#define FL_RDR_CONTEXT_LIT_TEX_KALPHA_B (1 << 12)       // 0x1000 - Lit textured pass, alpha uses KONST
#define FL_RDR_CONTEXT_TEX_MODULATE (1 << 13)           // 0x2000 - Single texture, raster color, texture alpha
#define FL_RDR_CONTEXT_TEX_MODULATE_KALPHA (1 << 14)    // 0x4000 - Single texture, raster color, KONST alpha
#define FL_RDR_CONTEXT_FLAT_COLOR (1 << 15)             // 0x8000 - No texgen, flat/channel color
#define FL_RDR_CONTEXT_TEX_ALPHA_CONST (1 << 16)        // 0x10000 - Textured pass then const/color combine
#define FL_RDR_CONTEXT_BLOOM (1 << 17)                  // 0x20000 - Bloom material pass
#define FL_RDR_CONTEXT_LOW_INTENSITY_CONST (1 << 18)    // 0x40000 - Constant 0x04040404 TEV color
#define FL_RDR_CONTEXT_VERTEX_COLOR_TEX_ALPHA (1 << 19) // 0x80000 - Raster color with texture alpha
#define FL_RDR_CONTEXT_4TEXGEN_ONLY (1 << 20)           // 0x100000 - Only sets GXSetNumTexGens(4)

#define FL_RDR_CONTEXT_HAS_LIGHTMAP_TEXTURE (1 << 21) // 0x200000 - Cache bit, m_ActiveBitmaps[BITMAP_RADIOSITY] != NULL
#define FL_RDR_CONTEXT_MATERIAL_TEXTURE (1 << 22)     // 0x400000 - Cache bit, m_ActiveMaterialTextureFlag bit 0

#define FL_RDR_CONTEXT_LIGHT_MASK 0xFF         // 0xFF - GX_LIGHT0..GX_LIGHT7
#define FL_RDR_CONTEXT_MODE_MASK 0x1FFFFF      // 0x1FFFFF - Input mode mask, includes GX light bits
#define FL_RDR_CONTEXT_MODE_ONLY_MASK 0x1FFF00 // 0x1FFF00 - Engine render-context mode bits
#define FL_RDR_CONTEXT_CACHE_MASK 0x7FFFFF     // 0x7FFFFF - Mode plus cache-only texture bits

// $SABE: Taken from MonopolyX360SUB.xdb and adapted, could be wrong (For more values see the file)
enum DrawingOrder {
    do_null = 0,
    do_none = 1,
    do_opaque = 2,
    do_unk_3 = 3,
    do_unk_4 = 4,
    do_unk_5 = 5,
    do_unk_6 = 6,
    do_unk_7 = 7,
    do_unk_8 = 8,
    do_shadow_cast = 9,
    do_scene_draw = 10,
    do_transp_first = 11,
    do_transp = 12,
    do_unk_13 = 13,
    do_unk_14 = 14,
    do_flare = 15,
    do_unk_16 = 16,
    do_unk_17 = 17,
    do_unk_18 = 18,
    do_last = 19,
    do_count = 20
};

// $SABE: Taken from MonopolyX360SUB.xdb and adapted, could be wrong (For more values see the file)
enum DrawingState {
    ds_ztest = 0x1,
    ds_zwrite = 0x2,
    ds_cwritergb = 0x4,
    ds_cwritea = 0x8,
    ds_ablend = 0x10,
    ds_noatest = 0x20,
    ds_aref128 = 0x40,
    ds_cw = 0x80,
    ds_ccw = 0x100,
    ds_unk_0x200 = 0x200,
    ds_unk_0x400 = 0x400,
    ds_disable_forced_dstalpha = 0x800,
    ds_ztestwrite = ds_ztest | ds_zwrite,                                   // 0x3
    ds_cwrite = ds_cwritergb | ds_cwritea,                                  // 0xc
    ds_alpha = ds_ablend | ds_noatest | ds_aref128,                         // 0x70
    ds_zonly = ds_ztest | ds_zwrite | ds_cw,                                // 0x83
    ds_opaque_no_alpha_write = ds_ztest | ds_zwrite | ds_cwritergb | ds_cw, // 0x87
    ds_opaque = ds_ztest | ds_zwrite | ds_cwrite | ds_cw,                   // 0x8f
    ds_cull_order = ds_cw | ds_ccw,                                         // 0x180
};

// TODO: Move these to their own files
#define DRAW2D_VTXBUFFER_NB 32
#define DRAW2D_VERTEX_LIST_ALIGN 32

class Draw2D_Z {
public:
    struct GCVertex2DStream {
        Vec3f m_Pos;
        GXColor m_Color;
        Vec2f m_UV;
    };

    class GCListVertex2D {
    public:
        U8 m_PrimType;
        U8 m_VertexCountHigh;
        U8 m_VertexCountLow;
        U8 m_VertexData[DRAW2D_VTXBUFFER_NB * sizeof(GCVertex2DStream)];
    } Aligned_Z(32);

    class Primitive2DList_Z {
    public:
        Material_Z* m_Material;
        S32 m_DisplayListIdx;
        U16 m_DrawState;
        U32 m_RenderFlags;
    };

    void Init();
    void Shut();
    void Begin();
    void End();
    void Minimize();

    U8* m_CurWritePtr;
    U8* m_CurWriteEnd;
    U8* m_PreviousWritePtr;
    Material_Z* m_CurMaterial;
    U16 m_CurDrawState;
    U32 m_CurRenderFlags;
    S32 m_CurDisplayListIdx;
    S32 m_RemainingVertexCount;
    BnkDynArray_Z<GCListVertex2D, 16, FALSE, FALSE, DRAW2D_VERTEX_LIST_ALIGN> m_Vtx2DBufferDA;
    DynArray_Z<Primitive2DList_Z, 8, TRUE, TRUE, 4> m_PrimitiveLists;
};

// END TODO

// TODO: Move these to their own files
#define DRAW3D_VTXBUFFER_NB 128
#define DRAW3D_VERTEX_LIST_ALIGN 32

class Draw3D_Z {
public:
    struct GCVertex3DStream {
        Vec3f m_Pos;
        GXColor m_Color;
        Vec2f m_UV;
    };

    class GCDisplayListVertex3D : public BaseDisplayList_Z {
    public:
        U8 m_Unk_0x04[0x1c];
        U8 m_PrimType;
        U8 m_VertexCountHigh;
        U8 m_VertexCountLow;
        U8 m_VertexData[DRAW3D_VTXBUFFER_NB * sizeof(GCVertex3DStream)];
    } Aligned_Z(32);

    void End();
    void EndRender();

    void SwitchBuffer() {
        m_CurBankIdx = 1 - m_CurBankIdx;
        m_CurBankIdx = 1 - m_CurBankIdx;
    }

    S32 m_CurBankIdx;
    BaseDisplayList_Z* m_CurDisplayList;
    BnkDynArray_Z<GCDisplayListVertex3D, 16, FALSE, TRUE, DRAW3D_VERTEX_LIST_ALIGN> m_Vtx3DBufferDA[2];
};

// END TODO

// TODO: Move these to their own files
struct ScanCode_Z {
    Float m_U1;
    Float m_V1;
    Float m_U2;
    Float m_V2;
    Float m_SizeX;
    Float m_SizeY;
    Bool m_Valid;

    ScanCode_Z() {
    }
};

struct FontString_Z {
    Material_ZHdl m_MaterialHdl;
    ScanCode_Z m_Characters[256];

    void MarkHandles();
    void Init();
};

// END TODO

#define EXT_PRIM_INFO_ALIGN 32

struct ExtPrimitiveInfo_Z {
    Mtx m_Local2Cam Aligned_Z(32);
    U32 m_MtxId;
    U32 m_MtxKey;
    U32 m_RenderContextFlag;
    U32 m_BlendFlag;
    Mtx m_InvTransposed Aligned_Z(32);
    U32 m_Order;
    U8 m_DrawOrderGroup;
    U16 m_DrawState;
    U8 m_UnusedSortKeyInsideGroup_0x78;
    StreamList_Z* m_StreamList;
    BaseDisplayList_Z* m_DisplayList;
    Material_Z* m_Material;
    GXLightObj m_DirectionalLight;
    GXColor m_AmbientColor;
    Vec3f m_ObjDatasColor;
    Float m_FadeValue;
    U32 m_OmniLightCount;
    U32 m_OmniLightMask;
    GXLightObj m_OmniLights[MAX_OMNI];
    HFogData_Z* m_MainFog;
    HFogData_Z* m_EnabledFog;
    Float m_FogNear;
    Float m_FogFar;
    Float m_FogStartZ;
    Float m_FogEndZ;
    Bitmap_Z* m_ActiveBitmaps[BITMAP_NB_TYPES];
    S32 m_PrevDrawCallIdx;
    S32 m_NextDrawCallIdx;
};

typedef DynArray_Z<ExtPrimitiveInfo_Z, 256, FALSE, FALSE, EXT_PRIM_INFO_ALIGN> ExtPrimitiveInfo_ZDA;

#define INVALID_SORT_ELEM_IDX 0xFFFF

// Radix sort element
struct SortElem_Z {
    U32 m_Key; // Radix sorted by (high to low priority) (m_DrawOrderGroup << 24) | (m_UnusedSortKeyInsideGroup_0x78 << 16) | (((U8)m_Order >> 8) << 8) | (U8)m_OrderLowByte;
    U16 m_NextElemIdx;
};

class GCRenderer_Z : public Renderer_Z {
public:
    // TODO: Finish matching
    GCRenderer_Z() {
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
    }

    virtual ~GCRenderer_Z() {
        Shut();
    }

    virtual Bool Init(S32 i_SizeX, S32 i_SizeY);
    virtual void Shut();
    virtual void Reset();
    virtual void BeginRender();
    virtual void EndRender(Float i_DeltaTime);
    virtual Bool Minimize();
    virtual void Draw(S32 i_ViewportId, Float i_DeltaTime);

    virtual void ClearZBuffer(S32 i_X, S32 i_Y, S32 i_Width, S32 i_Height);

    virtual void ClearFrameBuffer(S32 a1, S32 a2, S32 a3, S32 a4) { }

    virtual void PushOrder(Float i_Order) {
        m_PushedOrder = i_Order;
    }

    virtual void PushDo(U8 i_DrawOrderGroup) {
        m_PushedDrawOrderGroup = i_DrawOrderGroup;
    }

    virtual void PushDs(U16 i_DrawState) {
        m_PushedDrawState = i_DrawState;
    }

    virtual void SetActiveMaterial(Material_Z* i_Material);
    virtual void SetActiveTexture(Bitmap_Z* i_Bitmap, S32 i_Unk);
    virtual void FreeTexture(S16 i_TexId);
    virtual U32 GetTextureSize();
    virtual void MarkHandles();
    virtual void DrawFace(Vertex3D& a1, Vertex3D& a2, Vertex3D& a3);
    virtual void DrawStrip(Vertex3D* a1, U32 a2, Bool a3);
    virtual void DrawStripAfterScene(Vertex3D* a1, U32 a2, Bool a3);
    virtual void DrawLine(const Vec3f& a1, const Vec3f& a2, const Color& a3, Bool a4, Bool a5);
    virtual void DrawLine(const Vec2f& a1, const Vec2f& a2, const Color& a3, Float a4);
    virtual void DrawImage(Bitmap_ZHdl& a1);
    virtual void DrawCross(const Vec3f& a1, const Color& a2, Float a3);
    virtual void Draw2DQuad(const Vec2f& a1, const Vec2f& a2, const Vec2f& a3, const Vec2f& a4, const Color& a5, const Color& a6, Float a7);
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Color& a4, Float a5);
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Float a4);
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Vec2f& a3, Vec2f& a4, Vec3f& a5, Float a6);
    virtual void Draw2DQuad(Vec2f* a1, Vec3f* a2, Vec2f* a3, Float a4, Float a5);
    virtual void DrawStrip(Vec2f* a1, S32 a2, const Color& a3, Float a4);
    virtual void DrawFan(Vec2f* a1, S32 a2, const Color& a3, Float a4);
    virtual void DrawString(const Vec2f& a1, const Char* a2, const Color& a3, Float a4, Float a5);
    virtual void DrawString(const Vec3f& a1, const Char* a2, Bool a3);
    virtual void DrawString(const Vec3f& a1, const Char* a2, const Color& a3, Bool a4);
    virtual void MakeScreenShot(Char* i_FilePath);
    virtual void FlushActiveViewport();
    virtual void InitRenderStates();
    virtual void SetProfiler(Bool i_Enable);
    virtual void PushADraw(StreamList_Z* a1, BaseDisplayList_Z* a2, S32 a3);

    static void ClearAFrameBuffer(U8* i_Buffer, S32 i_UnkS32);
    void RestoreViewport();
    void SetLocal2Cam(const Mat4x4& i_Local2Cam, U32 i_GXMtxId);
    void SetWorldToCam(const Mat4x4&);
    void SetMaterial(Material_Z* i_Material, GXChannelID i_Channel);
    void SetRenderBlendOp(U32 i_BlendFlag);
    void DrawState(U16 i_StateFlag);
    void SetRenderContext(U32 i_ContextFlag);
    void SetTexture(Bitmap_Z* i_Bitmap, GXTexWrapMode i_WrapS, GXTexWrapMode i_WrapT, GXTexMapID i_TexMapID = GX_TEXMAP0);

    // GCRendererTransp_Z.cpp

    Bool DrawExtPrimitive(ExtPrimitiveInfo_Z* i_ExtPrimInfo);
    U16 SortRendererDatas(SortElem_Z* i_SortElems);
    virtual void DrawTransparent(DrawInfo_Z& i_DrawInfo);
    void DrawOrder(DrawInfo_Z& i_DrawInfo, unsigned char i_Order);
    void ImmediatQuad(const Vec2f& i_UVMin, const Vec2f& i_UVMax, const Vec2f& i_PosMin, const Vec2f& i_PosMax, const Vec2f& i_Size, const Color& i_Color, Float i_Z);
    void InitBlock(DrawInfo_Z& i_DrawInfo);

    // GCRendererLighting_Z.cpp

    void DisableFog();
    void EnableFog();
    void NoFog();
    void NoOmnis();

    // void ResetContextFlag() {
    //     m_VizQueryDisplayListCount = 0;
    // }

    // void ResetOther() {
    //     m_RenderContextFlag = -1;
    //     m_VizQueryDisplayListCount = 0;
    //     for (S32 i = 0; i < 20; i++) {
    //         m_DrawOrderGroupDrawCallCount[i] = 0;
    //     }
    // }

private:
    GXFifoObj* m_FifoObject;
    U8* m_FifoBuffer; // Size of 380 * 1024
    U8* m_FrameBuffers[2];
    U8* m_CurFrameBuffer;
    U32 m_FrameBufferIdx;
    GXRenderModeObj m_RenderModeObj;
    S32 m_CurBlendFlags;
    Float m_PushedOrder;
    U8 m_PushedDrawOrderGroup;
    U8 m_CurDrawOrderGroup;
    U16 m_PushedDrawState;
    U16 m_CurDrawState;
    U8 m_PushedUnusedSortKeyInsideGroup_0x2672;
    U8 m_UnkU8_SetTo0_0x2673;
    S32 m_DrawOrderGroupDrawCallCount[do_count];
    Bool m_DrawOrderGroupShouldDraw[do_count];
    ExtPrimitiveInfo_ZDA m_DrawCalls;
    U32 m_FrameBufferTextureSize;
    U8* m_FrameBufferTextureData;
    GXTexObj m_FrameBufferTexObj;
    S32 m_VizQueryDisplayListCount;
    DisplayList_Z m_VizQueryDisplayLists[VIZ_QUERY_DISPLAY_LIST_SIZE];
    GXLightObj m_CurDirectionalLight;
    GXColor m_CurAmbientColor;
    Vec4f m_CurObjColor;
    S32 m_CurOmniLightCount;
    U32 m_CurOmniLightMask;
    GXLightObj m_CurOmniLights[MAX_OMNI];
    HFogData_Z* m_CurMainFog;
    HFogData_Z* m_CurEnabledFog;
    Float m_FogNear;
    Float m_FogFar;
    Float m_FogStartZ;
    Float m_FogEndZ;
    S32 m_RenderContextFlag;
    U32 m_ActiveMaterialTextureFlag; // Max FL_TEX_ALL (all bitmaps)
    U32 m_CurMtxId;
    S32 m_CurMtxKey;
    Mat4x4 m_Local2Cam Aligned_Z(64);
    Mat4x4 m_InvTransposed Aligned_Z(64);
    Mat4x4 m_CameraMatrix Aligned_Z(64);
    Mat4x4 m_ProjMatrix Aligned_Z(64);
    Mat4x4 m_OrthoMatrix Aligned_Z(64);
    Bool m_SkipFrameBufferEffects;
    Bool m_CopyFrameBufferAlpha;
    FontString_Z m_FontString;
    Draw2D_Z m_Draw2D;
    Draw3D_Z m_Draw3D;
    ARamXAllocator_Z<256, 32, 262144> m_ARamAllocator;
};

#endif // _GCRENDERER_Z_H_
