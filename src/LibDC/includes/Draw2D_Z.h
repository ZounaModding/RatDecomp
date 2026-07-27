#ifndef _DRAW2D_Z_H_
#define _DRAW2D_Z_H_
#include "BnkDynArray_Z.h"
#include "Math_Z.h"
#include "DCRendererEnum_Z.h"
#include <dc/pvr.h>

class Material_Z;

#define DRAW2D_VTXBUFFER_GRANULARITY 256
#define DRAW2D_VTXBUFFER_ALIGN 32
#define DRAW2D_VERTEX_LIST_GRANULARITY 16

class Draw2D_Z {
public:
    // Group of vertices that share the same material, draw state, and render flags
    class Primitive2DList_Z {
    public:
        Material_Z* m_Material;
        U16 m_DrawState;
        U32 m_RenderFlags;
        U16 m_VertexIdx;      // Index of the first vertex in this bucket's buffer
        U16 m_VertexCount;    // Number of vertices in this list
        pvr_poly_hdr_t m_Hdr; // Compiled once when the list is opened
    };

    class Draw2DBuffer_Z {
    public:
        DynArray_Z<pvr_vertex_t, DRAW2D_VTXBUFFER_GRANULARITY, FALSE, FALSE, DRAW2D_VTXBUFFER_ALIGN> m_Vtx2DBufferDA;
        DynArray_Z<Primitive2DList_Z, DRAW2D_VERTEX_LIST_GRANULARITY, FALSE, FALSE, alignof(Primitive2DList_Z)> m_PrimitiveLists;
    };

    void Begin();
    void Submit(DrawList_Z i_List);
    void LogStats();

    void EndOpaque() { Submit(dl_opaque); }

    void EndPunchThrough() { Submit(dl_punchthrough); }

    void EndTransparent() { Submit(dl_transparent); }

    pvr_vertex_t* Request(Material_Z* i_Material, Bool i_Transparent, S32 i_VertexCount);
    Primitive2DList_Z* AddPrimitiveList(DrawList_Z i_List, Material_Z* i_Material, U16 i_DrawState, S32 i_RenderFlags);

    void ExpandReserve(DrawList_Z i_List, U32 i_AddReserve = DRAW2D_VTXBUFFER_GRANULARITY) {
        DynArray_Z<pvr_vertex_t, DRAW2D_VTXBUFFER_GRANULARITY, FALSE, FALSE, DRAW2D_VTXBUFFER_ALIGN>& l_VertexDA = m_Buffers[i_List].m_Vtx2DBufferDA;
        l_VertexDA.SetReserve(l_VertexDA.GetSize() + i_AddReserve);
    }

    void Minimize() {
        for (S32 i = 0; i < dl_count; i++) {
            m_Buffers[i].m_Vtx2DBufferDA.Minimize();
            m_Buffers[i].m_PrimitiveLists.Minimize();
        }
    }

    Draw2DBuffer_Z m_Buffers[dl_count];
};

typedef Draw2D_Z::Primitive2DList_Z Primitive2DList_Z;

DrawList_Z Draw2DListFromRenderFlags(U32 i_RenderFlags);
pvr_list_t PvrListFromDraw2DList(DrawList_Z i_List);

#endif
