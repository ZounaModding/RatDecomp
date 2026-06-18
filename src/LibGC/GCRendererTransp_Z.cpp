#include "GCRenderer_Z.h"

ExternC_Z void exit(int);

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

    m_DrawOrderGroupShouldDraw[do_unk_16] = (U32)i_DrawInfo.m_VpId < 6 && l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    m_DrawOrderGroupShouldDraw[do_shadow_cast] = (U32)i_DrawInfo.m_VpId < 6 && l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    m_DrawOrderGroupShouldDraw[do_unk_14] = FALSE;
    m_DrawOrderGroupShouldDraw[do_unk_13] = m_DrawOrderGroupShouldDraw[do_unk_14];

    m_DrawOrderGroupShouldDraw[do_last] = l_FrameBufferEffectFlag && !m_SkipFrameBufferEffects;

    S32 l_DrawCallCount = m_DrawCalls.GetSize();

    if (l_DrawCallCount != 0 && m_FrameBufferIdx >= 2) {
        SortElem_Z l_SortElems[4096];

        S32 NbDatas = Min(l_DrawCallCount, (S32)4096);

        SortElem_Z* l_CurElem = l_SortElems;
        ExtPrimitiveInfo_Z* l_DrawCalls = m_DrawCalls.GetArrayPtr();
        ExtPrimitiveInfo_Z* l_CurDrawCall = l_DrawCalls;

        U16 Cpt = 1;

        while (NbDatas--) {
            U8* l_Key = (U8*)&l_CurElem->m_Key;

            l_Key[3] = (U8)l_CurDrawCall->m_Order;
            l_Key[2] = (U8)(l_CurDrawCall->m_Order >> 8);
            l_Key[1] = l_CurDrawCall->m_UnusedSortKeyInsideGroup_0x78;
            l_Key[0] = l_CurDrawCall->m_DrawOrderGroup;

            l_CurElem->m_NextElemIdx = Cpt++;

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

void GCRenderer_Z::DrawOrder(DrawInfo_Z& i_DrawInfo, unsigned char i_Order) { }

void GCRenderer_Z::ImmediatQuad(
    const Vec2f& i_UVMin,
    const Vec2f& i_UVMax,
    const Vec2f& i_PosMin,
    const Vec2f& i_PosMax,
    const Vec2f& i_Size,
    const Color& i_Color,
    Float i_Z
) {
}

void GCRenderer_Z::InitBlock(DrawInfo_Z& i_DrawInfo) { }
