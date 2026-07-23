#include "Renderer_Z.h"
#include "Node_ZHdl.h"
#include "SystemObject_Z.h"
#include "Types_Z.h"
#include "Math_Z.h"
#include "Node_Z.h"
#include "Camera_Z.h"
#include "Console_Z.h"
#include "Program_Z.h"
#include "SystemDatas_Z.h"
#include "World_Z.h"
#include "ABC_ScriptManager.h"
#include "SoundManager_Z.h"
#include "Memory_Z.h"

#pragma dont_inline on

Renderer_Z::Renderer_Z() {
    REGISTERCOMMAND("SetSplitType", SetSplitType);
    REGISTERCOMMAND("SetWorldToSplit", SetWorldToSplit);
    REGISTERCOMMAND("SetLodRender", SetLodRender);
    REGISTERCOMMANDC("SetLIghtingType", SetLIghtingType, " [Pixel=0 Vertex=1]");
    REGISTERCOMMANDC("SetLodTexture", SetLOdTexture, "[Max=0,Min=max(level-1)]");
    REGISTERCOMMANDC("SetMaxAnisotropy", SetMAxAnisotropy, "[Min=1,Max=caps]");
    REGISTERCOMMANDC("SetTExtureFiltering", SetTExtureFiltering, "[Bilinear=0,Trilinear=1,Anisotropic=2]");
    REGISTERCOMMANDC("FitOnObject", FitOnObject, " [ObjectName] [VpId=0]");
    REGISTERCOMMANDC("DisplayImage", DisplayImage, " [DB ImageName]");
    REGISTERCOMMANDC("MakeAVI", MakeAvi, "FrameBuffer - Fps");
    REGISTERCOMMAND("CloseAVI", CloseAvi);
    REGISTERCOMMAND("StartRenderBench", StartRenderBench);
    m_ActiveViewport = 0;
    m_EffectFlag = FL_EFFECT_BLOOM | FL_EFFECT_VSYNC;
    m_XScaleFactor = RATIO_SCREEN_STANDARD;
    EnableEffectFlag(FL_EFFECT_CLEAR_SCREEN);
    m_UnkRtcFXColor.Set(0.5f, 0.5f, 0.5f, 1.0f);
    m_UnkVec3f_001_0x76c = Vec3f(0.0f, 0.0f, 1.0f);
    m_UnkVec3f_100_0x778 = Vec3f(1.0f, 0.0f, 0.0f);
    SetLodPatchMax(8);
    SetLodPatchMin(1);
    SetLodPatchDist(-1.0f);
    SetLodDist(10.0f);
    SetLodFadeDist(10.0f);
    SetLodShadowFadeDist(10.0f);
    SetParticlesFadeDist(20.0f);
    SetDOF_Depth(34.0f);
    SetLightingType(0);
    SetTextureFiltering(0);
    SetTextureQuality(0);
    SetMaxAnisotropy(1);
    SetMultisampleType(0);
    m_LightFactorX = 1.0f;
    m_LightFactorY = 1.0f;
    m_LightFactorZ = 1.0f;
    m_UnkBoolFalse_0x73C = FALSE;
    m_SpecialVisionColor1.Set(0.0f, 0.03137255f, 0.15686275f);
    m_SpecialVisionColor2.Set(0.0f, 0.0f, 0.0f);
    m_SpecialVisionColor3.Set(1.0f, 1.0f, 0.0f);
    m_SpecialVisionColor4.Set(1.0f, 1.0f, 0.0f);
    m_UnkParam_0 = 0;
    m_VSize = 0.0f;
    m_HSize = 0.0f;
    m_VCenter = 0.0f;
    m_HCenter = 0.0f;
    m_SizeY = 100.0f;
    m_SizeX = 100.0f;
    m_UnkBoolFalse_0x704 = FALSE;
    m_CurFps = 30.0f;
    m_CpuTargetMsOrFps = 30.0f;
    m_GpuTargetMsOrFps = 30.0f;
    m_UnkFloat_0_0x8c4 = 0.0f;
    m_UnkFloat_0_0x8bc = 0.0f;
    m_DisplayCacheStateUpdateTimer = 0.0f;
    m_GblDmaTotalNb = 0;
    m_GblDmaMaxInUseNb = 0;
    m_GblDmaInUseNb = 0;
    m_GifDmaTotalNb = 0;
    m_GifDmaMaxInUseNb = 0;
    m_GifDmaInUseNb = 0;
    m_DisplayFpsUpdateTimer = 0.0f;
    m_CpuFps = 60.0f;
    m_GpuFps = 60.0f;
    m_Fps = 60.0f;
    m_ClearColor.Set(0.0, 0.0f, 0.0f, 1.0f);
    m_PerlinArray3D.Generate(0.5f, 1.0f, 16);
}

