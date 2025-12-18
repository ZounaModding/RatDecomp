#ifndef _RENDERER_Z_H_
#define _RENDERER_Z_H_
#include "SystemObject_Z.h"
#include "Material_Z.h"
#include "Types_Z.h"
#include "PerlinArray3D_Z.h"
#include "Gaussian_Z.h"
#include "RenderPrimitiveBuffers_Z.h"
#include "Assert_Z.h"
#include "Viewport_Z.h"

#define RATIO_SCREEN_STANDARD (4.f / 3.f)
#define RATIO_SCREEN_WIDESCREEN (16.f / 9.f)

enum ScreenType {
    screen_standard = 0,
    screen_widescreen
};

struct DrawInfo_Z {
    Mat4x4 Local2Cam;
    Mat4x4 World2Cam;
    Box_Z m_Box;

public:
    DrawInfo_Z() { }
};

struct CacheState_Z {
public:
    S32 m_TotalNb;
    S32 m_InUseNb;
    S32 m_MaxInUseNb;
};

struct PrimitiveInfo_Z;
struct ObjConstant_Z;
class Node_Z;
struct VertexBuffer_Z;
struct IndexBuffer_Z;
struct Vertex3D;
class LightData_Z;

class Renderer_Z {
private:
    Viewport_Z m_Viewports[7];
    S8 m_NbViewports;
    S8 m_ActiveViewport;
    S32 m_SizeX;
    S32 m_SizeY;
    Material_Z* m_ActiveMaterial;
    Bitmap_Z* m_ActiveBitmap;
    Bitmap_Z* m_ActiveRadiosityBitmap;
    Char m_Pad0x6a8[36];
    void* m_NullVertexConstant;
    Float m_LodDist;
    Float m_LodPatchDist;
    U32 m_LodPatchMax;
    U32 m_LodPatchMin;
    CacheState_Z m_LightCacheState1;
    CacheState_Z m_LightCacheState2;
    CacheState_Z m_PatchCacheState;
    Bool m_UnkBoolFalse0x704;
    Float m_DisplayCacheStateUpdateTimer;
    Float m_LightFactorX;
    Float m_LightFactorY;
    Float m_LightFactorZ;
    Vec3f m_FogColor;
    U32 m_LightingType;
    U32 m_LodTexture;
    U32 m_TextureQuality;
    U32 m_TextureFiltering;
    U32 m_MaxAnistropy;
    U32 m_MultisampleType;
    Bool m_UnkBoolFalse_0x73C;
    S32 m_EffectFlag;
    Color m_ClearColor;
    Color m_UnkRtcFXColor;
    Char m_Pad_0x764[8];
    Vec3f m_UnkVec3f_001_0x76c;
    Vec3f m_UnkVec3f_100_0x778;
    Float m_LodfadeDist;
    Float m_LodShadowFadeDist;
    Float m_ParticlesFadeDist;
    Float m_DOF_Depth;
    MipmapGC m_MipmapGC;
    S32 m_MipmapedGaussConvol;
    Vec3f m_SpecialVisionColor1;
    Vec3f m_SpecialVisionColor2;
    Vec3f m_SpecialVisionColor3;
    Vec3f m_SpecialVisionColor4;
    Vec2f m_ShadowMapRectPoint1;
    Vec2f m_ShadowMapRectPoint2;
    Vec2f m_ShadowMapRectPoint3;
    Float m_HSize;
    Float m_VSize;
    Float m_UnkParam_0; // $VIOLET: always 0
    Float m_HCenter;
    Float m_VCenter;
    Float m_DefaultScreenRatio;
    Float m_GpuTargetMsOrFps;
    Float m_UnkFloat_0_0x8bc;
    Float m_CpuTargetMsOrFps;
    Float m_UnkFloat_0_0x8c4;
    Float m_CurFps;
    String_Z<ARRAY_CHAR_MAX> m_TimeString;
    Float m_DisplayFpsUpdateTimer;
    String_Z<ARRAY_CHAR_MAX> m_GpuString;
    Float m_GpuFps;
    String_Z<ARRAY_CHAR_MAX> m_CpuString;
    Float m_CpuFps;
    String_Z<ARRAY_CHAR_MAX> m_FpsString;
    Float m_Fps;
    String_Z<ARRAY_CHAR_MAX> m_FragmentsString;
    String_Z<ARRAY_CHAR_MAX> m_LargestBlockString;
    String_Z<ARRAY_CHAR_MAX> m_FreeMemString;
    String_Z<ARRAY_CHAR_MAX> m_MatrixUsageString;
    String_Z<ARRAY_CHAR_MAX> m_LightC1String;
    String_Z<ARRAY_CHAR_MAX> m_LightC2String;
    String_Z<ARRAY_CHAR_MAX> m_PatchCDString;
    String_Z<ARRAY_CHAR_MAX> m_GlobalDmaString;
    U32 m_GblDmaInUseNb;
    U32 m_GblDmaMaxInUseNb;
    U32 m_GblDmaTotalNb;
    String_Z<ARRAY_CHAR_MAX> m_GifDmaString;
    U32 m_GifDmaInUseNb;
    U32 m_GifDmaMaxInUseNb;
    U32 m_GifDmaTotalNb;

public:
    Renderer_Z();
    static void SwitchScreen(ScreenType i_ScreenType);
    void SetSize(S32 i_SizeX, S32 i_SizeY);

