#include "DCRenderer_Z.h"
#include "Math_Z.h"
#include "Timer_Z.h"
#include "Bitmap_Z.h"
#include "Material_Z.h"
#include <dc/pvr.h>

static void PvrBlendFromRenderFlags(U32 i_RenderFlags, pvr_blend_mode_t& o_Src, pvr_blend_mode_t& o_Dst);
static pvr_depthcmp_mode_t PvrDepthCmpFromDrawState(U16 i_DrawState);
static pvr_cull_mode_t PvrCullFromDrawState(U16 i_DrawState, U32 i_RenderFlags);
static pvr_uv_clamp_t PvrUvClampFromRenderFlags(U32 i_RenderFlags);
static S32 PvrFormatFromBitmap(Bitmap_Z* i_Bitmap);
static pvr_filter_mode_t PvrFilterFromBitmap(Bitmap_Z* i_Bitmap);

static VU32 s_VblankCount;

static void DCVblankHandler(uint32_t i_Code, void* i_Data) {
    s_VblankCount++;
}

// Init

Bool DCRenderer_Z::Init(S32 i_SizeX, S32 i_SizeY) {
    if (!Renderer_Z::Init(i_SizeX, i_SizeY)) {
        return FALSE;
    }

    m_PvrParams = {
        .opb_sizes = {
            PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_8, PVR_BINSIZE_0, PVR_BINSIZE_8 },
        .vertex_buf_size = (1024 + 1024) * 1024,
        .autosort_disabled = FALSE,
        .opb_overflow_count = 7,
    };

    vid_set_mode(DM_640x480, PM_RGB565);
    pvr_init(&m_PvrParams);
#ifdef DEBUG_Z
    SetPvrBgColor(COLOR_ZOUNA);
#else
    SetPvrBgColor(COLOR_BLACK);
#endif

    Renderer_ZDefaultNear = DC_DEFAULT_NEAR;
    Renderer_ZCurrentNear = DC_DEFAULT_NEAR;

    m_DebugFontString.Init();

    vblank_handler_add(DCVblankHandler, NULL);

    return TRUE;
}

// Update

void DCRenderer_Z::BeginRender() {
    Renderer_Z::BeginRender();

    m_RetraceCount = s_VblankCount;

    S32 i = m_TextureHA.FindFirst();
    while (i != -1) {
        m_TextureHA[i].m_Used = FALSE;
        i = m_TextureHA.FindNext(i);
    }

    m_Draw2D.Begin();
}

void DCRenderer_Z::EndRender(Float i_DeltaTime) {
    Renderer_Z::EndRender(i_DeltaTime);

    UpdatePvrBgColor(GetAbsoluteTime());

    pvr_wait_ready();
    pvr_scene_begin();

    DrawOpaque();
    DrawPunchthrough();
    DrawTransparent();

    //m_Draw2D.LogStats();

    pvr_scene_finish();
}

void DCRenderer_Z::WaitForRetrace(Float i_DeltaTime) {
    U32 l_TargetDelta = (gData.m_GameFlag & FL_GAME_2_FRAMES) ? 2 : 1;
    while (s_VblankCount - m_RetraceCount < l_TargetDelta) {
        vid_waitvbl();
    }
}

// Draw the opaque geometry here
void DCRenderer_Z::DrawOpaque() {
    pvr_list_begin(PVR_LIST_OP_POLY);

    m_Draw2D.EndOpaque();

    pvr_list_finish();
}

// Draw the punchthrough geometry here
void DCRenderer_Z::DrawPunchthrough() {
    PVR_SET(PVR_PT_ALPHA_REF, DC_PT_ALPHA_REF);

    pvr_list_begin(PVR_LIST_PT_POLY);

    m_Draw2D.EndPunchThrough();

    pvr_list_finish();
}

// Draw the transparent geometry here
void DCRenderer_Z::DrawTransparent() {
    pvr_list_begin(PVR_LIST_TR_POLY);

    m_Draw2D.EndTransparent();

    pvr_list_finish();
}

void DCRenderer_Z::SetActiveMaterial(Material_Z* i_Material) {
    Renderer_Z::SetActiveMaterial(i_Material);
}

void DCRenderer_Z::MarkHandles() {
    m_DebugFontString.MarkHandles();
}

