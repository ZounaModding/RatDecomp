#ifndef _RENDERER_Z_H_
#define _RENDERER_Z_H_
#include "SystemObject_Z.h"
#include "Material_Z.h"
#include "Types_Z.h"
#include "Perlin3D_Z.h"
#include "Gaussian_Z.h"
#include "PrimitiveBuffers_Z.h"
#include "Viewport_Z.h"
#include "StaticArray_Z.h"
#include "GameManager_Z.h"

#define RATIO_SCREEN_STANDARD (4.f / 3.f)
#define RATIO_SCREEN_WIDESCREEN (16.f / 9.f)

#define FL_EFFECT_UNK_0x1 (U32)(1 << 0)             // 0x1
#define FL_EFFECT_CLEAR_SCREEN (U32)(1 << 1)        // 0x2
#define FL_EFFECT_UNK_0x4 (U32)(1 << 2)             // 0x4
#define FL_EFFECT_UNK_0x8 (U32)(1 << 3)             // 0x8
#define FL_EFFECT_DEPTH_OF_FIELD (U32)(1 << 4)      // 0x10
#define FL_EFFECT_VSYNC (U32)(1 << 5)               // 0x20
#define FL_EFFECT_UNK_0x40 (U32)(1 << 6)            // 0x40
#define FL_EFFECT_SPECIAL_VISION (U32)(1 << 7)      // 0x80
#define FL_EFFECT_UNK_0x100 (U32)(1 << 8)           // 0x100
#define FL_EFFECT_UNK_0x200 (U32)(1 << 9)           // 0x200
#define FL_EFFECT_SCREEN_DISTORTION (U32)(1 << 10)  // 0x400
#define FL_EFFECT_BLOOM (U32)(1 << 11)              // 0x800
#define FL_EFFECT_BLACK_AND_WHITE (U32)(1 << 12)    // 0x1000
#define FL_EFFECT_UNK_0x2000 (U32)(1 << 13)         // 0x2000
#define FL_EFFECT_UNK_0x4000 (U32)(1 << 14)         // 0x4000
#define FL_EFFECT_RADIAL_MOTION_BLUR (U32)(1 << 15) // 0x8000
#define FL_EFFECT_DEBUG_BUFFERS (U32)(1 << 29)      // 0x20000000
#define FL_EFFECT_MAKE_AVI (U32)(1 << 31)           // 0x80000000

// $SABE: PC only
enum SID_VS {
    SID_VS_DFLT = 0,
    SID_VS_NODFLT = 1,
    SID_VS_ZONLY = 2,
    SID_VS_OMNI = 3,
    SID_VS_REFR = 4,
    SIS_VS_OMNI_SHADOW = 5,
    SID_VS_SHADOW = 6,
    SID_VS_WATER = 7,
    SID_VS_COUNT = 8,
    SID_VS_NONE = 8
};

class Omni_Z;
class Camera_Z;
class LightData_Z;
class HFogData_Z;
class Occluder_Z;

enum ScreenType {
    screen_standard = 0,
    screen_widescreen
};

struct OmniFrust_Z {
    Sphere_Z m_BSphereCamSpace;
    Vec3f m_DirectionCamSpace;
    Float m_InvCosOuterHalfRad;
    Vec2f m_SinCosOuterHalfRad;
    U8 m_Unk_0x28[8]; // could be align, which means struct from 0x0 to 0x30
    Vec4f m_TranslationWorldSpace;
    Vec4f m_DirectionWorldSpace;
    Float m_UniformStartSquared; // omni m_Start * node->m_UniformScale
    Node_Z* m_Node;
    Omni_Z* m_Omni;
    U8 m_IndexInSA;
};

typedef StaticArray_Z<OmniFrust_Z*, 64, FALSE, FALSE> OmniFrustPtr_ZSA;

struct OmniStruct_Z {
    Vec4f m_TranslationLocalSpace;
    Vec3f m_DirectionWorldSpace;
    Float m_InvCosOuterHalfRad;
    Float m_SinOuterHalfRad;
    Float m_CosOuterHalfRad;
    Float m_Unk_0x28;
    Float m_Unk_0x2c;
    Float m_StartSqr;
    Float m_Attn;
    Float m_SpotAtt0;
    Float m_SpotAtt1;
    S32 m_Flag;
    OmniFrust_Z* m_OmniFrust;
    Float m_Unk_0x48;
    Float m_Unk_0x4c;
};

