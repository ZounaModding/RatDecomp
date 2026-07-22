#include "Names.h"

#include "BaseInGameDatas_G.h"
#include "Bitmap_Z.h"
#include "Bitmap_ZHdl.h"
#include "ClassManager_Z.h"
#include "Fonts_Z.h"
#include "Language_Z.h"
#include "LevelData_G.h"
#include "LoadingDraw_G.h"
#include "MaterialAnim_Z.h"
#include "MathTools_Z.h"
#include "Movie_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "ScriptManager_G.h"
#include "StreamManager_Z.h"
#include "SystemDatas_Z.h"
#include "Viewport_Z.h"

ExternC_Z int sprintf(char* i_Buf, const char* i_Format, ...);

U32 imageTGA[LOADING_HAT_BITMAP_PIXEL_COUNT] = {
#include "LoadingDraw_G.inc"
};

void LoadingDraw_C::Init() {
    ManipulatorDraw_Z::Init();
    SetGroup(ag_loading_draw_notpaused);

    m_LoadingHatMaterial = Material_ZHdl(gData.ClassMgr->NewObject("Material_Z"));
    m_LoadingHatMaterial->EnableRenderFlag(FL_MTL_RDR_TEX_ADDRESS_CLAMP_U);
    m_LoadingHatMaterial->EnableRenderFlag(FL_MTL_RDR_TEX_ADDRESS_CLAMP_V);

    Bitmap_ZHdl l_BitmapHdl(gData.ClassMgr->NewObject("Bitmap_Z"));
    Bitmap_Z* l_Bitmap = l_BitmapHdl;
    l_Bitmap->InitBmap(LOADING_HAT_BITMAP_SIZE, LOADING_HAT_BITMAP_SIZE, BM_8888, (U8*)imageTGA, NULL);
    l_Bitmap->EnableFlag(FL_BITMAP_UNK_0x8);
    l_Bitmap->SetTransp(BM_TRANSP_ONE);
    m_LoadingHatMaterial->SetBitmap(l_BitmapHdl);

    m_LevelLoadingScreenEnabled = TRUE;
    m_LoadingHatVisibleTimer = -1.0f;
    m_LevelLoadingScreenFadeOutTimer = -1.0f;
    m_LevelLoadingScreenFadeInTimer = -1.0f;
    m_IsStopped = TRUE;
    Activate();
    VpRegister();
}

Bool LoadingDraw_C::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }
    m_LoadingHatMaterial->MarkHandles();
    return TRUE;
}

void LoadingDraw_C::StartDrawLoadingBitmap() {
    m_IsStarted = TRUE;
    m_LevelLoadingScreenEnabled = TRUE;
    m_LevelLoadingScreenFadeOutTimer = -1.0f;
    m_LevelLoadingScreenFadeInTimer = -1.0f;
}

void LoadingDraw_C::EndDrawLoadingBitmap() {
    if (!m_IsStarted) {
        return;
    }

    m_LevelLoadingScreenEnabled = FALSE;
    m_LevelLoadingScreenFadeOutTimer = -1.0f;
    m_LevelLoadingScreenFadeInTimer = -1.0f;
    m_IsStarted = FALSE;
}

void LoadingDraw_C::Update(Float i_DeltaTime) {
    m_LoadingHatSpinTimer += i_DeltaTime;
    if (m_LoadingHatSpinTimer > LOADING_HAT_SPIN_DURATION) {
        m_LoadingHatSpinTimer -= LOADING_HAT_SPIN_DURATION;
        ++m_LoadingHatSpinCount;
    }

    if (m_LoadingHatVisibleTimer >= 0.0f) {
        m_LoadingHatVisibleTimer -= i_DeltaTime;
    }
    if (m_LevelLoadingScreenFadeInTimer >= 0.0f) {
        m_LevelLoadingScreenFadeInTimer += i_DeltaTime;
    }
    if (m_LevelLoadingScreenFadeOutTimer >= 0.0f) {
        if (i_DeltaTime > LOADING_DRAW_FADE_MAX_DELTA) {
            i_DeltaTime = LOADING_DRAW_FADE_MAX_DELTA;
        }
        m_LevelLoadingScreenFadeOutTimer += i_DeltaTime;
    }
}

void LoadingDraw_C::StopAnimLoading(Bool i_Stop) {
    m_IsStopped = i_Stop;
}

