#include "DCRenderer_Z.h"
#include "Material_Z.h"
#include "Program_Z.h"

DrawList_Z Draw2DListFromRenderFlags(U32 i_RenderFlags) {
    if (i_RenderFlags & (FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_ADDITIF | FL_MTL_RDR_SOUSTRACTIF | FL_MTL_RDR_DESTADDITIF)) {
        return dl_transparent;
    }
    if (i_RenderFlags & FL_MTL_RDR_IS_TRANSPARENT) {
        return dl_punchthrough;
    }
    return dl_opaque;
}

pvr_list_t PvrListFromDraw2DList(DrawList_Z i_List) {
    switch (i_List) {
        case dl_opaque:
            return PVR_LIST_OP_POLY;
        case dl_punchthrough:
            return PVR_LIST_PT_POLY;
        default:
            return PVR_LIST_TR_POLY;
    }
}

void Draw2D_Z::Begin() {
    for (S32 i = 0; i < dl_count; i++) {
        m_Buffers[i].m_Vtx2DBufferDA.Empty();
        m_Buffers[i].m_PrimitiveLists.Empty();
    }
}

void Draw2D_Z::Submit(DrawList_Z i_List) {
    Draw2DBuffer_Z& l_Buffer = m_Buffers[i_List];
    for (S32 i = 0; i < l_Buffer.m_PrimitiveLists.GetSize(); i++) {
        Primitive2DList_Z& l_List = l_Buffer.m_PrimitiveLists[i];
        pvr_prim(&l_List.m_Hdr, sizeof(l_List.m_Hdr));
        pvr_prim(&l_Buffer.m_Vtx2DBufferDA[l_List.m_VertexIdx], l_List.m_VertexCount * sizeof(pvr_vertex_t));
    }
}

void Draw2D_Z::LogStats() {
#ifdef DEBUG_Z
    static const Char* l_Names[dl_count] = { "OP", "PT", "TR" };

    for (S32 i = 0; i < dl_count; i++) {
        Draw2DBuffer_Z& l_Buffer = m_Buffers[i];
        S32 l_VertexCount = l_Buffer.m_Vtx2DBufferDA.GetSize();
        S32 l_Capacity = l_VertexCount + l_Buffer.m_Vtx2DBufferDA.GetReserved();

        Report_Z(
            "Draw2D %s: runs %d verts %d vtxbytes %d listbytes %d\n",
            l_Names[i],
            l_Buffer.m_PrimitiveLists.GetSize(),
            l_VertexCount,
            l_Capacity * (S32)sizeof(pvr_vertex_t),
            (l_Buffer.m_PrimitiveLists.GetSize() + l_Buffer.m_PrimitiveLists.GetReserved()) * (S32)sizeof(Primitive2DList_Z)
        );
    }
#endif
}

Primitive2DList_Z* Draw2D_Z::AddPrimitiveList(DrawList_Z i_List, Material_Z* i_Material, U16 i_DrawState, S32 i_RenderFlags) {
    Draw2DBuffer_Z& l_Buffer = m_Buffers[i_List];
    S32 l_Index = l_Buffer.m_PrimitiveLists.Add();
    Primitive2DList_Z& l_List = l_Buffer.m_PrimitiveLists[l_Index];
    pvr_poly_cxt_t l_Cxt;
    ((DCRenderer_Z*)gData.MainRdr)->BuildPolyContext(l_Cxt, i_Material, i_DrawState, i_RenderFlags, PvrListFromDraw2DList(i_List));
    pvr_poly_compile(&l_List.m_Hdr, &l_Cxt);
    l_List.m_Material = i_Material;
    l_List.m_DrawState = i_DrawState;
    l_List.m_RenderFlags = i_RenderFlags;
    l_List.m_VertexIdx = l_Buffer.m_Vtx2DBufferDA.GetSize();
    l_List.m_VertexCount = 0;
    return &l_List;
}

pvr_vertex_t* Draw2D_Z::Request(Material_Z* i_Material, Bool i_Transparent, S32 i_VertexCount) {
    if (i_VertexCount <= 0) {
        return NULL;
    }

    U16 l_DrawState = ds_zonly;
    U32 l_RenderFlags = 0;

    if (i_Material) {
        l_RenderFlags = i_Material->GetRenderFlag();
    }
    if (i_Transparent || (l_RenderFlags & FL_MTL_RDR_IS_ALPHABLENDED)) {
        l_RenderFlags |= FL_MTL_RDR_IS_ALPHABLENDED;
        l_DrawState |= ds_ablend;
        l_DrawState &= ~ds_zwrite;
    }
    if (!i_Material || i_Material->GetCode() != FL_MTL_CODE_ZONLY) {
        l_DrawState |= ds_cwrite;
    }
    if (l_RenderFlags & FL_MTL_RDR_TWO_SIDE) {
        l_DrawState &= ~ds_cw;
    }

    DrawList_Z l_ListType = Draw2DListFromRenderFlags(l_RenderFlags);
    Draw2DBuffer_Z& l_Buffer = m_Buffers[l_ListType];

    Primitive2DList_Z* l_List;
    if (!l_Buffer.m_PrimitiveLists.GetSize()) {
        l_List = AddPrimitiveList(l_ListType, i_Material, l_DrawState, l_RenderFlags);
    }
    else {
        l_List = &l_Buffer.m_PrimitiveLists[l_Buffer.m_PrimitiveLists.GetSize() - 1];
        if (l_List->m_Material != i_Material || l_List->m_DrawState != l_DrawState || l_List->m_RenderFlags != l_RenderFlags) {
            l_List = AddPrimitiveList(l_ListType, i_Material, l_DrawState, l_RenderFlags);
        }
    }

    U16 l_FirstIdx = l_Buffer.m_Vtx2DBufferDA.GetSize();
    if (i_VertexCount > l_Buffer.m_Vtx2DBufferDA.GetReserved()) {
        ExpandReserve(l_ListType);
    }
    l_Buffer.m_Vtx2DBufferDA.SetSize(l_FirstIdx + i_VertexCount, TRUE);
    l_List->m_VertexCount += i_VertexCount;

    pvr_vertex_t* l_Vertex = &l_Buffer.m_Vtx2DBufferDA[l_FirstIdx];

    while (i_VertexCount--) {
        l_Vertex->flags = i_VertexCount == 0 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
        l_Vertex++;
    }

    return &l_Buffer.m_Vtx2DBufferDA[l_FirstIdx];
}