typedef StaticArray_Z<OmniStruct_Z, 64, FALSE, FALSE> OmniStruct_ZSA;

struct DrawInfo_Z {

    enum {
        FL_DRAWINFO_NONE = 0,
        FL_DRAWINFO_UNK_0x1 = 1,
        FL_DRAWINFO_UNK_0x2 = 2,
        FL_DRAWINFO_DO_POST_PROCESS = 4,
        FL_DRAWINFO_NO_HFOG = 8,
        FL_DRAWINFO_LIGHT_BAKED = 16,
    };

    Mat4x4 m_Local2Cam;
    Mat4x4 m_World2Cam;
    Box_Z m_BBoxCamSpace;
    ClipSphere_Z m_ClipSph;
    Viewport_Z* m_Vp;
    U32 m_VpId;
    S32 m_FirstPlayerVpId;
    S32 m_VpCount;
    Float m_VpTangent; // idk to what
    Float m_VpInvDiagTangent;
    Float m_VpDFov;
    Float m_VpRatio;
    Float m_VpHRatio;
    Float m_VpVRatio;
    Float m_VpStartX;
    Float m_VpStartY;
    Float m_VpSizeX;
    Float m_VpSizeY;
    Camera_Z* m_Camera;
    Node_Z* m_CameraNode;
    S32 m_UnkZero_0x120;
    Node_Z* m_Node;
    LightData_Z* m_LightData;
    S32 m_UnkZero_0x12c;
    HFogData_Z* m_CurrentNodeHFogData;
    HFogData_Z* m_MainHFogData; // HFog from first hfog node that can be found in children of root
    U8 m_UnkBytes_0x138[8];     // could be alignment
    Vec4f m_CameraTranslation;  // last is 1.0f
    Vec4f m_CameraDirection;    // last is 1.0f
    S32 m_Lod;                  // lod level to use for current
    S32 m_Flag;
    Float m_FadeValue; // idk what else to call it, it determines whether the object should be drawn or not
    Float m_Unk_0x16c;
    OmniFrustPtr_ZSA m_OmniFrustPtrSA;
    OmniStruct_ZSA m_OmniStructSA;
    S32 m_ActiveOmniCount;
    S32 m_ActiveOmniFlags;
    OmniStruct_Z* m_OmniStructs[64];
    Float m_LodFadeDist;
    Float m_LodShadowFadeDist;
    Float m_ParticlesFadeDist;
    Float m_LodDist;
    Float m_LodPatchMin;
    Float m_LodPatchMax;
    Float m_LodPatchDist;
    Occluder_Z* m_Occluder;
    Bool m_IsOccluding;
    BitArray_Z* m_OccludedZonesBA;
    S32 m_Unk0_0x17c0_From_Renderer_0x704; // gets assigned from Renderer_Z::m_UnkBoolFalse_0x704
    Float m_DeltaTime;

    DrawInfo_Z() {
        m_Node = NULL;
        m_UnkZero_0x120 = 0;
        m_Vp = NULL;
        m_Camera = NULL;
        m_LightData = NULL;
        m_UnkZero_0x12c = 0;
        m_CurrentNodeHFogData = NULL;
        m_MainHFogData = NULL;
        m_OccludedZonesBA = NULL;
        m_ActiveOmniCount = 0;
        m_VpCount = 0;
        m_DeltaTime = 0.0f;
        m_Occluder = NULL;
        m_IsOccluding = FALSE;
    }

    ~DrawInfo_Z() {
    }
};

struct CacheState_Z {
    CacheState_Z() {
        Reset();
    }

