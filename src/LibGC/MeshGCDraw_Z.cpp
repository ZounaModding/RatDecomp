#include "MeshGC_Z.h"
#include "GCRenderer_Z.h"
#include "NodeFlag_Z.h"
#include "Omni_Z.h"
#include "SystemDatas_Z.h"

void MeshGC_Z::Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) {
    if (i_DrawInfo.m_Node->GetObjectA() == this) {
        Mesh_Z::Draw(i_DrawInfo, i_Data);
        if (i_DrawInfo.m_FadeValue < 1.0f) {
            return;
        }
    }

    GETPTR(GetHandle());

    if (IsFlag(FL_OBJECT_SKINNED)) {
        GetDebugName();
    }
    else {
        GCRenderer_Z* l_Renderer = (GCRenderer_Z*)i_DrawInfo.m_Vp->GetRenderer();
        l_Renderer->SetLocal2Cam(i_DrawInfo.m_Local2Cam, 0);
        Omni_Z::SetOmnis(m_BBoxLocal, i_DrawInfo, TRUE, FALSE, U32_MINUS_ONE, 0);
        Bool l_IsFaded = l_Renderer->SetLights(i_DrawInfo, i_Data->GetDfltColor());
        l_Renderer->PushOrder(i_DrawInfo.m_ClipSph.Sph.Center.z + i_DrawInfo.m_ClipSph.Sph.Radius);

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxDesc(GX_VA_TEX1, GX_INDEX16);

        l_Renderer->SetRenderContext(FL_RDR_CONTEXT_LIT_TEX_SIMPLE_A);
        DrawWithMaterials(i_DrawInfo, m_StreamList, m_Materials, 0, l_IsFaded);
    }
}

Bool MeshGC_Z::DrawWithMaterials(DrawInfo_Z& i_DrawInfo, StreamList_Z& i_StreamList, Material_ZHdlDA& i_Materials, U32 i_Flag, Bool i_IsFaded) {
    GCRenderer_Z* l_Renderer = (GCRenderer_Z*)i_DrawInfo.m_Vp->GetRenderer();
    i_StreamList.SetStream();

    S32 l_MaterialCount = m_MaterialIndices.GetSize();
    S32 l_CurrentMaterialIndex = -1;
    U32 l_RenderFlag = 0;
    Bool l_Result = FALSE;

    for (S32 i = 0; i < l_MaterialCount; i++) {
        S32 l_MaterialIndex = m_MaterialIndices[i];
        if (l_MaterialIndex != l_CurrentMaterialIndex) {
            l_CurrentMaterialIndex = l_MaterialIndex;
            Material_Z* l_Material;
            if (gData.MainRdr->IsEffectFlag(FL_EFFECT_UNK_0x200) && i_DrawInfo.m_Node->IsFlagEnable(FL_NODE_SPECIAL_VISION)) {
                l_Material = (Material_Z*)GETPTR(gData.SystemDatas->GetNoTextureMaterial());
            }
            else {
                l_Material = (Material_Z*)GETPTR(i_Materials[l_MaterialIndex]);
            }

            l_RenderFlag = l_Material->GetRenderFlag();
            l_Renderer->SetMaterial(l_Material, GX_COLOR1A1);
        }

        if (l_RenderFlag & FL_MTL_RDR_INVISIBLE) {
            continue;
        }

        if ((l_RenderFlag & FL_MTL_CODE_ALL) == FL_MTL_CODE_ENVMAP_ALPHA) {
            l_Renderer->SetRenderBlendOp(FL_MTL_RDR_IS_TRANSPARENT);
            l_Renderer->PushDo(do_opaque);
            if (l_RenderFlag & FL_MTL_RDR_TWO_SIDE) {
                l_Renderer->PushDs(ds_ztest | ds_zwrite | ds_cwrite);
            }
            else {
                l_Renderer->PushDs(ds_opaque);
            }
            l_Renderer->PushADraw(&i_StreamList, &m_DisplayLists[i], FL_RDR_CONTEXT_LIT_TEX_KALPHA_B);
        }
        else if (i_IsFaded || (l_RenderFlag & FL_MTL_RDR_IS_ALPHABLENDED)) {
            if (l_RenderFlag & FL_MTL_RDR_TRANSP_ATFIRST) {
                l_Renderer->PushDo(do_transp_first);
            }
            else {
                l_Renderer->PushDo(do_transp);
            }

            S32 l_DrawCallIndex = -1;
            if (i_IsFaded && !(l_RenderFlag & (FL_MTL_RDR_ADDITIF | FL_MTL_RDR_SOUSTRACTIF))) {
                l_Renderer->PushDs(ds_zonly);
                l_DrawCallIndex = l_Renderer->PushADraw(
                    &i_StreamList, &m_DisplayLists[i], -1
                );
            }

            l_Renderer->SetRenderBlendOp(
                l_Renderer->GetCurrentBlendFlags() | FL_MTL_RDR_IS_ALPHABLENDED
            );
            l_Renderer->PushDs(ds_ztest | ds_cwritergb | ds_ablend);
            l_Renderer->PushLinkedDraw(
                l_DrawCallIndex, &i_StreamList, &m_DisplayLists[i]
            );
            l_Result = TRUE;
        }
        else if (i_Flag & FL_OBJECT_HAS_SHADOW_VOLUME) {
            l_Renderer->PushDo(do_scene_draw);
            l_Renderer->PushDs((l_RenderFlag & FL_MTL_RDR_TWO_SIDE) ? (ds_ztest | ds_zwrite | ds_cwrite) : ds_opaque);
            l_Renderer->PushADraw(&i_StreamList, &m_DisplayLists[i], -1);
            l_Result = TRUE;
        }
        else {
            m_DisplayLists[i].Call();
        }
    }

    return l_Result;
}

void MeshGC_Z::DrawCastingShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) {
}