void LoadingDraw_C::Draw(const DrawInfo_Z& i_DrawInfo) {
    Bool l_StopDraw = m_IsStopped;
    if (gData.StreamMgr->GetLastError() != STR_ERROR_NONE || gData.MovieMgr->GetLastError() != MOVIE_ERROR_NONE) {
        l_StopDraw = TRUE;
    }

    if (!gData.ClassMgr->IsBigFileOpened()) {
        BaseInGameDatas_G* l_InGameDatas = gScriptMgr->GetInGameDatas();
        if (l_InGameDatas && !l_InGameDatas->IsBlackScreen()) {
            if (m_LoadingHatVisibleTimer <= 0.0f) {
                l_StopDraw = TRUE;
            }
        }
        else {
            m_LoadingHatVisibleTimer = LOADING_HAT_VISIBLE_DURATION;
        }
    }
    else {
        m_LoadingHatVisibleTimer = LOADING_HAT_VISIBLE_DURATION;
    }

    Renderer_Z* l_Renderer = i_DrawInfo.m_Vp->GetRenderer();
    S32 l_StartX;
    S32 l_StartY;
    S32 l_SizeX;
    S32 l_SizeY;
    i_DrawInfo.m_Vp->GetPosAndSize(l_StartX, l_StartY, l_SizeX, l_SizeY);
    Vec2f l_UvTopLeft;
    Vec2f l_UvBottomRight;
    l_UvTopLeft.x = 0.0f;
    l_UvTopLeft.y = 0.0f;
    l_UvBottomRight.x = 1.0f;
    l_UvBottomRight.y = 1.0f;

    if (m_LevelLoadingScreenEnabled) {
        MaterialAnim_ZHdl l_LoadingBitmapHdl = gData.SystemDatas->GetMaterialByName(
            Name_Z(Name_Z::GetID("LOADING_BITMAP", 0))
        );
        ResourceObject_ZHdl l_LoadingBitmapCheckHdl;
        l_LoadingBitmapCheckHdl = l_LoadingBitmapHdl;
        if (l_LoadingBitmapCheckHdl.IsValid()) {
            if (m_LevelLoadingScreenFadeInTimer < 0.0f) {
                m_LevelLoadingScreenFadeInTimer = 0.0f;
            }
            Float l_Alpha = LOADING_SCREEN_FADE_IN_RATE * m_LevelLoadingScreenFadeInTimer;
            if (l_Alpha > 1.0f) {
                l_Alpha = 1.0f;
            }
            DrawLoadingBitmap(i_DrawInfo.m_Vp, l_Alpha);
        }
    }
    else {
        if (m_LevelLoadingScreenFadeOutTimer < 0.0f) {
            m_LevelLoadingScreenFadeOutTimer = 0.0f;
        }
        if (m_LevelLoadingScreenFadeOutTimer < LOADING_SCREEN_FADE_OUT_DURATION) {
            DrawLoadingBitmap(i_DrawInfo.m_Vp, 1.0f - m_LevelLoadingScreenFadeOutTimer / LOADING_SCREEN_FADE_OUT_DURATION);
        }
    }

    if (gData.MovieMgr->IsPlaying() || l_StopDraw) {
        return;
    }

    l_Renderer->SetActiveMaterial(m_LoadingHatMaterial);
    Float l_SpinRatio = m_LoadingHatSpinTimer / LOADING_HAT_SPIN_DURATION;
    Float l_SpinSin = O_Sin(3.14159f * l_SpinRatio);
    Float l_SpinOffset = LOADING_HAT_MAX_HORIZONTAL_OFFSET * l_SpinSin;
    Vec2f l_BottomLeft;
    Vec2f l_TopRight;
    l_TopRight.y = LOADING_HAT_SCREEN_Y_RATIO * (Float)l_SizeY;
    l_BottomLeft.x = LOADING_HAT_LEFT_X + l_SpinOffset;
    l_TopRight.x = LOADING_HAT_RIGHT_X - l_SpinOffset;
    l_BottomLeft.y = l_TopRight.y - LOADING_HAT_DRAW_SIZE;

    U32 l_SpinParity = m_LoadingHatSpinCount & 1;
    Color l_BottomColor;
    Color l_TopColor;
    Vec3f l_ColorIntensity;
    if ((l_SpinParity != 0 && l_SpinRatio < LOADING_HAT_SPIN_HALF) || (l_SpinParity == 0 && l_SpinRatio > LOADING_HAT_SPIN_HALF)) {
        l_ColorIntensity.Set(LOADING_DRAW_BRIGHT_INTENSITY, LOADING_DRAW_BRIGHT_INTENSITY, LOADING_DRAW_BRIGHT_INTENSITY);
    }
    else {
        l_ColorIntensity.Set(LOADING_DRAW_DIM_INTENSITY, LOADING_DRAW_DIM_INTENSITY, LOADING_DRAW_DIM_INTENSITY);
    }
    l_TopColor.Set(l_ColorIntensity.x, l_ColorIntensity.y, l_ColorIntensity.z, 1.0f);
    l_BottomColor.Set(l_ColorIntensity.x, l_ColorIntensity.y, l_ColorIntensity.z, 1.0f);
    l_Renderer->Draw2DQuad(l_BottomLeft, l_TopRight, l_UvTopLeft, l_UvBottomRight, l_BottomColor, l_TopColor, LOADING_DRAW_Z_OFFSET);

    S32 l_FontId = gData.SystemDatas->GetFontId(fontName);
    if (l_FontId >= 0) {
        Fonts_Z* l_Font = gData.SystemDatas->GetFont(l_FontId);
        Char l_Text[LOADING_HAT_TEXT_BUFFER_SIZE];
        sprintf(l_Text, "%s", TT(LOADING_HAT_TEXT_ID));
        Color l_TextBottomColor;
        Color l_TextTopColor;
        l_TextTopColor.r = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextTopColor.g = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextTopColor.b = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextTopColor.a = 1.0f;
        l_TextBottomColor.r = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextBottomColor.g = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextBottomColor.b = LOADING_DRAW_BRIGHT_INTENSITY;
        l_TextBottomColor.a = 1.0f;
        Float l_TextX = LOADING_HAT_TEXT_X;
        l_Font->DrawString(l_Text, (S32)l_TextX, (S32)l_BottomLeft.y, l_TextBottomColor, l_TextTopColor, 1.0f, LOADING_DRAW_Z_OFFSET);
    }
}

