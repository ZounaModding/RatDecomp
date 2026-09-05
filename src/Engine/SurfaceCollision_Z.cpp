#include "Surface_Z.h"
#include "Collision_Z.h"
#include "CollisionTool_Z.h"
#include "Material_Z.h"
#include "Node_Z.h"
#include "ObjectCollisionTools_Z.h"
#include "SeadIndex_Z.h"
#include "SurfaceCache_Z.h"

// TODO: Finish matching
Bool Surface_Z::GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
    if ((i_NoFlag & (FL_COL_SURFACE | FL_COL_SPLINE)) == (FL_COL_SURFACE | FL_COL_SPLINE)) {
        return FALSE;
    }

    CollisionReport_Z l_Report;
    Segment_Z l_LocalSeg;

    l_Report.m_CollisionDistance = 1.e10f;

    S32 l_Lod = gData.ColSurfaceCache->GetCollisionLod();

    if (!(i_NoFlag & FL_COL_SURFACE) && SegmentVsSphere(i_Seg, i_Node->GetBSphere())) {
        i_Node->GetLocal(i_Seg, l_LocalSeg);

        S32 l_NbPatch = m_PatchTab.GetSize();

        if (m_Sead) {
            if (m_Sead->RayCast(l_LocalSeg)) {
                S32 l_NbHit = m_Sead->GetHitIndex();

                if (l_NbHit) {
                    Patch* l_Patch = m_PatchTab.GetArrayPtr();
                    S16* l_HitList = m_Sead->GetHitElementList();

                    while (l_NbHit--) {
                        S16 l_PatchId = *l_HitList++;
                        Patch* l_HitPatch = &l_Patch[l_PatchId];

                        if (SegmentVsSphere(l_LocalSeg, l_HitPatch->m_BSphere) && SegmentVsBox(l_LocalSeg, l_HitPatch->m_BBox)) {
                            U64 l_ColFlag = l_HitPatch->m_Material->GetCollisionFlag();

                            if ((i_Flag & l_ColFlag) && !(i_NoFlag & l_ColFlag) && LineVsPatch(l_LocalSeg, GetCollisionPtr(l_PatchId), l_Report, l_Lod)) {
                                l_Report.m_Flag = l_ColFlag;
                                l_Report.m_EleIdx = l_PatchId;

                                if (i_Flag & FL_COL_ONE_HIT) {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            Patch* l_Patch = m_PatchTab.GetArrayPtr();

            for (S16 i = 0; i < l_NbPatch; i++, l_Patch++) {
                if (SegmentVsSphere(l_LocalSeg, l_Patch->m_BSphere) && SegmentVsBox(l_LocalSeg, l_Patch->m_BBox)) {
                    U64 l_ColFlag = l_Patch->m_Material->GetCollisionFlag();

                    if ((i_Flag & l_ColFlag) && !(i_NoFlag & l_ColFlag) && LineVsPatch(l_LocalSeg, GetCollisionPtr(i), l_Report, l_Lod)) {
                        l_Report.m_Flag = l_ColFlag;
                        l_Report.m_EleIdx = i;

                        if (i_Flag & FL_COL_ONE_HIT) {
                            break;
                        }
                    }
                }
            }
        }
    }
    else {
        l_LocalSeg.Length = 0.0f;
    }

    if ((i_Flag & FL_COL_SPLINE) && m_EdgeColTab.GetSize() && (i_Seg.Direction.x || i_Seg.Direction.z)) {
        if (!l_LocalSeg.Length) {
            i_Node->GetLocal(i_Seg, l_LocalSeg);
        }

        Float l_Step = 1.0f / (Float)(l_Lod + 1);

        StaticArray_Z<TreeStackEle, MAX_RECURSE> l_Stack;
        Sphere_Z l_Sphere;
        S16 l_Ret;

        l_Stack.SetSize(0);
        l_Stack.Add(TreeStackEle(0));

        for (;;) {
            RECURSE_TREE(l_Stack, l_Sphere, CylinderSphereVsSegment(l_Sphere, l_LocalSeg), l_Ret, m_STreeTab, m_EdgeColTab);

            if (l_Ret == -1) {
                break;
            }

            const Vec3f* l_Vertices = m_Vertices.GetArrayPtr();
            const Edge& l_Edge = m_EdgeTab[m_EdgeColTab[l_Ret].m_Index];

            if (LineVsSplineCollide(l_LocalSeg, l_Vertices[l_Edge.P[0]], l_Vertices[l_Edge.T[0]], l_Vertices[l_Edge.T[1]], l_Vertices[l_Edge.P[1]], l_Report, l_Step)) {
                l_Report.m_Flag = FL_COL_SPLINE;
                l_Report.m_EleIdx = l_Ret;

                if (i_Flag & FL_COL_ONE_HIT) {
                    break;
                }
            }
        }
    }

    if (l_Report.m_CollisionDistance < 1.e10f) {
        Vec4f l_World = i_Node->GetWorldMatrix() * l_Report.m_Intersection;

        l_Report.m_Intersection = l_World;
        l_Report.m_CollisionDistance = i_Seg.Direction * Vec4f(l_World.x - i_Seg.Origin.x, l_World.y - i_Seg.Origin.y, l_World.z - i_Seg.Origin.z, 1.0f).xyz();

        if (!o_Result.m_IsIntersect || l_Report.m_CollisionDistance < o_Result.m_CollisionDistance) {
            o_Result.m_CollisionDistance = l_Report.m_CollisionDistance;
            o_Result.m_Intersection = l_Report.m_Intersection;

            Vec4_Rotate(o_Result.m_Normal, i_Node->GetRotInWorldMatrix(), l_Report.m_Normal);

            Float l_Norm2 = o_Result.m_Normal.GetNorm2();

            if (fabsf(1.0f - l_Norm2) > 1.e-4f) {
                o_Result.m_Normal *= 1.0f / Sqrt(l_Norm2);
            }

            o_Result.m_Flag = l_Report.m_Flag;
            o_Result.m_EleUV = VEC2F_NULL;
            o_Result.m_EleIdx = l_Report.m_EleIdx;
            o_Result.m_Node = i_Node;
            o_Result.m_Object = this;
            o_Result.m_ObjectDatas = i_Data;
            o_Result.m_IsIntersect = TRUE;

            return TRUE;
        }
    }

    return FALSE;
}

Bool Surface_Z::GetClingLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ClingLineResult_Z& o_Result) {
    return FALSE;
}
