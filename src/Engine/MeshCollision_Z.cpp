#include "Mesh_Z.h"
#include "CollisionTool_Z.h"
#include "MaterialFlag_Z.h"
#include "MeshCollision_Z.h"
#include "Node_Z.h"
#include "ObjectCollisionTools_Z.h"

// TODO: Finish matching
Bool Mesh_Z::GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
    if (IsFlag(FL_OBJECT_SKINNED) || i_Data->IsFlag(FL_OBJECTDATAS_HIDE)) {
        return FALSE;
    }

    if (!(i_Flag & FL_COL_MESH_PRIM)) {
        return FALSE;
    }

    if (!SegmentVsSphere(i_Seg, i_Node->GetBSphere())) {
        return FALSE;
    }

    S32 i;
    S32 l_Nb;
    Segment_Z l_LocalSeg;
    CollisionReport_Z l_Report;

    i_Node->GetLocal(i_Seg, l_LocalSeg);

    Mat4x4* l_WorldMatrix = i_Node->GetWorldMatrixPtr();

    l_Report.m_CollisionDistance = 1.e10f;

    if ((i_Flag & FL_COL_MESH_CYLINDRE_PRIM) && m_CylindreCollisions.GetSize()) {
        l_Nb = m_CylindreCollisions.GetSize();
        CylindreCol_Z* l_Cylindre = m_CylindreCollisions.GetArrayPtr();

        for (i = 0; i < l_Nb; i++, l_Cylindre++) {
            if (!((U64)l_Cylindre->m_Flag & i_NoFlag) && ((U64)l_Cylindre->m_Flag & i_Flag) && SegmentVsCylindre(l_LocalSeg, l_Cylindre->m_Cylindre, l_Report)) {
                l_Report.m_EleIdx = i;
                l_Report.m_Flag = FL_COL_MESH_CYLINDRE_PRIM | l_Cylindre->m_Flag;
            }
        }
    }

    if ((i_Flag & FL_COL_MESH_BOX_PRIM) && m_BoxCollisions.GetSize()) {
        l_Nb = m_BoxCollisions.GetSize();
        BoxCol_Z* l_Box = m_BoxCollisions.GetArrayPtr();

        for (i = 0; i < l_Nb; i++, l_Box++) {
            if (!((U64)l_Box->m_Flag & i_NoFlag) && ((U64)l_Box->m_Flag & i_Flag) && SegmentVsBox(l_LocalSeg, l_Box->m_Box, l_Report)) {
                l_Report.m_EleIdx = i;
                l_Report.m_Flag = FL_COL_MESH_BOX_PRIM | l_Box->m_Flag;
            }
        }
    }

    if ((i_Flag & FL_COL_MESH_SPHERE_PRIM) && m_SphereCollisions.GetSize()) {
        l_Nb = m_SphereCollisions.GetSize();
        SphereCol_Z* l_Sphere = m_SphereCollisions.GetArrayPtr();

        for (i = 0; i < l_Nb; i++, l_Sphere++) {
            if (!((U64)l_Sphere->m_Flag & i_NoFlag) && ((U64)l_Sphere->m_Flag & i_Flag) && SegmentVsSphere(l_LocalSeg, l_Sphere->m_Sphere, l_Report)) {
                l_Report.m_EleIdx = i;
                l_Report.m_Flag = FL_COL_MESH_SPHERE_PRIM | l_Sphere->m_Flag;
            }
        }
    }

    m_AABBCol.Intersect(l_LocalSeg, m_OptimizedVertices, l_Report, m_Materials, i_Flag, i_NoFlag);

    if (l_Report.m_CollisionDistance < 1.e10f) {
        Float l_Dist = l_Report.m_CollisionDistance * i_Node->GetUniformScale();

        if (!o_Result.m_IsIntersect || l_Dist < o_Result.m_CollisionDistance) {
            o_Result.m_CollisionDistance = l_Dist;
            o_Result.m_Intersection = *l_WorldMatrix * l_Report.m_Intersection;

            Vec4_Rotate(o_Result.m_Normal, i_Node->GetRotInWorldMatrix(), l_Report.m_Normal);

            Float l_Norm2 = o_Result.m_Normal.GetNorm2();

            if (fabsf(1.0f - l_Norm2) > 1.e-4f) {
                o_Result.m_Normal *= 1.0f / Sqrt(l_Norm2);
            }

            o_Result.m_Flag = l_Report.m_Flag;
            o_Result.m_EleIdx = l_Report.m_EleIdx;
            o_Result.m_Node = i_Node;
            o_Result.m_Object = this;
            o_Result.m_ObjectDatas = i_Data;
            o_Result.m_IsIntersect = TRUE;
        }
    }

    return o_Result.m_IsIntersect;
}
