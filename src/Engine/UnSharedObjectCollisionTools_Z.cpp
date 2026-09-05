#include "CollisionTool_Z.h"

// TODO: Finish matching
Bool LineVsPatch(const Segment_Z& i_Segment, Vec4f* i_Cache, CollisionReport_Z& o_Report, S32 i_Lod) {
    S32 l_Size = i_Lod + 1;
    Bool l_Ret = FALSE;
    Vec4f* l_Vtx0 = i_Cache;
    Vec4f* l_Vtx1 = i_Cache + i_Lod + 1;
    Vec4f* l_Vtx2 = i_Cache + 1;
    Vec4f* l_Vtx3 = l_Vtx1 + 1;
    Vec4f* l_Normal = i_Cache + l_Size * l_Size;
    Vec4f* l_VtxB;
    Vec4f* l_VtxC;
    S32 i;
    S32 j;
    S32 k;

    for (i = 0; i < i_Lod; i++) {
        for (j = 0; j < i_Lod; j++) {
            Vec3f l_ToCell = l_Normal->xyz() - i_Segment.Origin;
            Vec3f l_Center = i_Segment.Direction * (i_Segment.Direction * l_ToCell) + i_Segment.Origin;
            Vec3f l_Delta = i_Segment.Origin - l_Vtx0->xyz();

            l_Normal++;

            for (k = 0; k < 2; k++, l_Normal++) {
                if (l_Normal->w) {
                    Float l_Dist = l_Normal->xyz() * l_Delta;

                    if (l_Dist > 0.0f) {
                        Float l_T = -(l_Dist / (l_Normal->xyz() * i_Segment.Direction));

                        if (l_T >= 0.0f && l_T <= i_Segment.Length && l_T < o_Report.m_CollisionDistance) {
                            if (k) {
                                l_VtxB = l_Vtx3;
                                l_VtxC = l_Vtx2;
                            }
                            else {
                                l_VtxB = l_Vtx1;
                                l_VtxC = l_Vtx3;
                            }

                            Vec3f l_Inter = i_Segment.Direction * l_T + i_Segment.Origin;
                            Vec4f l_E0 = l_Vtx0->xyz() - l_Inter;
                            Vec4f l_E1 = l_VtxB->xyz() - l_Inter;

                            if ((l_E0 ^ l_E1) * *l_Normal >= 0.0f) {
                                Vec4f l_E2 = l_VtxC->xyz() - l_Inter;

                                if ((l_E1 ^ l_E2) * *l_Normal >= 0.0f) {
                                    if ((l_E2 ^ l_E0) * *l_Normal >= 0.0f) {
                                        o_Report.m_Normal = *l_Normal;
                                        o_Report.m_Intersection = l_Inter;
                                        o_Report.m_CollisionDistance = l_T;
                                        l_Ret = TRUE;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            l_Vtx0++;
            l_Vtx1++;
            l_Vtx2++;
            l_Vtx3++;
        }

        l_Vtx0++;
        l_Vtx1++;
        l_Vtx2++;
        l_Vtx3++;
    }

    return l_Ret;
}

Bool LineVsSplineCollide(const Segment_Z& i_Segment, const Vec3f& i_P0, const Vec3f& i_P1, const Vec3f& i_P2, const Vec3f& i_P3, CollisionReport_Z& o_Report, Float i_Step) {
    return FALSE;
}
