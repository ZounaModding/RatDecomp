#include "GCRenderer_Z.h"
#include "FlareGC_Z.h"

ExternC_Z void exit(int);

S32 GCRenderer_Z::PushADraw(StreamList_Z* i_StreamList, BaseDisplayList_Z* i_DisplayList, S32 i_RenderContextFlag) {
    S32 l_DrawCallIdx = m_DrawCalls.Add();
    ExtPrimitiveInfo_Z& l_DrawCall = m_DrawCalls[l_DrawCallIdx];
    m_DrawOrderGroupDrawCallCount[m_PushedDrawOrderGroup]++;

    U32 l_Order;
    if (m_PushedDrawOrderGroup == ds_opaque) {
        l_Order = 0;
    }
    else {
        l_Order = (U32)(65535.0f / (1.0f + m_PushedOrder));
    }

    l_DrawCall.m_Order = l_Order;

    l_DrawCall.m_DrawOrderGroup = m_PushedDrawOrderGroup;
    l_DrawCall.m_DrawState = m_PushedDrawState;
    l_DrawCall.m_UnusedSortKeyInsideGroup_0x78 = m_PushedUnusedSortKeyInsideGroup_0x2672;
    l_DrawCall.m_StreamList = i_StreamList;
    l_DrawCall.m_DisplayList = i_DisplayList;
    l_DrawCall.m_Material = m_ActiveMaterial;
    if (i_RenderContextFlag == -1) {
        l_DrawCall.m_RenderContextFlag = m_RenderContextFlag & ~FL_RDR_CONTEXT_LIGHT_MASK;
    }
    else {
        l_DrawCall.m_RenderContextFlag = i_RenderContextFlag;
    }

    l_DrawCall.m_BlendFlag = m_CurBlendFlags;
    l_DrawCall.m_DirectionalLight = m_CurDirectionalLight;
    l_DrawCall.m_AmbientColor = m_CurAmbientColor;
    l_DrawCall.m_ObjDatasColor.x = m_CurObjColor.x;
    l_DrawCall.m_ObjDatasColor.y = m_CurObjColor.y;
    l_DrawCall.m_ObjDatasColor.z = m_CurObjColor.z;
    l_DrawCall.m_FadeValue = m_CurObjColor.w;
    l_DrawCall.m_OmniLightCount = m_CurOmniLightCount;
    l_DrawCall.m_OmniLightMask = m_CurOmniLightMask;
    builtin_memcpy(l_DrawCall.m_OmniLights, m_CurOmniLights, sizeof(l_DrawCall.m_OmniLights));
    l_DrawCall.m_MainFog = m_CurMainFog;
    l_DrawCall.m_EnabledFog = m_CurEnabledFog;
    l_DrawCall.m_FogNear = m_FogNear;
    l_DrawCall.m_FogFar = m_FogFar;
    l_DrawCall.m_FogStartZ = m_FogStartZ;
    l_DrawCall.m_FogEndZ = m_FogEndZ;
    PSMTXCopy(*(Mtx*)&m_Local2Cam, l_DrawCall.m_Local2Cam);
    PSMTXCopy(*(Mtx*)&m_InvTransposed, l_DrawCall.m_InvTransposed);
    l_DrawCall.m_MtxId = m_CurMtxId;
    l_DrawCall.m_MtxKey = m_CurMtxKey;
    l_DrawCall.m_ActiveBitmaps[BITMAP_RADIOSITY] = m_ActiveBitmaps[BITMAP_RADIOSITY];
    l_DrawCall.m_PrevDrawCallIdx = -1;
    l_DrawCall.m_NextDrawCallIdx = -1;
    return l_DrawCallIdx;
}