    void Reset() {
        m_MaxInUseNb = 0;
        m_InUseNb = 0;
        m_TotalNb = 0;
    }

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

enum BitmapType {
    BITMAP_DIFFUSE = 0,   // Color bitmap
    BITMAP_RADIOSITY = 1, // Baked lighting bitmap
    BITMAP_UNK_0x2 = 2,   // Unknown purpose
    BITMAP_UNK_0x3 = 3,   // Unknown purpose
    BITMAP_UNK_0x4 = 4,   // Unknown purpose
    BITMAP_UNK_0x5 = 5,   // Unknown purpose
    BITMAP_UNK_0x6 = 6,   // Unknown purpose
    BITMAP_UNK_0x7 = 7,   // Unknown purpose
    BITMAP_UNK_0x8 = 8,   // Unknown purpose
    BITMAP_UNK_0x9 = 9,   // Unknown purpose
    BITMAP_UNK_0xA = 10,  // Unknown purpose
    BITMAP_NB_TYPES
};

class Renderer_Z {
protected:
    Viewport_Z m_Viewports[7];
    S8 m_NbViewports;
    S8 m_ActiveViewport;
    S32 m_SizeX;
    S32 m_SizeY;
    Material_Z* m_ActiveMaterial;
    Bitmap_Z* m_ActiveBitmaps[BITMAP_NB_TYPES];
    void* m_NullVertexConstant;
    Float m_LodDist;
    Float m_LodPatchDist;
    S32 m_LodPatchMax;
    S32 m_LodPatchMin;
    CacheState_Z m_LightCacheState1;
    CacheState_Z m_LightCacheState2;
    CacheState_Z m_PatchCacheState;
    Bool m_UnkBoolFalse_0x704;
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
    U32 m_EffectFlag;
    Color m_ClearColor;
    Color m_UnkRtcFXColor;
    Char m_Pad_0x764[8];
    Vec3f m_UnkVec3f_001_0x76c;
    Vec3f m_UnkVec3f_100_0x778;
    Float m_LodFadeDist;
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
    Float m_XScaleFactor;
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

    static Float Renderer_ZDefaultNear;
    static Float Renderer_ZCurrentNear;

public:
    Renderer_Z();
    static void SwitchScreen(ScreenType i_ScreenType);
    void SetSize(S32 i_SizeX, S32 i_SizeY);

    Viewport_Z& GetViewport(S32 i_ViewportID) { return m_Viewports[i_ViewportID]; }

    inline Float GetScreenRatio() {
        return ScreenRatio;
    }

    virtual ~Renderer_Z();
    virtual Bool Init(S32 i_SizeX, S32 i_SizeY);
    virtual void Shut();
    virtual void Reset();

    virtual void UpdateResource() { }

    virtual void BeginRender();
    virtual void EndRender(Float i_DeltaTime);
    virtual void Draw(Float i_DeltaTime);

    virtual void WaitEndFrame() { }

    virtual void WaitForRetrace(Float i_DeltaTime) { }

    virtual void CleanAllDatas() { }

    virtual Bool Minimize() { return FALSE; }

    virtual void Draw(S32 i_ViewportId, Float i_DeltaTime);

    virtual void SetViewMatrix(Bool i_Unk) { }

    virtual void DrawTransparent(DrawInfo_Z& i_DrawInfo);

    virtual void DrawPostRenderEffects(DrawInfo_Z& i_DrawInfo) { }

    virtual void InitViewport(U32 i_NbViewports);
    virtual void ClearZBuffer(S32 i_X, S32 i_Y, S32 i_Width, S32 i_Height);
    virtual void ClearFrameBuffer(S32 a1, S32 a2, S32 a3, S32 a4);

    virtual void GetRendererParams(Float& o_HSize, Float& o_VSize, Float& o_UnkParam_0, Float& o_HCenter, Float& o_VCenter) {
        o_HSize = m_HSize;
        o_VSize = m_VSize;
        o_UnkParam_0 = m_UnkParam_0;
        o_HCenter = m_HCenter;
        o_VCenter = m_VCenter;
    }

    virtual U32 GetViewportParam(S32 a1, Float& a2, Float& a3, Float& a4, Float& a5, Float& a6) { return 0; }

    virtual void SetGammaRamp(Float a1, Float a2, Float a3) { }

    virtual void SetBrightness(Float i_Brightness) { }

    virtual void SetContrast(Float i_Contrast) { }

    virtual void SetGamma(Float i_Gamma) { }

    virtual void SetClearColor(const Color& i_ClearColor) {
        m_ClearColor = i_ClearColor;
    }

    virtual Float GetBrightness() { return 0.0f; }

    virtual Float GetContrast() { return 1.0f; }

    virtual Float GetGamma() { return 1.0f; }

    virtual Color& GetClearColor() { return m_ClearColor; }

    virtual void MoveScreenOrigin(S32 a1, S32 a2);
    virtual void SetDOF_Depth(Float i_DofDepth);

    virtual void PushOrder(Float i_Order) { }

    virtual void PushSo(U8 i_SceneOrder) { }