    Viewport_Z& GetViewport(S32 i_ViewportID) { return m_Viewports[i_ViewportID]; }

    Float GetScreenRatio();
    // clang-format off
    virtual ~Renderer_Z();                                                                                                                   /* 0x08 */
    virtual Bool Init(S32 i_SizeX, S32 i_SizeY);                                                                                             /* 0x0C */
    virtual void Shut();                                                                                                                     /* 0x10 */
    virtual void Reset();                                                                                                                    /* 0x14 */
    virtual void UpdateResource();                                                                                                           /* 0x18 */
    virtual void BeginRender();                                                                                                              /* 0x1C */
    virtual void EndRender(Float a1);                                                                                                        /* 0x20 */
    virtual void Draw(Float a1);                                                                                                             /* 0x24 */
    virtual void WaitEndFrame();                                                                                                             /* 0x28 */
    virtual void WaitForRetrace(Float a1);                                                                                                   /* 0x2C */
    virtual void CleanAllDatas();                                                                                                            /* 0x30 */
    virtual void Minimize();                                                                                                                 /* 0x34 */
    virtual void Draw(S32 a1, Float a2);                                                                                                     /* 0x38 */
    virtual void SetViewMatrix(Bool a1);                                                                                                     /* 0x3C */
    virtual void DrawTransparent(DrawInfo_Z& a1);                                                                                            /* 0x40 */
    virtual void DrawPostRenderEffects(DrawInfo_Z& a1);                                                                                      /* 0x44 */
    virtual void InitViewport(U32 a1);                                                                                                       /* 0x48 */
    virtual void ClearZBuffer(S32 a1, S32 a2, S32 a3, S32 a4);                                                                               /* 0x4C */
    virtual void ClearFrameBuffer(S32 a1, S32 a2, S32 a3, S32 a4);                                                                           /* 0x50 */
    virtual void GetRendererParams(Float& a1, Float& a2, Float& a3, Float& a4, Float& a5);                                                   /* 0x54 */
    virtual void GetViewportParam(S32 a1, Float& a2, Float& a3, Float& a4, Float& a5, Float& a6);                                            /* 0x58 */
    virtual void SetGammaRamp(Float a1, Float a2, Float a3);                                                                                 /* 0x5C */
    virtual void SetBrightness(Float a1);                                                                                                    /* 0x60 */
    virtual void SetContrast(Float a1);                                                                                                      /* 0x64 */
    virtual void SetGamma(Float a1);                                                                                                         /* 0x68 */
    virtual void SetClearColor(const Color& a1);                                                                                             /* 0x6C */
    virtual void GetBrightness();                                                                                                            /* 0x70 */
    virtual void GetContrast();                                                                                                              /* 0x74 */
    virtual void GetGamma();                                                                                                                 /* 0x78 */
    virtual void GetClearColor();                                                                                                            /* 0x7C */
    virtual void MoveScreenOrigin(S32 a1, S32 a2);                                                                                           /* 0x80 */
    virtual void SetDOF_Depth(Float a1);                                                                                                     /* 0x84 */
    virtual void PushOrder(Float a1);                                                                                                        /* 0x88 */
    virtual void PushSo(U8 a1);                                                                                                              /* 0x8C */
    virtual void PushDo(U8 a1);                                                                                                              /* 0x90 */
    virtual void PushDs(U16 a1);                                                                                                             /* 0x94 */
    virtual void PushGroupId(U8 a1);                                                                                                         /* 0x98 */
    virtual void PushDraw(const PrimitiveInfo_Z* a1, S32 a2);                                                                                /* 0x9C */
    virtual void PushMatrix(const Mat4x4* a1);                                                                                               /* 0xA0 */
    virtual void SetDLight(const DrawInfo_Z& a1);                                                                                            /* 0xA4 */
    virtual void GetPushedVertexConstant(S32 a1);                                                                                            /* 0xA8 */
    virtual void PushVertexConstant(S32 a1);                                                                                                 /* 0xAC */
    virtual void SetVertexConstant(ObjConstant_Z* a1, S32 a2);                                                                               /* 0xB0 */
    virtual void SetActiveMaterial(Material_Z* a1);                                                                                          /* 0xB4 */
    virtual void SetBlankMaterial();                                                                                                         /* 0xB8 */
    virtual void SetActiveTexture(Bitmap_Z* a1, S32 a2);                                                                                     /* 0xBC */
    virtual void FreeTexture(S16 a1) {}                                                                                                      /* 0xC0 */
    virtual void CreateVertexBuffer(S32 a1);                                                                                                 /* 0xC4 */
    virtual void DeleteVertexBuffer(VertexBuffer_Z* a1);                                                                                     /* 0xC8 */
    virtual void CreateIndexBuffer(S32 a1);                                                                                                  /* 0xCC */
    virtual void DeleteIndexBuffer(IndexBuffer_Z* a1);                                                                                       /* 0xD0 */
    virtual void SetLightFactor(Float a1, Float a2, Float a3);                                                                               /* 0xD4 */
    virtual void GetLightFactor(Float& a1, Float& a2, Float& a3);                                                                            /* 0xD8 */
    virtual void GetSpecialVisionColor(Vec3f& a1, Vec3f& a2, Node_Z* a3) const;                                                              /* 0xDC */
    virtual void SetSpecialVisionColor(const Vec3f& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4);                                  /* 0xE0 */
    virtual void GetTextureSize();                                                                                                           /* 0xE4 */
    virtual void GetPercentAlloc(S32 a1);                                                                                                    /* 0xE8 */
    virtual void MarkHandles();                                                                                                              /* 0xEC */
    virtual void SetCurrentFogColor(const Vec3f& a1);                                                                                        /* 0xF0 */
    virtual void ApplyDirectionalOnVertex(Vertex3D* a1, Vec3f& a2, LightData_Z& a3);                                                         /* 0xF4 */
    virtual void ApplyFogOnVertex(Vertex3D* a1, Vec4f& a2);                                                                                  /* 0xF8 */
    virtual void DrawFace(Vertex3D& a1, Vertex3D& a2, Vertex3D& a3);                                                                         /* 0xFC */
    virtual void DrawStrip(Vertex3D* a1, U32 a2, Bool a3);                                                                                   /* 0x100 */
    virtual void DrawStripAfterScene(Vertex3D* a1, U32 a2, Bool a3);                                                                         /* 0x104 */
    virtual void DrawLine(const Vec3f& a1, const Vec3f& a2, const Color& a3, Bool a4, Bool a5);                                              /* 0x108 */
    virtual void DrawLine(const Vec2f& a1, const Vec2f& a2, const Color& a3, Float a4);                                                      /* 0x10C */
    virtual void DrawImage(Bitmap_ZHdl& a1);                                                                                                 /* 0x110 */
    virtual void DrawCross(const Vec3f& a1, const Color& a2, Float a3);                                                                      /* 0x114 */
    virtual void Draw2DQuad(const Vec2f& a1, const Vec2f& a2, const Vec2f& a3, const Vec2f& a4, const Color& a5, const Color& a6, Float a7); /* 0x118 */
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Color& a4, Float a5);                                                             /* 0x11C */
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Float a4);                                                                        /* 0x120 */
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Vec2f& a3, Vec2f& a4, Vec3f& a5, Float a6);                                                  /* 0x124 */
    virtual void DrawTri(Vec2f& a1, Vec2f& a2, Vec2f& a3, Color& a4, Float a5);                                                              /* 0x128 */
    virtual void Draw2DQuad(Vec2f* a1, Vec3f* a2, Vec2f* a3, Float a4, Float a5);                                                            /* 0x12C */
    virtual void DrawStrip(Vec2f* a1, S32 a2, const Color& a3, Float a4);                                                                    /* 0x130 */
    virtual void DrawFan(Vec2f* a1, S32 a2, const Color& a3, Float a4);                                                                      /* 0x134 */
    virtual void EnableZBuffer(Bool a1);                                                                                                     /* 0x138 */
    virtual void DrawString(const Vec2f& a1, const Char* a2, const Color& a3, Float a4, Float a5);                                           /* 0x13C */
    virtual void DrawString(const Vec3f& a1, const Char* a2, Bool a3);                                                                       /* 0x140 */
    virtual void DrawString(const Vec3f& a1, const Char* a2, const Color& a3, Bool a4);                                                      /* 0x144 */
    virtual void SetShadowMapRect(Vec2f& a1, Vec2f& a2, Vec2f& a3);                                                                          /* 0x148 */
    virtual void RenderScreen(U8** a1, S32& a2, S32& a3, U8& a4, Bool a5);                                                                   /* 0x14C */
    virtual void MakeScreenShot(Char* a1);                                                                                                   /* 0x150 */
    virtual void MakeAVI(S32 a1, S32 a2);                                                                                                    /* 0x154 */
    virtual void CloseAVI();                                                                                                                 /* 0x158 */
    virtual void SetActiveViewport(S32 i_ViewportID);                                                                                        /* 0x15C */
    virtual void FlushActiveViewport();                                                                                                      /* 0x160 */
    // clang-format on
private:
    DynArray_Z<VertexBuffer_Z*> m_VertexBufferPtrDA;
    DynArray_Z<IndexBuffer_Z*> m_IndexBufferPtrDA;
    PerlinArray3D_Z<8, 8, 8> m_PerlinArray3D;
};

#endif