S32 GCRenderer_Z::PushLinkedDraw(S32 i_LinkedDrawCallIdx, StreamList_Z* i_StreamList, BaseDisplayList_Z* i_DisplayList) {
    S32 l_NewDrawCallIdx = PushADraw(i_StreamList, i_DisplayList, -1);
    if (i_LinkedDrawCallIdx >= 0) {
        m_DrawCalls[i_LinkedDrawCallIdx].m_NextDrawCallIdx = l_NewDrawCallIdx;
    }
    m_DrawCalls[l_NewDrawCallIdx].m_PrevDrawCallIdx = i_LinkedDrawCallIdx;
    return l_NewDrawCallIdx;
}

S32 GCRenderer_Z::PushVizQuery(DrawInfo_Z& i_DrawInfo, const Vec3f& i_VertexPosScreen) {
    DisplayList_Z* l_DisplayList;
    S32 l_Result = m_VizQueryDisplayListCount;
    if (l_Result == VIZ_QUERY_DISPLAY_LIST_SIZE) {
        return -1;
    }

    m_VizQueryDisplayListCount++;
    l_DisplayList = &m_VizQueryDisplayLists[l_Result];
    l_DisplayList->Begin();
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT7, 4);

    Float l_Z = -i_VertexPosScreen.z;
    Float l_Size = 8.0f * i_VertexPosScreen.z / i_DrawInfo.m_Vp->GetHSize();

    GXPosition3f32(i_VertexPosScreen.x - l_Size, i_VertexPosScreen.y - l_Size, l_Z);
    GXPosition3f32(i_VertexPosScreen.x + l_Size, i_VertexPosScreen.y - l_Size, l_Z);
    GXPosition3f32(i_VertexPosScreen.x - l_Size, i_VertexPosScreen.y + l_Size, l_Z);
    GXPosition3f32(i_VertexPosScreen.x + l_Size, i_VertexPosScreen.y + l_Size, l_Z);

    l_DisplayList->End();
    l_Result = PushADraw(&FlareDataGC_Z::VizQueryStreamList, l_DisplayList, -1);
    m_DrawCalls[l_Result].m_RenderContextFlag = FL_RDR_CONTEXT_FLAT_COLOR;
    return l_Result;
}

Bool GCRenderer_Z::DrawExtPrimitive(ExtPrimitiveInfo_Z* i_ExtPrimInfo) {
    return FALSE;
}

// TODO: Finish matching
U16 GCRenderer_Z::SortRendererDatas(SortElem_Z* i_List) {
    U16 l_Head[256];
    U16 l_Tail[256];

    for (int i = 0; i < 256; i++) {
        l_Head[i] = INVALID_SORT_ELEM_IDX;
    }

    U16 l_CurIdx = 0;

    for (int i = 3; i >= 0; i--) {
        while (l_CurIdx != INVALID_SORT_ELEM_IDX) {
            U32 l_Key = ((U8*)&i_List[l_CurIdx].m_Key)[i];

            if (l_Head[l_Key] == INVALID_SORT_ELEM_IDX) {
                l_Head[l_Key] = l_CurIdx;
                l_Tail[l_Key] = l_CurIdx;
            }
            else {
                i_List[l_Tail[l_Key]].m_NextElemIdx = l_CurIdx;
                l_Tail[l_Key] = l_CurIdx;
            }

            l_CurIdx = i_List[l_CurIdx].m_NextElemIdx;
        }

        l_CurIdx = INVALID_SORT_ELEM_IDX;

        for (int j = 255; j >= 0; j--) {
            if (l_Head[j] != INVALID_SORT_ELEM_IDX) {
                i_List[l_Tail[j]].m_NextElemIdx = l_CurIdx;
                l_CurIdx = l_Head[j];
                l_Head[j] = INVALID_SORT_ELEM_IDX;
            }
        }
    }

    return l_CurIdx;
}