    virtual void PushDo(U8 i_DrawOrderGroup) { }

    virtual void PushDs(U16 i_DrawState) { }

    virtual U32 PushGroupId(U8 i_GroupId) { return 0; }

    virtual S32 PushDraw(const PrimitiveInfo_Z* i_PrimInfo, S32 i_Unk) { return -1; }

    virtual void PushMatrix(const Mat4x4* i_Matrix) { };

    virtual void SetDLight(const DrawInfo_Z& i_DrawInfo) { }

    virtual void* GetPushedVertexConstant(S32 i_Idx) { return m_NullVertexConstant; }

    virtual void* PushVertexConstant(S32 i_Idx) { return m_NullVertexConstant; }

    virtual void SetVertexConstant(ObjConstant_Z* i_VertexConstant, S32 i_Idx) { }

    virtual void SetActiveMaterial(Material_Z* i_Material);
    virtual void SetBlankMaterial();
    virtual void SetActiveTexture(Bitmap_Z* i_Bitmap, S32 i_Unk);

    virtual void FreeTexture(S16 i_TexId) { }

    virtual void CreateVertexBuffer(S32 a1);
    virtual void DeleteVertexBuffer(VertexBuffer_Z* a1);
    virtual void CreateIndexBuffer(S32 a1);
    virtual void DeleteIndexBuffer(IndexBuffer_Z* a1);
    virtual void SetLightFactor(Float a1, Float a2, Float a3);
    virtual void GetLightFactor(Float& a1, Float& a2, Float& a3);

    virtual void GetSpecialVisionColor(Vec3f& o_SpecialVisionColor1, Vec3f& o_SpecialVisionColor2, Node_Z* i_Node) const {
        if (!i_Node->IsFlagEnable(FL_NODE_SPECIAL_VISION)) {
            return;
        }
        o_SpecialVisionColor1 = m_SpecialVisionColor3;
        o_SpecialVisionColor2 = m_SpecialVisionColor4;
    }

    virtual void SetSpecialVisionColor(const Vec3f& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4);
    virtual U32 GetTextureSize();

    virtual Float GetPercentAlloc(S32 i_Unk) { return 0.0f; }

    virtual void MarkHandles() { }

    virtual void SetCurrentFogColor(const Vec3f& i_Color) {
        m_FogColor = i_Color;
    }

    virtual void ApplyDirectionalOnVertex(Vertex3D* a1, Vec3f& a2, LightData_Z& a3);
    virtual void ApplyFogOnVertex(Vertex3D* a1, Vec4f& a2);
    virtual void DrawFace(Vertex3D& a1, Vertex3D& a2, Vertex3D& a3);
    virtual void DrawStrip(Vertex3D* a1, U32 a2, Bool a3);
    virtual void DrawStripAfterScene(Vertex3D* a1, U32 a2, Bool a3);
    virtual void DrawLine(const Vec3f& a1, const Vec3f& a2, const Color& a3, Bool a4, Bool a5);
    virtual void DrawLine(const Vec2f& a1, const Vec2f& a2, const Color& a3, Float a4);
    virtual void DrawImage(Bitmap_ZHdl& a1);
    virtual void DrawCross(const Vec3f& a1, const Color& a2, Float a3);
    virtual void Draw2DQuad(const Vec2f& a1, const Vec2f& a2, const Vec2f& a3, const Vec2f& a4, const Color& a5, const Color& a6, Float a7);
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Color& a3, Color& a4, Float a5);
    virtual void DrawQuad(Vec2f& i_Pos, Vec2f& i_BottomRight, Color& i_Color, Float i_Z);
    virtual void DrawQuad(Vec2f& a1, Vec2f& a2, Vec2f& a3, Vec2f& a4, Vec3f& a5, Float a6);

    virtual void DrawTri(Vec2f& i_Point1, Vec2f& i_Point2, Vec2f& i_Point3, Color& i_Color, Float i_Z) { }

    virtual void Draw2DQuad(Vec2f* a1, Vec3f* a2, Vec2f* a3, Float a4, Float a5);
    virtual void DrawStrip(Vec2f* a1, S32 a2, const Color& a3, Float a4);
    virtual void DrawFan(Vec2f* a1, S32 a2, const Color& a3, Float a4);

    virtual void EnableZBuffer(Bool i_Enable) { }