Renderer_Z::~Renderer_Z() {
}

Bool Renderer_Z::Init(S32 i_SizeX, S32 i_SizeY) {
    SetSize(i_SizeX, i_SizeY);
    SwitchScreen(screen_standard);
    m_MipmapedGaussConvol = Compute_MipmapedGaussConvol(&m_MipmapGC, i_SizeX, i_SizeY, 14.0f, 1.0f);
    return TRUE;
}

void Renderer_Z::Shut() {
    for (S32 i = 0; i < m_VertexBufferPtrDA.GetSize(); i++) {
        m_VertexBufferPtrDA[i]->Release();
        Delete_Z m_VertexBufferPtrDA[i];
    }
    for (S32 i = 0; i < m_IndexBufferPtrDA.GetSize(); i++) {
        m_IndexBufferPtrDA[i]->Release();
        Delete_Z m_IndexBufferPtrDA[i];
    }
    m_VertexBufferPtrDA.Flush();
    m_IndexBufferPtrDA.Flush();
}

void Renderer_Z::SetSize(S32 i_SizeX, S32 i_SizeY) {
    m_SizeX = i_SizeX;
    m_SizeY = i_SizeY;
    m_Viewports[6].SetPosAndSize(0, 0, i_SizeX, i_SizeY);
    m_Viewports[6].SetRenderer(this);
}