void GCRenderer_Z::DrawTransparent(DrawInfo_Z& i_DrawInfo) {
    S32 i;
    m_Draw3D.End();

    m_CurMtxKey = -1;
    m_CurDrawOrderGroup = do_none;
    m_UnkU8_SetTo0_0x2673 = 0;

    for (i = 0; i < do_count; i++) {
        m_DrawOrderGroupShouldDraw[i] = TRUE;
    }

    U32 l_FrameBufferEffectFlag = i_DrawInfo.m_Flag & 4;

    m_DrawOrderGroupShouldDraw[do_postproc] = i_DrawInfo.m_VpId < 6 && l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    m_DrawOrderGroupShouldDraw[do_shadow_cast] = i_DrawInfo.m_VpId < 6 && l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    m_DrawOrderGroupShouldDraw[do_unk_14] = FALSE;
    m_DrawOrderGroupShouldDraw[do_unk_13] = m_DrawOrderGroupShouldDraw[do_unk_14];

    m_DrawOrderGroupShouldDraw[do_last] = l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    S32 l_DrawCallCount = m_DrawCalls.GetSize();

    if (l_DrawCallCount != 0 && m_FrameBufferIdx >= 2) {
        SortElem_Z l_SortElems[MAX_DRAW_CALLS_PER_FRAME];

        S32 l_DrawCallsToProcess = Min(l_DrawCallCount, MAX_DRAW_CALLS_PER_FRAME);

        SortElem_Z* l_CurElem = l_SortElems;
        ExtPrimitiveInfo_Z* l_DrawCalls = m_DrawCalls.GetArrayPtr();
        ExtPrimitiveInfo_Z* l_CurDrawCall = l_DrawCalls;

        U16 l_NextIdx = 1;
        while (l_DrawCallsToProcess--) {
            U8* l_Key = (U8*)&l_CurElem->m_Key;
            l_Key[3] = l_CurDrawCall->m_Order;
            l_Key[2] = (l_CurDrawCall->m_Order >> 8);
            l_Key[1] = l_CurDrawCall->m_UnusedSortKeyInsideGroup_0x78;
            l_Key[0] = l_CurDrawCall->m_DrawOrderGroup;
            l_CurElem->m_NextElemIdx = l_NextIdx++;
            l_CurElem++;
            l_CurDrawCall++;
        }
        l_CurElem--;
        l_CurElem->m_NextElemIdx = INVALID_SORT_ELEM_IDX;

        U16 l_ItemId = SortRendererDatas(l_SortElems);

        while (l_ItemId != INVALID_SORT_ELEM_IDX) {
            S32 l_LastItemId = l_ItemId & INVALID_SORT_ELEM_IDX;
            ExtPrimitiveInfo_Z& l_DrawCall = l_DrawCalls[l_ItemId];
            if (l_DrawCall.m_PrevDrawCallIdx < 0 && m_DrawOrderGroupShouldDraw[l_DrawCall.m_DrawOrderGroup]) {
                DrawOrder(i_DrawInfo, l_DrawCall.m_DrawOrderGroup);
                for (;;) {
                    if (!DrawExtPrimitive(&l_DrawCalls[l_ItemId])) {
                        break;
                    }
                    if (m_DrawCalls[l_ItemId].m_NextDrawCallIdx < 0) {
                        break;
                    }
                    l_ItemId = m_DrawCalls[l_ItemId].m_NextDrawCallIdx;
                }
            }

            l_ItemId = l_SortElems[l_LastItemId].m_NextElemIdx;
        }
    }

    DrawOrder(i_DrawInfo, do_count);
    DrawState(ds_opaque);
    NoFog();
    NoOmnis();
    GXDrawDone();

    m_Draw2D.End();
    m_Draw3D.SwitchBuffer();

    m_DrawCalls.Empty();

    m_VizQueryDisplayListCount = 0;

    for (i = 0; i < do_count; i++) {
        m_DrawOrderGroupDrawCallCount[i] = 0;
    }

    InitBlock(i_DrawInfo);
}

