#include "Surface_Z.h"
#include "SurfaceCache_Z.h"
#include "Program_Z.h"

// TODO: Finish matching
Vec4f* Surface_Z::GetCollisionPtr(U16 i_PatchId) {
    Vec4f* l_Cache;

    if (m_PatchTab[i_PatchId].m_ColCacheId == U16_MAX) {
        U16 l_FreeId = gData.ColSurfaceCache->GetFreeEntry(this, i_PatchId);
        l_Cache = gData.ColSurfaceCache->GetCollisionCache(l_FreeId);
        FillColCache(l_Cache, m_PatchTab[i_PatchId]);
        m_PatchTab[i_PatchId].m_ColCacheId = l_FreeId;
    }
    else {
        l_Cache = gData.ColSurfaceCache->GetCollisionCache(m_PatchTab[i_PatchId].m_ColCacheId);
    }

    return l_Cache;
}

// TODO: Finish matching
void Surface_Z::FillColCache(Vec4f* o_Cache, const Patch& i_Patch) {
    QuadCtrlPoint_Z l_CtrlPoints;

    GetQuadPatchCtrlPoint(i_Patch, l_CtrlPoints);

    S32 l_Lod = gData.ColSurfaceCache->GetCollisionLod();
    Vec4f l_D0[4];
    Vec4f l_D1[4];
    Vec4f l_D2[4];
    Vec4f l_D3[4];
    Vec4f* l_Out = o_Cache;
    Float l_Step = 1.0f / (Float)l_Lod;
    Float l_Step2 = l_Step * l_Step;
    Float l_Step3 = l_Step2 * l_Step;
    S32 i;
    S32 j;

    for (i = 0; i < 4; i++) {
        Vec4f l_A;
        Vec4f l_B;
        Vec4f l_C;
        Vec4f l_D;

        Vec4_Scale(l_A, -1.0f, l_CtrlPoints.m_ControlPoints[0][i]);
        Vec4_Add_Scale(l_A, l_A, 3.0f, l_CtrlPoints.m_ControlPoints[1][i]);
        Vec4_Add_Scale(l_A, l_A, -3.0f, l_CtrlPoints.m_ControlPoints[2][i]);
        Vec4_Add_Scale(l_A, l_A, 1.0f, l_CtrlPoints.m_ControlPoints[3][i]);

        Vec4_Scale(l_B, 3.0f, l_CtrlPoints.m_ControlPoints[0][i]);
        Vec4_Add_Scale(l_B, l_B, -6.0f, l_CtrlPoints.m_ControlPoints[1][i]);
        Vec4_Add_Scale(l_B, l_B, 3.0f, l_CtrlPoints.m_ControlPoints[2][i]);

        Vec4_Scale(l_C, -3.0f, l_CtrlPoints.m_ControlPoints[0][i]);
        Vec4_Add_Scale(l_C, l_C, 3.0f, l_CtrlPoints.m_ControlPoints[1][i]);

        l_D0[i] = l_D = l_CtrlPoints.m_ControlPoints[0][i];

        Vec4_Scale(l_D1[i], l_Step3, l_A);
        Vec4_Add_Scale(l_D1[i], l_D1[i], l_Step2, l_B);
        Vec4_Add_Scale(l_D1[i], l_D1[i], l_Step, l_C);

        Vec4_Scale(l_D3[i], 6.0f * (l_Step3), l_A);
        Vec4_Add_Scale(l_D2[i], l_D3[i], l_Step2, l_B * 2.0f);
    }

    for (i = 0; i < l_Lod + 1; i++) {
        Vec4f l_A;
        Vec4f l_B;
        Vec4f l_C;
        Vec4f l_D;
        Vec4f l_Cur;
        Vec4f l_Delta1;
        Vec4f l_Delta2;
        Vec4f l_Delta3;

        Vec4_Scale(l_A, -1.0f, l_D0[0]);
        Vec4_Add_Scale(l_A, l_A, 3.0f, l_D0[1]);
        Vec4_Add_Scale(l_A, l_A, -3.0f, l_D0[2]);
        Vec4_Add_Scale(l_A, l_A, 1.0f, l_D0[3]);

        Vec4_Scale(l_B, 3.0f, l_D0[0]);
        Vec4_Add_Scale(l_B, l_B, -6.0f, l_D0[1]);
        Vec4_Add_Scale(l_B, l_B, 3.0f, l_D0[2]);

        Vec4_Scale(l_C, -3.0f, l_D0[0]);
        Vec4_Add_Scale(l_C, l_C, 3.0f, l_D0[1]);

        l_Cur = l_D = l_D0[0];

        Vec4_Scale(l_Delta1, l_Step3, l_A);
        Vec4_Add_Scale(l_Delta1, l_Delta1, l_Step2, l_B);
        Vec4_Add_Scale(l_Delta1, l_Delta1, l_Step, l_C);

        Vec4_Scale(l_Delta3, 6.0f * (l_Step3), l_A);
        Vec4_Add_Scale(l_Delta2, l_Delta3, 2.0f * l_Step2, l_B);

        for (j = 0; j < l_Lod + 1; j++) {
            *l_Out = l_Cur;
            l_Out++;

            Vec4_Add(l_Cur, l_Cur, l_Delta1);
            Vec4_Add(l_Delta1, l_Delta1, l_Delta2);
            Vec4_Add(l_Delta2, l_Delta2, l_Delta3);
        }

        for (j = 0; j < 4; j++) {
            Vec4_Add(l_D0[j], l_D0[j], l_D1[j]);
            Vec4_Add(l_D1[j], l_D1[j], l_D2[j]);
            Vec4_Add(l_D2[j], l_D2[j], l_D3[j]);
        }
    }

    Vec4f* l_Row1 = o_Cache + l_Lod + 1;
    Vec4f* l_P00 = o_Cache;
    Vec4f* l_P10 = l_Row1;
    Vec4f* l_P01 = o_Cache + 1;
    Vec4f* l_P11 = l_Row1 + 1;
    Vec4f l_Normal;
    Float l_Norm;

    for (i = 0; i < l_Lod; i++) {
        for (j = 0; j < l_Lod; j++) {
            Vec4f l_U;
            Vec4f l_V;

            Vec4_Scale(l_Out[0], 0.25f, *l_P00);
            Vec4_Add_Scale(l_Out[0], l_Out[0], 0.25f, *l_P10);
            Vec4_Add_Scale(l_Out[0], l_Out[0], 0.25f, *l_P01);
            Vec4_Add_Scale(l_Out[0], l_Out[0], 0.25f, *l_P11);

            Float l_Dist00 = Vec4_Dist2(*l_P00, l_Out[0]);
            Float l_Dist10 = Vec4_Dist2(*l_P10, l_Out[0]);
            Float l_Dist01 = Vec4_Dist2(*l_P01, l_Out[0]);
            Float l_Dist11 = Vec4_Dist2(*l_P11, l_Out[0]);

            l_Out[0].w = Max(l_Dist00, Max(l_Dist10, Max(l_Dist01, l_Dist11)));
            l_Out[0].w = Sqrt(l_Out[0].w);

            Vec4_Sub(l_U, *l_P10, *l_P00);
            Vec4_Sub(l_V, *l_P11, *l_P00);
            Vec4_Cross(l_Normal, l_U, l_V);
            l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

            if (l_Norm <= Float_Eps) {
                l_Out[1] = VEC4F_NULL;
            }
            else {
                Vec4_Scale(l_Out[1], 1.0f / l_Norm, l_Normal);
                l_Out[1].w = l_Norm;
            }

            Vec4_Sub(l_U, *l_P11, *l_P00);
            Vec4_Sub(l_V, *l_P01, *l_P00);
            Vec4_Cross(l_Normal, l_U, l_V);
            l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

            if (l_Norm <= Float_Eps) {
                l_Out[2] = VEC4F_NULL;
            }
            else {
                Vec4_Scale(l_Out[2], 1.0f / l_Norm, l_Normal);
                l_Out[2].w = l_Norm;
            }

            l_Out += 3;
            l_P00++;
            l_P10++;
            l_P01++;
            l_P11++;
        }

        l_P00++;
        l_P10++;
        l_P01++;
        l_P11++;
    }

    Vec4f* l_Left = o_Cache;
    Vec4f* l_Right = o_Cache + 1;

    for (i = 0; i < l_Lod; i++) {
        Vec4_Sub(l_Normal, *l_Right, *l_Left);
        l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

        if (l_Norm <= Float_Eps) {
            l_Out[0] = VEC4F_NULL;
        }
        else {
            Vec4_Scale(l_Out[0], 1.0f / l_Norm, l_Normal);
            l_Out[0].w = l_Norm;
        }

        l_Out++;
        l_Left++;
        l_Right++;
    }

    Vec4f* l_Up = o_Cache;
    Vec4f* l_UpRight = o_Cache + 1;
    Vec4f* l_Row1Right = l_Row1 + 1;

    for (i = 0; i < l_Lod; i++) {
        l_Normal = *l_Row1 - *l_Up;
        l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

        if (l_Norm <= Float_Eps) {
            l_Out[0] = VEC4F_NULL;
        }
        else {
            Vec4_Scale(l_Out[0], 1.0f / l_Norm, l_Normal);
            l_Out[0].w = l_Norm;
        }

        l_Out++;

        for (j = 0; j < l_Lod; j++) {
            l_Normal = *l_Row1Right - *l_UpRight;
            l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

            if (l_Norm <= Float_Eps) {
                l_Out[0] = VEC4F_NULL;
            }
            else {
                Vec4_Scale(l_Out[0], 1.0f / l_Norm, l_Normal);
                l_Out[0].w = l_Norm;
            }

            l_Normal = *l_Row1Right - *l_Row1;
            l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

            if (l_Norm <= Float_Eps) {
                l_Out[1] = VEC4F_NULL;
            }
            else {
                Vec4_Scale(l_Out[1], 1.0f / l_Norm, l_Normal);
                l_Out[1].w = l_Norm;
            }

            l_Normal = *l_Row1 - *l_UpRight;
            l_Norm = Sqrt(Vec4_Dot(l_Normal, l_Normal));

            if (l_Norm <= Float_Eps) {
                l_Out[2] = VEC4F_NULL;
            }
            else {
                Vec4_Scale(l_Out[2], 1.0f / l_Norm, l_Normal);
                l_Out[2].w = l_Norm;
            }

            l_Out += 3;
            l_Up++;
            l_Row1++;
            l_UpRight++;
            l_Row1Right++;
        }

        l_Up++;
        l_Row1++;
        l_UpRight++;
        l_Row1Right++;
    }
}