DCTexture_Z* DCRenderer_Z::LoadTexture(Bitmap_Z* i_Bitmap) {
    DCTexture_Z* l_Texture;
    S16 l_TexId = i_Bitmap->GetTexId();
    if (l_TexId == INVALID_TEXID) {
        S32 l_SizeX = i_Bitmap->GetSizeX();
        S32 l_SizeY = i_Bitmap->GetSizeY();
        void* l_Ptr = pvr_mem_malloc(l_SizeX * l_SizeY * 2);
        ASSERT_Z(l_Ptr, "DCRenderer_Z::LoadTexture: Out of VRAM");
        pvr_txr_load(i_Bitmap->GetDatas(), l_Ptr, l_SizeX * l_SizeY * 2);
        l_TexId = m_TextureHA.Add();
        l_Texture = &m_TextureHA[l_TexId];
        l_Texture->m_Ptr = (pvr_ptr_t)l_Ptr;
        l_Texture->m_Format = PvrFormatFromBitmap(i_Bitmap);
        l_Texture->m_SizeX = l_SizeX;
        l_Texture->m_SizeY = l_SizeY;
        l_Texture->m_Bmap = i_Bitmap;
        i_Bitmap->SetTexId(l_TexId);
    }
    else {
        l_Texture = &m_TextureHA[l_TexId];
    }
    l_Texture->m_Used = TRUE;
    return l_Texture;
}

void DCRenderer_Z::FreeTexture(S16 i_TexId) {
    pvr_mem_free((void*)m_TextureHA[i_TexId].m_Ptr);
    m_TextureHA[i_TexId].m_Bmap->SetTexId((S16)INVALID_TEXID);
    m_TextureHA.Remove(i_TexId);
}

// SOUSTRACTIF has no PVR equivalent, falls back to alpha blending.
static void PvrBlendFromRenderFlags(U32 i_RenderFlags, pvr_blend_mode_t& o_Src, pvr_blend_mode_t& o_Dst) {
    switch (i_RenderFlags & (FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_IS_TRANSPARENT | FL_MTL_RDR_ADDITIF | FL_MTL_RDR_SOUSTRACTIF | FL_MTL_RDR_DESTADDITIF)) {
        case FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_ADDITIF:
        case FL_MTL_RDR_ADDITIF:
            o_Src = PVR_BLEND_SRCALPHA;
            o_Dst = PVR_BLEND_ONE;
            break;

        case FL_MTL_RDR_DESTADDITIF:
        case FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_DESTADDITIF:
            o_Src = PVR_BLEND_DESTALPHA;
            o_Dst = PVR_BLEND_ONE;
            break;

        case FL_MTL_RDR_SOUSTRACTIF:
        case FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_SOUSTRACTIF:
            o_Src = PVR_BLEND_SRCALPHA;
            o_Dst = PVR_BLEND_INVSRCALPHA;
            break;

        case FL_MTL_RDR_IS_ALPHABLENDED:
            o_Src = PVR_BLEND_SRCALPHA;
            o_Dst = PVR_BLEND_INVSRCALPHA;
            break;

        // Punch through
        case FL_MTL_RDR_IS_TRANSPARENT:
            o_Src = PVR_BLEND_ONE;
            o_Dst = PVR_BLEND_ZERO;
            break;

        case FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_IS_TRANSPARENT:
            o_Src = PVR_BLEND_SRCALPHA;
            o_Dst = PVR_BLEND_INVSRCALPHA;
            break;

        default:
            o_Src = PVR_BLEND_ONE;
            o_Dst = PVR_BLEND_ZERO;
            break;
    }
}

static pvr_depthcmp_mode_t PvrDepthCmpFromDrawState(U16 i_DrawState) {
    if (i_DrawState & ds_ztest) {
        return PVR_DEPTHCMP_GEQUAL;
    }
    return PVR_DEPTHCMP_ALWAYS;
}

// TODO: backface culling once the 3D winding is settled. 2D quads wind clockwise
// in screen space and request ds_cw, so mapping it to PVR_CULLING_CW culls them.
static pvr_cull_mode_t PvrCullFromDrawState(U16 i_DrawState, U32 i_RenderFlags) {
    return PVR_CULLING_SMALL;
}