    virtual void DrawString(const Vec2f& i_Pos, const Char* i_Text, const Color& i_Color, Float i_Scale, Float i_Z);
    virtual void DrawString(const Vec3f& a1, const Char* a2, Bool a3);
    virtual void DrawString(const Vec3f& a1, const Char* a2, const Color& a3, Bool a4);

    virtual void SetShadowMapRect(Vec2f& i_ShadowMapRectPoint1, Vec2f& i_ShadowMapRectPoint2, Vec2f& i_ShadowMapRectPoint3) {
        m_ShadowMapRectPoint1 = i_ShadowMapRectPoint1;
        m_ShadowMapRectPoint2 = i_ShadowMapRectPoint2;
        m_ShadowMapRectPoint3 = i_ShadowMapRectPoint3;
    }

    virtual U32 RenderScreen(U8** a1, S32& a2, S32& a3, U8& a4, Bool a5) {
        return 0;
    }

    virtual void MakeScreenShot(Char* i_FilePath);

    virtual void MakeAVI(S32 a1, S32 a2) {
        EnableEffectFlag(FL_EFFECT_MAKE_AVI);
        ProgramDefaultFlag();
        gData.DisableEngineFlag(FL_DISPLAY_FPS);
    }

    virtual void CloseAVI() {
        DisableEffectFlag(FL_EFFECT_MAKE_AVI);
    }

    virtual void SetActiveViewport(S32 i_ViewportID);
    virtual void FlushActiveViewport();

    inline Bool IsEffectFlag(U32 i_Flag) { return (m_EffectFlag & i_Flag); }

    inline void EnableEffectFlag(U32 i_Flag) { m_EffectFlag |= i_Flag; }

    inline void DisableEffectFlag(U32 i_Flag) { m_EffectFlag &= ~i_Flag; }

    void SetLightingType(U32 i_LightingType) {
        m_LightingType = i_LightingType;
    }

    U32 GetLightingType() const {
        return m_LightingType;
    }

    void SetLodDist(Float i_LodDist) {
        m_LodDist = i_LodDist;
    }

    void SetLodFadeDist(Float i_LodFadeDist) {
        m_LodFadeDist = i_LodFadeDist;
    }

    void SetLodPatchDist(Float i_LodPatchDist) {
        m_LodPatchDist = i_LodPatchDist;
    }

    void SetLodPatchMin(S32 i_LodPatchMin) {
        m_LodPatchMin = i_LodPatchMin;
    }

    void SetLodShadowFadeDist(Float i_LodShadowFadeDist) {
        m_LodShadowFadeDist = i_LodShadowFadeDist;
    }

    void SetMaxAnisotropy(U32 i_MaxAnisotropy) {
        m_MaxAnistropy = i_MaxAnisotropy;
    }

    void SetMultisampleType(U32 i_MultisampleType) {
        m_MultisampleType = i_MultisampleType;
    }

    void SetParticlesFadeDist(Float i_ParticlesFadeDist) {
        m_ParticlesFadeDist = i_ParticlesFadeDist;
    }

    void SetTextureFiltering(U32 i_TextureFiltering) {
        m_TextureFiltering = i_TextureFiltering;
    }

    void SetTextureQuality(U32 i_TextureQuality) {
        m_TextureQuality = i_TextureQuality;
    }

    void SetLodPatchMax(S32 i_LodPatchMax) {
        m_LodPatchMax = i_LodPatchMax;
    }

    S32 GetNbViewport() const {
        return m_NbViewports;
    }

    inline Float GetScreenRatio() const {
        return ScreenRatio;
    }

    inline Float GetXScaleFactor() const {
        return m_XScaleFactor;
    }

private:
    DynArray_Z<VertexBuffer_Z*> m_VertexBufferPtrDA;
    DynArray_Z<IndexBuffer_Z*> m_IndexBufferPtrDA;
    PerlinArray3D_Z<8, 8, 8> m_PerlinArray3D;
    static Float ScreenRatio;
};

Bool SetSplitType();
Bool SetWorldToSplit();
Bool SetLodRender();
Bool SetLIghtingType();
Bool SetLOdTexture();
Bool SetMAxAnisotropy();
Bool SetTExtureFiltering();
Bool FitOnObject();
Bool DisplayImage();
Bool MakeAvi();
Bool CloseAvi();
Bool StartRenderBench();

#endif