void Renderer_Z::InitViewport(U32 i_NbViewports) {
    S32 l_SizeX;
    S32 l_SizeY;
    switch (i_NbViewports) {
        case 1:
            m_NbViewports = 1;
            m_Viewports[0].SetPosAndSize(0, 0, m_SizeX, m_SizeY);
            break;
        case 2:
            m_NbViewports = 2;
            if (GetScreenRatio() == RATIO_SCREEN_WIDESCREEN) {
                l_SizeX = m_SizeX >> 1;
                m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, m_SizeY);
                m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, m_SizeY);
            }
            else {
                S32 l_SizeY = m_SizeY >> 1;
                m_Viewports[0].SetPosAndSize(0, 0, m_SizeX, l_SizeY - 1);
                m_Viewports[1].SetPosAndSize(0, l_SizeY + 1, m_SizeX, l_SizeY - 1);
            }
            break;
        case 3: {
            m_NbViewports = 3;
            S32 l_SizeX = m_SizeX >> 1;
            S32 l_SizeY = m_SizeY >> 1;
            m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[2].SetPosAndSize(0, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
        } break;
        case 4: {
            m_NbViewports = 4;
            S32 l_SizeX = m_SizeX >> 1;
            S32 l_SizeY = m_SizeY >> 1;
            m_Viewports[0].SetPosAndSize(0, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[1].SetPosAndSize(l_SizeX + 1, 0, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[2].SetPosAndSize(0, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
            m_Viewports[3].SetPosAndSize(l_SizeX + 1, l_SizeY + 1, l_SizeX - 1, l_SizeY - 1);
            break;
        }
    }
    for (S32 i = 0; i < m_NbViewports; i++) {
        m_Viewports[i].SetRenderer(this);
    }
}

void Renderer_Z::SetActiveViewport(S32 i_ViewportId) {
    SetCurrentFogColor(VEC3F_NULL);
    FlushActiveViewport();
    m_ActiveViewport = i_ViewportId;
    Viewport_Z& l_Viewport = GetViewport(i_ViewportId);
    m_HSize = l_Viewport.GetHSize();
    m_VSize = l_Viewport.GetVSize();
    m_HCenter = l_Viewport.GetHCenter();
    m_VCenter = l_Viewport.GetVCenter();
    Node_Z* l_CameraNode = GetViewport(i_ViewportId).GetCamera();
    if (l_CameraNode) {
        Camera_Z* l_Camera = (Camera_Z*)(l_CameraNode->GetObject(FALSE));
        l_Camera->UpdateInverseWorldMatrix(l_CameraNode);
    }
}

void Renderer_Z::FlushActiveViewport() { }

void Renderer_Z::Draw(S32 i_ViewportId, Float i_DeltaTime) {
    SetActiveViewport(i_ViewportId);

    DrawInfo_Z l_DrawInfo;
    l_DrawInfo.m_VpId = i_ViewportId;
    l_DrawInfo.m_FirstPlayerVpId = -1;
    l_DrawInfo.m_VpCount = -1;
    l_DrawInfo.m_Unk0_0x17c0_From_Renderer_0x704 = m_UnkBoolFalse_0x704;
    l_DrawInfo.m_ParticlesFadeDist = m_ParticlesFadeDist;
    l_DrawInfo.m_LodShadowFadeDist = m_LodShadowFadeDist;
    l_DrawInfo.m_LodFadeDist = m_LodFadeDist;
    l_DrawInfo.m_LodDist = m_LodDist;
    l_DrawInfo.m_LodPatchMin = m_LodPatchMin;
    l_DrawInfo.m_LodPatchMax = m_LodPatchMax;
    l_DrawInfo.m_LodPatchDist = m_LodPatchDist;
    l_DrawInfo.m_DeltaTime = i_DeltaTime;
    m_Viewports[i_ViewportId].Draw(l_DrawInfo);

    if (i_ViewportId == GLOBAL_DEBUG_VIEWPORT) {
        SetActiveMaterial(NULL);
        gData.GameMgr->DebugDisplay(&GetViewport(GLOBAL_DEBUG_VIEWPORT));
        gData.ScriptMgr->Draw(l_DrawInfo);
        gData.SoundMgr->Draw(l_DrawInfo);
        gData.ClassMgr->Draw(l_DrawInfo);

        m_CurFps = 1.0f / i_DeltaTime;
        FastSmooth(m_Fps, m_CurFps, 1.1f, i_DeltaTime, m_Fps);

        U32 l_FpsThreshold = 59;
        if (gData.m_GameFlag & FL_GAME_2_FRAMES) {
            l_FpsThreshold = 29;
        }

        if ((gData.m_EngineFlag & FL_DISPLAY_FPS) && m_Fps < (Float)l_FpsThreshold) {
            Color l_BorderColor;
            l_BorderColor.Set(1.0f, 0.0f, 0.0f, 0.5f);
            Vec2f l_P0;
            Vec2f l_P1;
            l_P0.Set(0.0f, 0.0f);
            l_P1.Set((Float)m_SizeX, 16.0f);
            DrawQuad(l_P0, l_P1, l_BorderColor, 0.1f);
            l_P0.Set((Float)(m_SizeX - 16), 16.0f);
            l_P1.Set((Float)m_SizeX, (Float)(m_SizeY - 16));
            DrawQuad(l_P0, l_P1, l_BorderColor, 0.1f);
            l_P0.Set(0.0f, (Float)(m_SizeY - 16));
            l_P1.Set((Float)m_SizeX, (Float)m_SizeY);
            DrawQuad(l_P0, l_P1, l_BorderColor, 0.1f);
            l_P0.Set(0.0f, 16.0f);
            l_P1.Set(16.0f, (Float)(m_SizeY - 16));
            DrawQuad(l_P0, l_P1, l_BorderColor, 0.1f);
        }

        if (gData.m_EngineFlag & FL_DISPLAY_FPS) {
            S32 l_SizeX;
            S32 l_SizeY;
            GetSize(l_SizeX, l_SizeY);
            Vec2f l_TextPos((Float)(l_SizeX - 210), (Float)(l_SizeY - 24));
            Vec2f l_BgPos(l_TextPos);

            CacheState_Z l_MatState;
            gData.MatrixBuffer->GetState(l_MatState);

            S32 l_MatInUse;
            S32 l_LightC1;
            S32 l_LightC2;
            S32 l_PatchCD;
            U32 l_GblDma;
            U32 l_GifDma;
            Float l_Timer = m_DisplayCacheStateUpdateTimer;
            if (l_Timer < 0.0f) {
                m_DisplayCacheStateUpdateTimer = l_Timer - i_DeltaTime;
                l_LightC1 = m_LightCacheState1.m_MaxInUseNb;
                l_LightC2 = m_LightCacheState2.m_MaxInUseNb;
                l_PatchCD = m_PatchCacheState.m_MaxInUseNb;
                l_GblDma = m_GblDmaMaxInUseNb;
                l_GifDma = m_GifDmaMaxInUseNb;
                l_MatInUse = l_MatState.m_MaxInUseNb;
            }
            else {
                m_DisplayCacheStateUpdateTimer = l_Timer + i_DeltaTime;
                l_LightC1 = m_LightCacheState1.m_InUseNb;
                l_LightC2 = m_LightCacheState2.m_InUseNb;
                l_PatchCD = m_PatchCacheState.m_InUseNb;
                l_GblDma = m_GblDmaInUseNb;
                l_GifDma = m_GifDmaInUseNb;
                l_MatInUse = l_MatState.m_InUseNb;
            }

            m_DisplayFpsUpdateTimer = m_DisplayFpsUpdateTimer - i_DeltaTime;
            Float l_GpuTarget = 1.0f / Clamp(m_GpuTargetMsOrFps, 0.0001f, 1.0f);
            FastSmooth(m_GpuFps, l_GpuTarget, 1.1f, i_DeltaTime, m_GpuFps);
            Float l_CpuTarget = 1.0f / Clamp(m_CpuTargetMsOrFps, 0.0001f, 1.0f);
            FastSmooth(m_CpuFps, l_CpuTarget, 1.1f, i_DeltaTime, m_CpuFps);

            if (m_DisplayFpsUpdateTimer <= 0.0f) {
                m_DisplayFpsUpdateTimer = 0.1f;
                Float l_Time = gData.m_Timer;
                U32 l_Sec = (U32)l_Time;
                U32 l_Min = (U32)(l_Time / 60.0f);
                U32 l_Hour = (U32)(l_Time / 3600.0f);
                m_TimeString.Sprintf("Time: %2dh%02dm%02ds", l_Hour % 3600, l_Min % 60, l_Sec % 60);

                const Char* l_VsyncStr = "";
                if (m_EffectFlag & FL_EFFECT_VSYNC) {
                    l_VsyncStr = "(vsync)";
                }
                m_FpsString.Sprintf("Fps: %.1f (%.2fms) %s", l_VsyncStr, m_Fps, 1000.0f / m_Fps);
                m_GpuString.Sprintf("Gpu: %.1f (%.2fms) %2.f%%", l_VsyncStr, m_GpuFps, 1000.0f / m_GpuFps, 100.0f * GetFreq() / m_GpuFps);
                m_CpuString.Sprintf("Cpu: %.1f (%.2fms) %2.f%%", l_VsyncStr, m_CpuFps, 1000.0f / m_CpuFps, 100.0f * GetFreq() / m_CpuFps);
                m_FragmentsString.Sprintf("Fragments: %d", MemManager.GetFragments());
                Float l_Largest = (Float)MemManager.GetLargestFree() / 1048576.0f;
                m_LargestBlockString.Sprintf("Largest Block: %d.%d Mo", (S32)l_Largest, (S32)(10.0f * l_Largest) - (S32)l_Largest * 10);
                Float l_FreeMem = (Float)MemManager.GetFreeMem() / 1048576.0f;
                m_FreeMemString.Sprintf("Free Mem: %d.%d Mo", (S32)l_FreeMem, (S32)(10.0f * l_FreeMem) - (S32)l_FreeMem * 10);
                m_MatrixUsageString.Sprintf("Mat: %d/%d", l_MatInUse, l_MatState.m_TotalNb);
                m_LightC1String.Sprintf("LightC1: %d/%d", l_LightC1, m_LightCacheState1.m_TotalNb);
                m_LightC2String.Sprintf("LightC2: %d/%d", l_LightC2, m_LightCacheState2.m_TotalNb);
                m_PatchCDString.Sprintf("PatchCD: %d/%d", l_PatchCD, m_PatchCacheState.m_TotalNb);
                m_GlobalDmaString.Sprintf("GblDMA: %d/%dk", (S32)l_GblDma >> 10, (S32)m_GblDmaTotalNb >> 10);
                m_GifDmaString.Sprintf("GifDMA: %d/%dk", (S32)l_GifDma >> 10, (S32)m_GifDmaTotalNb >> 10);
            }

            Color l_White(1.0f, 1.0f, 1.0f, 1.0f);
            Float l_Near = GetDefaultNear();
            DrawString(l_TextPos, m_TimeString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_FpsString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_FragmentsString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_LargestBlockString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_FreeMemString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, gData.SoundMgr->GetUsedTrackString(), l_White, l_Near, 1.0f);

            l_TextPos.x = l_TextPos.x - 4.0f;
            l_BgPos.y = l_BgPos.y + 12.0f;
            l_TextPos.y = (l_TextPos.y - 8.0f) + 4.0f;
            l_BgPos.x = l_BgPos.x + 204.0f;
            Color l_BgColor(0.1f, 0.2f, 0.2f, 0.6f);
            DrawQuad(l_TextPos, l_BgPos, l_BgColor, (Float)(1e-06f + l_Near));

            if (m_DisplayCacheStateUpdateTimer < 0.0f) {
                l_BgColor.Set(1.0f, 0.0f, 0.0f, 0.4f);
            }
            if (Abs(m_DisplayCacheStateUpdateTimer) > 1.0f) {
                S32 l_Sign;
                if (0.0f == m_DisplayCacheStateUpdateTimer) {
                    l_Sign = 0;
                }
                else if (m_DisplayCacheStateUpdateTimer >= 0.0f) {
                    l_Sign = 1;
                }
                else {
                    l_Sign = -1;
                }
                m_DisplayCacheStateUpdateTimer = 1e-06f * (Float)(-l_Sign);
            }

            l_TextPos.Set((Float)(l_SizeX - 360), (Float)(l_SizeY - 24));
            l_BgPos = l_TextPos;
            DrawString(l_TextPos, m_MatrixUsageString, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_LightC1String, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_LightC2String, l_White, l_Near, 1.0f);
            l_TextPos.y = l_TextPos.y - 8.0f;
            DrawString(l_TextPos, m_PatchCDString, l_White, l_Near, 1.0f);
            l_TextPos.x = l_TextPos.x - 4.0f;
            l_TextPos.y = (l_TextPos.y - 8.0f) + 4.0f;
            l_BgPos.y = l_BgPos.y + 12.0f;
            l_BgPos.x = l_BgPos.x + 148.0f;
            DrawQuad(l_TextPos, l_BgPos, l_BgColor, (Float)(0.001f + l_Near));
        }

        if (gData.m_EngineFlag & FL_INGAME_CONSOLE) {
            Char* l_Msg = gData.Cons->GetMessageBuffer();
            S32 l_End = strlen(l_Msg) - 1;
            S32 l_Line = 0;
            for (Char* l_Cur = l_Msg + l_End; l_End > 0 && (*l_Cur == '\n' || *l_Cur == '\r'); l_Cur--) {
                l_End--;
            }
            Color l_MsgColor(0.4f, 0.4f, 0.4f, 1.0f);
            if (gData.Cons->IsFlagEnable(FL_CONSOLE_PAUSED)) {
                l_MsgColor.g = 0.0f;
                l_MsgColor.b = 0.0f;
            }
            while (l_End > 0) {
                S32 l_Start = l_End - 1;
                for (Char* l_Cur = l_Msg + l_Start; l_Start != 0 && *l_Cur != '\n' && *l_Cur != '\r'; l_Cur--) {
                    l_Start--;
                }
                if (l_Start != l_End) {
                    U32 l_Len = (l_End - l_Start) + 1;
                    if (l_Len > 63) {
                        l_Len = 63;
                    }
                    Char l_Buf[76];
                    strncpy(l_Buf, l_Msg + l_Start, l_Len);
                    l_Buf[l_Len] = '\0';
                    Vec2f l_MsgPos(10.0f, 10.0f * (Float)(U32)(20 - l_Line) + 100.0f);
                    DrawString(l_MsgPos, l_Buf, l_MsgColor, 0.1f, 1.0f);
                    l_Line++;
                }
                if (l_Line == 20) {
                    break;
                }
                l_End = l_Start - 1;
            }
        }

        gData.Cons->Draw(l_DrawInfo);
    }
}

void Renderer_Z::Draw(Float i_DeltaTime) {
    for (S32 i = 0; i < GetNbViewport(); i++) {
        Draw(i, i_DeltaTime);
    }
    Draw(GLOBAL_DEBUG_VIEWPORT, i_DeltaTime);
}

void Renderer_Z::BeginRender() {
    m_ActiveMaterial = NULL;
    for (S32 i = 0; i < BITMAP_NB_TYPES; i++) {
        m_ActiveBitmaps[i] = NULL;
    }
}

void Renderer_Z::EndRender(Float i_DeltaTime) {
    FlushActiveViewport();
    if (!i_DeltaTime) {
        return;
    }
    for (S32 i = 0; i < GetNbViewport(); i++) {
        World_Z* l_VpWorld = GetViewport(i).GetWorld();
        if (l_VpWorld) {
            l_VpWorld->EndDraw();
        }
    }
}

void Renderer_Z::SetBlankMaterial() {
    SetActiveMaterial(gData.SystemDatas->GetDefaultMaterial());
}

// TODO: Implement commands below

Bool SetSplitType() {
    return TRUE;
}

Bool SetWorldToSplit() {
    return TRUE;
}

Bool SetLodRender() {
    S32 l_Nb = gData.Cons->GetNbParam();
    if (l_Nb != 2) {
        return TRUE;
    }
    if (!gData.Cons->IsParamFloat(1)) {
        return TRUE;
    }
    U32 l_Lod = gData.Cons->GetParamFloat(1);
    if (l_Lod > 8 || l_Lod < 1) {
        return TRUE;
    }
    gData.MainRdr->SetLodPatchMax(l_Lod);
    return TRUE;
}

Bool SetLIghtingType() {
    return TRUE;
}

Bool SetLOdTexture() {
    return TRUE;
}

Bool SetTExtureFiltering() {
    return TRUE;
}

Bool SetMAxAnisotropy() {
    return TRUE;
}

Bool FitOnObject() {
    return TRUE;
}

Bool DisplayImage() {
    return TRUE;
}

Bool MakeAvi() {
    return TRUE;
}

Bool CloseAvi() {
    return TRUE;
}

Bool StartRenderBench() {
    return TRUE;
}

#pragma dont_inline reset

Float Renderer_Z::Renderer_ZDefaultNear = 0.0f;

Float Renderer_Z::ScreenRatio = 0.0f;

void Renderer_Z::SetLightFactor(Float a1, Float a2, Float a3) {
}

void Renderer_Z::GetLightFactor(Float& a1, Float& a2, Float& a3) {
}

void Renderer_Z::ApplyDirectionalOnVertex(Vertex3D* a1, Vec3f& a2, LightData_Z& a3) {
}

Float Renderer_Z::Renderer_ZCurrentNear = 0.0f;

void Renderer_Z::ApplyFogOnVertex(Vertex3D* a1, Vec4f& a2) {
}