static pvr_uv_clamp_t PvrUvClampFromRenderFlags(U32 i_RenderFlags) {
    switch (i_RenderFlags & (FL_MTL_RDR_TEX_ADDRESS_CLAMP_U | FL_MTL_RDR_TEX_ADDRESS_CLAMP_V)) {
        case FL_MTL_RDR_TEX_ADDRESS_CLAMP_U:
            return PVR_UVCLAMP_U;
        case FL_MTL_RDR_TEX_ADDRESS_CLAMP_V:
            return PVR_UVCLAMP_V;
        case FL_MTL_RDR_TEX_ADDRESS_CLAMP_U | FL_MTL_RDR_TEX_ADDRESS_CLAMP_V:
            return PVR_UVCLAMP_UV;
        default:
            return PVR_UVCLAMP_NONE;
    }
}

static S32 PvrFormatFromBitmap(Bitmap_Z* i_Bitmap) {
    U8 l_Format = i_Bitmap->GetFormat();
    switch (l_Format) {
        case BM_4444:
            return PVR_TXRFMT_ARGB4444;
        case BM_565:
            return PVR_TXRFMT_RGB565;
        default:
            ASSERT_Z(FALSE, "DCRenderer_Z::PvrFormatFromBitmap: Unsupported bitmap format");
            break;
    }
    return 0;
}

static pvr_filter_mode_t PvrFilterFromBitmap(Bitmap_Z* i_Bitmap) {
    if (i_Bitmap->IsFlagEnable(FL_BITMAP_BILINEAR)) {
        return PVR_FILTER_BILINEAR;
    }
    return PVR_FILTER_NEAREST;
}

// The 2D assembler, standing in for GC's SetRenderBlendOp + SetRenderContext +
// DrawState. Per-polygon state is not latched in registers on the PVR, it goes
// out as a header in the vertex stream, so those three mostly collapse into one
// context fill; the exceptions are global registers, PVR_PT_ALPHA_REF and the fog
// colour and curve among them, where only the fog type reaches the header.
// Fills a local context only and must not touch the 3D state members: 2D and 3D
// both record during Draw and drain at EndRender, so writing them would break the
// "state the engine last set" meaning the 3D path depends on.
void DCRenderer_Z::BuildPolyContext(pvr_poly_cxt_t& o_Cxt, Material_Z* i_Material, U16 i_DrawState, U32 i_RenderFlags, pvr_list_t i_ListType) {
    // Left to do:
    //  - o_Cxt.gen.fog_type once there is fog state to read. It arrives outside
    //    the material through SetCurrentFogColor and GC's EnableFog / DisableFog,
    //    so it belongs in a member rather than a parameter.
    //
    // ds_cwrite has no equivalent, the PVR has no colour write mask, so
    // FL_MTL_CODE_ZONLY materials cannot be expressed.
    Bitmap_Z* l_Bitmap = i_Material ? (Bitmap_Z*)i_Material->GetBitmap(Material_Z::mtl_diffuse) : NULL;
    DCTexture_Z* l_Texture = l_Bitmap ? LoadTexture(l_Bitmap) : NULL;

    if (l_Texture) {
        pvr_poly_cxt_txr(&o_Cxt, i_ListType, l_Texture->m_Format, l_Texture->m_SizeX, l_Texture->m_SizeY, l_Texture->m_Ptr, PvrFilterFromBitmap(l_Bitmap));
        o_Cxt.txr.env = PVR_TXRENV_MODULATEALPHA;
    }
    else {
        pvr_poly_cxt_col(&o_Cxt, i_ListType);
    }

    PvrBlendFromRenderFlags(i_RenderFlags, o_Cxt.blend.src, o_Cxt.blend.dst);
    o_Cxt.depth.comparison = PvrDepthCmpFromDrawState(i_DrawState);
    o_Cxt.depth.write = (i_DrawState & ds_zwrite) != 0;
    o_Cxt.gen.culling = PvrCullFromDrawState(i_DrawState, i_RenderFlags);
    o_Cxt.txr.uv_clamp = PvrUvClampFromRenderFlags(i_RenderFlags);
}

Bool DCRenderer_Z::Minimize() {
    // Reset texture usage flags and free any that were not used
    S32 i = m_TextureHA.FindFirst();
    while (i != -1) {
        if (m_TextureHA[i].m_Used)
            m_TextureHA[i].m_Used = FALSE;
        else
            FreeTexture(i);

        i = m_TextureHA.FindNext(i);
    }
    m_TextureHA.Minimize();

    // Minimize the 2D queue
    m_Draw2D.Minimize();

    return TRUE;
}

// Shut

void DCRenderer_Z::Shut() {
    Renderer_Z::Shut();
}