// TODO: Finish matching
void GCRenderer_Z::DrawOrder(DrawInfo_Z& i_DrawInfo, U8 i_Order) {
    Vec2f l_UVMin;
    Vec2f l_UVMax;
    Vec2f l_PosMin;
    Vec2f l_PosMax;
    Vec2f l_Size;
    Color l_Color;

    if (i_Order == m_CurDrawOrderGroup) {
        return;
    }

    if (i_Order < m_CurDrawOrderGroup) {
        m_CurDrawOrderGroup = i_Order - 1;
    }

    while (m_CurDrawOrderGroup < i_Order) {
        m_ActiveMaterial = NULL;
        m_CurDrawOrderGroup++;

        S32 l_CurDrawOrderGroup = m_CurDrawOrderGroup;

        if (l_CurDrawOrderGroup < do_postproc) {
            if (l_CurDrawOrderGroup == do_scene_draw) {
                goto DrawSceneDraw;
            }

            continue;
        }

        if (l_CurDrawOrderGroup == do_global_screen_fx) {
            goto DrawFrameBufferEffects;
        }

        continue;

    DrawSceneDraw:
        if ((U32)m_DrawOrderGroupDrawCallCount[do_shadow_cast] != 0) {
            SetActiveMaterial(NULL);

            SetRenderContext(FL_RDR_CONTEXT_VERTEX_COLOR_TEX_ALPHA);
            SetRenderBlendOp(FL_MTL_RDR_SOUSTRACTIF);
            m_CurBlendFlags = -1;
            DrawState(ds_cwrite | ds_ablend | ds_aref128);

            DisableFog();
            NoOmnis();

            Float l_VpSizeY = i_DrawInfo.m_VpSizeY;
            Float l_VpSizeX = i_DrawInfo.m_VpSizeX;

            l_Size.x = l_VpSizeX;
            l_Size.y = l_VpSizeY;

            l_UVMin.x = 0.0f;
            l_UVMin.y = 0.0f;

            l_UVMax.x = l_VpSizeX;
            l_UVMax.y = l_VpSizeY;

            Float l_VpStartY = i_DrawInfo.m_VpStartY;
            Float l_VpStartX = i_DrawInfo.m_VpStartX;

            Float l_PosMaxY = l_VpStartY + l_VpSizeY;
            Float l_PosMaxX = l_VpStartX + l_VpSizeX;

            l_PosMin.x = l_VpStartX;
            l_PosMin.y = l_VpStartY;

            l_PosMax.x = l_PosMaxX;
            l_PosMax.y = l_PosMaxY;

            l_Color.r = 0.1f;
            l_Color.g = 0.1f;
            l_Color.b = 0.1f;
            l_Color.a = 1.0f;

            GXSetCopyFilter(FALSE, NULL, FALSE, m_RenderModeObj.vfilter);

            GXSetTexCopySrc(
                (U16)i_DrawInfo.m_VpStartX,
                (U16)i_DrawInfo.m_VpStartY,
                (U16)i_DrawInfo.m_VpSizeX,
                (U16)i_DrawInfo.m_VpSizeY
            );

            GXInitTexObj(
                &m_FrameBufferTexObj,
                m_FrameBufferTextureData,
                (U16)i_DrawInfo.m_VpSizeX,
                (U16)i_DrawInfo.m_VpSizeY,
                GX_TF_RGBA8,
                GX_CLAMP,
                GX_CLAMP,
                FALSE
            );

            GXInitTexObjLOD(
                &m_FrameBufferTexObj,
                GX_LINEAR,
                GX_NEAR,
                0.0f,
                0.0f,
                0.0f,
                FALSE,
                FALSE,
                GX_ANISO_1
            );

            GXSetCopyClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));

            GXSetTexCopyDst(
                GXGetTexObjWidth(&m_FrameBufferTexObj),
                GXGetTexObjHeight(&m_FrameBufferTexObj),
                GX_TF_RGBA8,
                FALSE
            );

            GXCopyTex(m_FrameBufferTextureData, FALSE);
            GXPixModeSync();

            GXSetCopyFilter(FALSE, m_RenderModeObj.sample_pattern, TRUE, m_RenderModeObj.vfilter);

            GXLoadTexObj(&m_FrameBufferTexObj, GX_TEXMAP0);

            ImmediatQuad(l_UVMin, l_UVMax, l_PosMin, l_PosMax, l_Size, l_Color, 1.0f);
        }

        continue;

    DrawFrameBufferEffects:
        if (m_DrawOrderGroupShouldDraw[do_postproc] && (m_EffectFlag & (FL_EFFECT_SPECIAL_VISION | FL_EFFECT_BLOOM | FL_EFFECT_RADIAL_MOTION_BLUR)) != 0) {
            SetActiveMaterial(NULL);

            GXSetCopyFilter(FALSE, NULL, FALSE, m_RenderModeObj.vfilter);

            GXSetTexCopySrc(
                (U16)i_DrawInfo.m_VpStartX,
                (U16)i_DrawInfo.m_VpStartY,
                (U16)i_DrawInfo.m_VpSizeX,
                (U16)i_DrawInfo.m_VpSizeY
            );

            GXInitTexObj(
                &m_FrameBufferTexObj,
                m_FrameBufferTextureData,
                (U16)(i_DrawInfo.m_VpSizeX * 0.5f),
                (U16)(i_DrawInfo.m_VpSizeY * 0.5f),
                GX_TF_RGBA8,
                GX_CLAMP,
                GX_CLAMP,
                FALSE
            );

            GXInitTexObjLOD(
                &m_FrameBufferTexObj,
                GX_LINEAR,
                GX_NEAR,
                0.0f,
                0.0f,
                0.0f,
                FALSE,
                FALSE,
                GX_ANISO_1
            );

            GXSetCopyClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));

            GXSetTexCopyDst(
                GXGetTexObjWidth(&m_FrameBufferTexObj),
                GXGetTexObjHeight(&m_FrameBufferTexObj),
                GX_TF_RGBA8,
                TRUE
            );

            GXCopyTex(m_FrameBufferTextureData, FALSE);
            GXPixModeSync();

            GXSetCopyFilter(FALSE, m_RenderModeObj.sample_pattern, TRUE, m_RenderModeObj.vfilter);

            GXLoadTexObj(&m_FrameBufferTexObj, GX_TEXMAP0);

            if (m_EffectFlag & FL_EFFECT_SPECIAL_VISION) {
                SpecialWarpZone(i_DrawInfo);
            }

            if (m_EffectFlag & FL_EFFECT_RADIAL_MOTION_BLUR) {
                RadialMBlur(i_DrawInfo);
            }

            if (m_EffectFlag & FL_EFFECT_BLOOM) {
                SetRenderBlendOp(FL_MTL_RDR_ADDITIF);
                SetRenderContext(FL_RDR_CONTEXT_BLOOM);

                DisableFog();
                NoOmnis();

                DrawState(ds_cwrite | ds_noatest);

                l_Color.r = 0.5f;
                l_Color.g = 0.5f;
                l_Color.b = 0.5f;
                l_Color.a = 1.0f;

                Float l_VpSizeY = i_DrawInfo.m_VpSizeY;
                Float l_VpSizeX = i_DrawInfo.m_VpSizeX;

                l_Size.y = l_VpSizeY * 0.5f;
                l_Size.x = l_VpSizeX * 0.5f;
                l_UVMin.x = 0.0f;
                l_UVMin.y = 0.0f;
                l_UVMax.x = l_VpSizeX * 0.5f;
                l_UVMax.y = l_VpSizeY * 0.5f;

                Float l_VpStartY = i_DrawInfo.m_VpStartY;
                Float l_VpStartX = i_DrawInfo.m_VpStartX;

                l_PosMin.x = l_VpStartX;
                l_PosMin.y = l_VpStartY;

                l_PosMax.x = l_VpStartX + l_VpSizeX;
                l_PosMax.y = l_VpStartY + l_VpSizeY;

                GXLoadTexObj(&m_FrameBufferTexObj, GX_TEXMAP0);

                ImmediatQuad(l_UVMin, l_UVMax, l_PosMin, l_PosMax, l_Size, l_Color, 1.0f);
            }
        }
    }
}

