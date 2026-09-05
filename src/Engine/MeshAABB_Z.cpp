#include "Mesh_Z.h"
#include "ClassManager_Z.h"
#include "CollisionTool_Z.h"
#include "Material_Z.h"
#include "Sys_Z.h"

#define MAX_AABB_RECURSE 32

inline Bool SegmentVsAABB(const Segment_Z& i_Segment, const Vec3f& i_Min, const Vec3f& i_Max) {
    for (S32 i = 0; i < 3; i++) {
        if (0.0f == i_Segment.Direction[i]) {
            if (i_Segment.Origin[i] < i_Min[i] || i_Segment.Origin[i] > i_Max[i]) {
                return FALSE;
            }
        }
        else {
            Float l_Inv = 1.0f / i_Segment.Direction[i];
            Float l_T0 = l_Inv * (i_Min[i] - i_Segment.Origin[i]);
            Float l_T1 = l_Inv * (i_Max[i] - i_Segment.Origin[i]);
            Float l_Far;
            Float l_Near;

            if (l_T0 > l_T1) {
                l_Near = l_T1;
                l_Far = l_T0;
            }
            else {
                l_Near = l_T0;
                l_Far = l_T1;
            }

            if (l_Near > l_Far) {
                return FALSE;
            }
            if (l_Far < 0.0f) {
                return FALSE;
            }
            if (l_Near > i_Segment.Length) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

Bool AABBCol_Z::Intersect(const Segment_Z& i_Segment, const Vec3f_S16_ZDA& i_Vertices, CollisionReport_Z& o_Report, const Material_ZHdlDA& i_Materials, U64 i_Flag, U64 i_NoFlag) {
    Bool l_Ret = FALSE;

    if (m_Nodes.GetSize() && i_Vertices.GetSize()) {
        AABBNode_Z* l_Stack[MAX_AABB_RECURSE];
        S32 l_StackNb = 1;
        U16 l_MaterialId = U16_MAX;
        U32 l_ColFlag = 0;

        l_Stack[0] = m_Nodes.GetArrayPtr();

        while (l_StackNb) {
            AABBNode_Z* l_Node = l_Stack[--l_StackNb];

            if (SegmentVsAABB(i_Segment, l_Node->m_Min, l_Node->m_Max)) {
                if (l_Node->m_FirstChild) {
                    l_Stack[l_StackNb++] = l_Node->m_FirstChild;
                    l_Stack[l_StackNb++] = l_Node->m_SecondChild;
                }
                else {
                    FaceCol_Z* l_Face = m_Faces.GetArrayPtr() + l_Node->m_FaceRange[0];
                    S32 l_NbFace = l_Node->m_FaceRange[1];

                    while (l_NbFace--) {
                        if (l_MaterialId != l_Face->m_MaterialId) {
                            l_MaterialId = l_Face->m_MaterialId;
                            l_ColFlag = i_Materials[l_MaterialId]->GetCollisionFlag();
                        }

                        if ((i_Flag & l_ColFlag) && !(i_NoFlag & l_ColFlag)) {
                            if (SegmentVsTri(i_Segment, i_Vertices[l_Face->m_Vertex1], i_Vertices[l_Face->m_Vertex2], i_Vertices[l_Face->m_Vertex3], o_Report)) {
                                o_Report.m_Flag = l_ColFlag;
                                l_Ret = TRUE;
                            }
                        }

                        l_Face++;
                    }
                }
            }
        }
    }

    return l_Ret;
}

void AABBCol_Z::Load(void** i_Data) {
    S32 l_Nb;

    LOADDYNARRAY_Z(m_Faces);
    LOADDYNARRAY_Z(m_Nodes);

    for (S32 i = 0; i < l_Nb; i++) {
        AABBNode_Z* l_Node = m_Nodes.GetArrayPtr() + i;

        if (l_Node->m_FirstChild) {
            l_Node->m_SecondChild = m_Nodes.GetArrayPtr() + (l_Node->m_ChildIds[0] - 1);
            l_Node->m_FirstChild = m_Nodes.GetArrayPtr() + (l_Node->m_ChildIds[1] - 1);
        }
    }
}