Bool LoadingDraw_C::DrawLoadingBitmap(const Viewport_Z* i_Viewport, Float i_Alpha) {
    Renderer_Z* l_Renderer = i_Viewport->GetRenderer();
    S32 l_StartX;
    S32 l_StartY;
    S32 l_SizeX;
    S32 l_SizeY;
    i_Viewport->GetPosAndSize(l_StartX, l_StartY, l_SizeX, l_SizeY);
    Vec2f l_UvTopLeft;
    Vec2f l_UvBottomRight;
    l_UvTopLeft.x = 0.0f;
    l_UvTopLeft.y = 0.0f;
    l_UvBottomRight.x = 1.0f;
    l_UvBottomRight.y = 1.0f;

    MaterialAnim_ZHdl l_MaterialAnimHdl = gData.SystemDatas->GetMaterialByName(
        Name_Z(Name_Z::GetID("LOADING_BITMAP", 0))
    );
    if (l_MaterialAnimHdl.IsValid()) {
        MaterialAnim_Z* l_MaterialAnim = l_MaterialAnimHdl;
        ResourceObject_ZHdl l_MaterialHdl;
        l_MaterialHdl = l_MaterialAnim->GetMaterial();
        if (l_MaterialHdl.IsValid()) {
            l_Renderer->SetActiveMaterial((Material_Z*)(ResourceObject_Z*)l_MaterialHdl);
            Vec2f l_BottomLeft;
            Vec2f l_TopRight;
            l_BottomLeft.x = 0.0f;
            l_BottomLeft.y = 0.0f;
            l_TopRight.x = (Float)l_SizeX;
            l_TopRight.y = (Float)l_SizeY;
            Color l_Color;
            l_Color.r = LOADING_DRAW_BRIGHT_INTENSITY;
            l_Color.g = LOADING_DRAW_BRIGHT_INTENSITY;
            l_Color.b = LOADING_DRAW_BRIGHT_INTENSITY;
            l_Color.a = i_Alpha;
            l_Renderer->Draw2DQuad(l_BottomLeft, l_TopRight, l_UvTopLeft, l_UvBottomRight, l_Color, l_Color, LOADING_DRAW_Z_OFFSET);

            LevelData_GHdl l_LevelDataHdl = gScriptMgr->GetCurrentLevelData();
            if (l_LevelDataHdl.IsValid()) {
                MaterialLib* l_LoadingLib = l_LevelDataHdl->GetMaterialLib(
                    Name_Z(Name_Z::GetID("LOADING", 0))
                );
                S32 l_TextId;
                if (l_LoadingLib != NULL && (l_TextId = l_LoadingLib->m_TextId) >= 0) {
                    S32 l_FontId = gData.SystemDatas->GetFontId(fontName);
                    if (l_FontId >= 0) {
                        Fonts_Z* l_Font = gData.SystemDatas->GetFont(l_FontId);
                        U32 l_TextWidth = l_Font->GetSizeStr(TT(l_TextId));
                        Float l_TextX = (Float)((S32)(0.5f * (Float)(l_SizeX - l_TextWidth)));
                        Float l_TextY = (Float)((S32)(LOADING_SCREEN_TEXT_Y_RATIO * (Float)l_SizeY));
                        FontParam_Z l_Params;
                        l_Params.m_ZOffset = LOADING_DRAW_Z_OFFSET;
                        l_Params.m_BottomColor.r = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_BottomColor.g = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_BottomColor.b = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_BottomColor.a = i_Alpha;
                        l_Params.m_TopColor.r = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_TopColor.g = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_TopColor.b = LOADING_DRAW_BRIGHT_INTENSITY;
                        l_Params.m_TopColor.a = i_Alpha;
                        l_Params.m_BottomLeftPos.x = l_TextX;
                        l_Params.m_BottomLeftPos.y = l_TextY;
                        l_Params.m_MarginMult1 = 1.0f;
                        l_Params.m_Text = TT(l_TextId);
                        l_Params.m_HasBorder = TRUE;
                        l_Params.m_BorderOffset = LOADING_SCREEN_TEXT_BORDER_OFFSET;
                        l_Params.m_BorderColor = COLOR_BLACK;
                        l_Font->DrawString(l_Params);
                    }
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}