// TODO: Finish matching
void GCRenderer_Z::ImmediatQuad(
    const Vec2f& i_UVMin,
    const Vec2f& i_UVMax,
    const Vec2f& i_PosMin,
    const Vec2f& i_PosMax,
    const Vec2f& i_Size,
    const Color& i_Color,
    Float i_Z
) {
    GCImmediateVertex_Z l_Vertices[4];
    Float l_InvSizeX = 1.0f / i_Size.x;
    Float l_InvSizeY = 1.0f / i_Size.y;

    l_Vertices[0].m_Position.x = i_PosMin.x;
    l_Vertices[0].m_Position.y = i_PosMin.y;
    l_Vertices[0].m_TextureCoordinates.x = i_UVMin.x * l_InvSizeX;
    l_Vertices[0].m_TextureCoordinates.y = i_UVMin.y * l_InvSizeY;

    l_Vertices[1].m_Position.x = i_PosMax.x;
    l_Vertices[1].m_Position.y = i_PosMin.y;
    l_Vertices[1].m_TextureCoordinates.x = i_UVMax.x * l_InvSizeX;
    l_Vertices[1].m_TextureCoordinates.y = i_UVMin.y * l_InvSizeY;

    l_Vertices[2].m_Position.x = i_PosMax.x;
    l_Vertices[2].m_Position.y = i_PosMax.y;
    l_Vertices[2].m_TextureCoordinates.x = i_UVMax.x * l_InvSizeX;
    l_Vertices[2].m_TextureCoordinates.y = i_UVMax.y * l_InvSizeY;

    l_Vertices[3].m_Position.x = i_PosMin.x;
    l_Vertices[3].m_Position.y = i_PosMax.y;
    l_Vertices[3].m_TextureCoordinates.x = i_UVMin.x * l_InvSizeX;
    l_Vertices[3].m_TextureCoordinates.y = i_UVMax.y * l_InvSizeY;

    U8 l_Red = (U8)(255.0f * i_Color.r);
    U8 l_Green = (U8)(255.0f * i_Color.g);
    U8 l_Blue = (U8)(255.0f * i_Color.b);
    U8 l_Alpha = (U8)(255.0f * i_Color.a);

    GXSetCurrentMtx(GX_IDENTITY);
    GXSetProjection(*(Mtx44*)&m_OrthoMatrix, GX_ORTHOGRAPHIC);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT4, 4);

    for (S32 i = 0; i < 4; i++) {
        GXPosition3f32(l_Vertices[i].m_Position.x, l_Vertices[i].m_Position.y, -i_Z);
        GXColor4u8(l_Red, l_Green, l_Blue, l_Alpha);
        GXTexCoord2f32(l_Vertices[i].m_TextureCoordinates.x, l_Vertices[i].m_TextureCoordinates.y);
    }

    GXSetProjection(*(Mtx44*)&m_ProjMatrix, GX_PERSPECTIVE);
}

void GCRenderer_Z::InitBlock(DrawInfo_Z& i_DrawInfo) {
    if (i_DrawInfo.m_VpId == 0 && (i_DrawInfo.m_Flag & DrawInfo_Z::FL_DRAWINFO_DO_POST_PROCESS)) {
        m_LightCacheState1.m_InUseNb = 0;
    }

    m_LightCacheState1.m_InUseNb = Max(m_LightCacheState1.m_InUseNb, m_ARamAllocator.m_AllocatedSize / 1024);
    m_LightCacheState1.m_MaxInUseNb = Max(m_LightCacheState1.m_InUseNb, m_LightCacheState1.m_MaxInUseNb);
    m_LightCacheState1.m_TotalNb = (U32)(m_ARamAllocator.m_EndAddress - m_ARamAllocator.m_StartAddress) >> 10;
    m_ARamAllocator.Init();
}

void GCRenderer_Z::SpecialWarpZone(DrawInfo_Z& i_DrawInfo) {
}

void GCRenderer_Z::RadialMBlur(DrawInfo_Z& i_DrawInfo) {
}
