#ifndef _DCRENDERER_Z_H_
#define _DCRENDERER_Z_H_
#include "Renderer_Z.h"
#include "DCRendererEnum_Z.h"
#include "DebugFontString_Z.h"
#include "Draw2D_Z.h"
#include "HoleArray_Z.h"

class Bitmap_Z;

#define RDR_COLOR_LOOP_PERIOD 5.0f
#define DC_DEFAULT_NEAR 0.001f
#define MAX_OMNI 3
#define MAX_DRAW_CALLS_PER_FRAME 4096
#define TEXTURE_GRANULARITY 64

// GC applies GX_CS_SCALE_2 on the final texture modulate stage of every textured
// render context, so 0.5 is its neutral vertex colour. The PVR has no output
// scale, so the doubling moves to the vertex and clamps before the texture
// multiply instead of after it.
#define DC_MODULATE_SCALE 2.0f

// Punch through alpha test threshold.
#define DC_PT_ALPHA_REF 64

static const Vec3f COLOR_ZOUNA = { 0.549f, 0.58f, 1.0f };
static const Vec3f COLOR_POUNI = { 0.365f, 1.0f, 0.714f };

class DCRenderer_Z : public Renderer_Z {
public:
    class DCTexture_Z {
    public:
        pvr_ptr_t m_Ptr;
        U32 m_Format;
        U16 m_SizeX;
        U16 m_SizeY;
        Bitmap_Z* m_Bmap;
        Bool m_Used;
    };

    DCRenderer_Z() {
    }

    virtual ~DCRenderer_Z() {
        Shut();
    }

    virtual Bool Init(S32 i_SizeX, S32 i_SizeY);
    virtual void Shut();
    virtual void BeginRender();
    virtual void EndRender(Float i_DeltaTime);
    virtual Bool Minimize();
    virtual void SetActiveMaterial(Material_Z* i_Material);

    virtual void DrawQuad(Vec2f& i_TopLeft, Vec2f& i_BottomRight, Color& i_Color, Float i_Z);
    virtual void DrawQuad(Vec2f& i_TopLeft, Vec2f& i_BottomRight, Vec2f& i_UvTopLeft, Vec2f& i_UvBottomRight, Vec3f& i_Color, Float i_Z);
    virtual void Draw2DQuad(const Vec2f& i_TopLeft, const Vec2f& i_BottomRight, const Vec2f& i_UvTopLeft, const Vec2f& i_UvBottomRight, const Color& i_ColorTop, const Color& i_ColorBottom, Float i_Z);
    virtual void Draw2DQuad(Vec2f* i_Positions, Vec3f* i_Colors, Vec2f* i_TextureCoordinates, Float i_Z, Float i_Alpha);
    virtual void DrawString(const Vec2f& i_Pos, const Char* i_Text, const Color& i_Color, Float i_Z, Float i_Scale);

    virtual void MarkHandles();
    virtual void FreeTexture(S16 i_TexId);

    virtual void WaitForRetrace(Float i_DeltaTime);

    void DrawOpaque();
    void DrawPunchthrough();
    void DrawTransparent();
    void BuildPolyContext(pvr_poly_cxt_t& o_Cxt, Material_Z* i_Material, U16 i_DrawState, U32 i_RenderFlags, pvr_list_t i_ListType);

    DCTexture_Z* LoadTexture(Bitmap_Z* i_Bitmap);

    void EmitQuad(const Vec2f& i_TopLeft, const Vec2f& i_BottomRight, const Vec2f& i_UvTopLeft, const Vec2f& i_UvBottomRight, const Color& i_ColorTop, const Color& i_ColorBottom, Float i_Z);

    void SetPvrBgColor(const Vec3f& i_Color) {
        m_PvrBgColor = i_Color;
        pvr_set_bg_color(
            m_PvrBgColor.x,
            m_PvrBgColor.y,
            m_PvrBgColor.z
        );
    }

    Vec3f& GetPvrBgColor() {
        return m_PvrBgColor;
    }

    void UpdatePvrBgColor(Float i_AbsTime) {
#ifdef DEBUG_Z
        static Bool l_HasRun = FALSE;
        static Float l_TimeOffset = 0.0f;
        if (!l_HasRun) {
            l_TimeOffset = i_AbsTime;
            l_HasRun = TRUE;
        }
        else {
            i_AbsTime -= l_TimeOffset;
        }

        Float l_PeriodDt = i_AbsTime / RDR_COLOR_LOOP_PERIOD;
        Float l_Phase = l_PeriodDt - (int)l_PeriodDt;

        if (l_Phase < 0.0f) {
            l_Phase += 1.0f;
        }

        Float l_TNorm;

        if (l_Phase < 0.5f) {
            l_TNorm = l_Phase * 2.0f;
        }
        else {
            l_TNorm = (1.0f - l_Phase) * 2.0f;
        }

        Float l_Red = COLOR_ZOUNA.x + l_TNorm * (COLOR_POUNI.x - COLOR_ZOUNA.x);
        Float l_Green = COLOR_ZOUNA.y + l_TNorm * (COLOR_POUNI.y - COLOR_ZOUNA.y);
        Float l_Blue = COLOR_ZOUNA.z + l_TNorm * (COLOR_POUNI.z - COLOR_ZOUNA.z);

        m_PvrBgColor.Set(l_Red, l_Green, l_Blue);

        pvr_set_bg_color(
            m_PvrBgColor.x,
            m_PvrBgColor.y,
            m_PvrBgColor.z
        );
#endif
    }

private:
    pvr_init_params_t m_PvrParams;
    Vec3f m_PvrBgColor;
    U32 m_RetraceCount;
    DebugFontString_Z m_DebugFontString;
    Draw2D_Z m_Draw2D;
    HoleArray_Z<DCTexture_Z, TEXTURE_GRANULARITY> m_TextureHA;
};

typedef DCRenderer_Z::DCTexture_Z DCTexture_Z;

#endif
