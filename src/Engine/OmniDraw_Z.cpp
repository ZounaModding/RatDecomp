#include "Omni_Z.h"
#include "ObjectCollisionTools_Z.h"

void Omni_Z::Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) { }

// TODO: Finish matching
void Omni_Z::InFrustrum(DrawInfo_Z& i_DrawInfo, const Sphere_Z& i_BSph, Node_Z* i_Node, Omni_Z* i_Omni, OmniFrust_Z& i_OmniFrust, U8 i_Index) {
    i_OmniFrust.m_IndexInSA = i_Index;
    i_OmniFrust.m_Node = i_Node;
    i_OmniFrust.m_Omni = i_Omni;

    Mat3x3& l_RotInWorld = i_Node->GetRotInWorldMatrix();
    Vec4f l_DirectionWorldSpace(-l_RotInWorld.m.m[2][0], -l_RotInWorld.m.m[2][1], -l_RotInWorld.m.m[2][2], 0.0f);
    i_OmniFrust.m_DirectionWorldSpace = l_DirectionWorldSpace;
    i_OmniFrust.m_TranslationWorldSpace = *(Vec4f*)&i_Node->GetWorldMatrix().m[3][0];

    Float l_OmniStart = i_Omni->m_Start;
    Float l_UniformScale = i_Node->GetUniformScale();
    i_OmniFrust.m_UniformStartSquared = l_UniformScale * l_OmniStart;
    i_OmniFrust.m_UniformStartSquared *= i_OmniFrust.m_UniformStartSquared;
    i_OmniFrust.m_BSphereCamSpace = i_BSph;

    if (i_Omni->IsFlag(FL_IS_OMNI_SPOTLIGHT)) {
        O_SinCos(i_OmniFrust.m_SinCosOuterHalfRad, i_Omni->m_SpotOuterHalfAngleRad);
        i_OmniFrust.m_InvCosOuterHalfRad = 1.0f / i_OmniFrust.m_SinCosOuterHalfRad.x;
        i_OmniFrust.m_SinCosOuterHalfRad.x *= i_OmniFrust.m_SinCosOuterHalfRad.x;
        i_OmniFrust.m_SinCosOuterHalfRad.y *= i_OmniFrust.m_SinCosOuterHalfRad.y;
        if (i_DrawInfo.m_CameraNode) {
            i_DrawInfo.m_World2Cam.MulWithoutTrans(i_OmniFrust.m_DirectionWorldSpace.xyz(), i_OmniFrust.m_DirectionCamSpace);
            return;
        }
    }
    else {
        i_OmniFrust.m_InvCosOuterHalfRad = 0.0f;
    }

    i_OmniFrust.m_DirectionCamSpace = VEC3F_NULL;
}

// TODO: Finish matching
U32 Omni_Z::SetOmnis(const Box_Z& i_BBox, DrawInfo_Z& i_DrawInfo, Bool i_LocalValue, Bool i_OnlyTest, U32 i_Flag, U32 i_NoFlag) {
    S32 i;
    Node_Z* l_Node;
    S32 l_ActiveOmniNb;
    S32 l_OmniFrustNb;
    OmniStruct_Z* l_FirstOmniStruct;
    OmniStruct_Z* l_LowPrioOmniStruct;
    OmniStruct_Z* l_MidPrioOmniStruct;
    OmniStruct_Z* l_PrioOmniStruct;
    l_OmniFrustNb = i_DrawInfo.m_OmniFrustPtrSA.GetSize();
    i_DrawInfo.m_ActiveOmniCount = 0;
    i_DrawInfo.m_ActiveOmniFlags = 0;
    i_DrawInfo.m_OmniStructSA.SetSizeNoConstruct(l_OmniFrustNb);

    ASSERTLE_Z(i_LocalValue == TRUE, "", 116, "bLocalValue==(Bool) (0==0)");

    if (l_OmniFrustNb) {
        l_Node = i_DrawInfo.m_Node;

        Float l_InvScale = l_Node->GetInverseWorldScale();
        U32 l_ObjFlag = l_Node->GetObjectA()->GetFlag();
        Float l_InvScaleSqr = l_InvScale * l_InvScale;
        i_NoFlag |= l_ObjFlag & FL_OBJECT_OMNI_REJECT_ALL;
        Mat4x4& l_InvWorldMat = l_Node->GetInverseWorldMatrix();

        OmniFrust_Z** l_OmniFrustPtrs = i_DrawInfo.m_OmniFrustPtrSA.GetArrayPtr();
        OmniStruct_Z* l_OmniStruct = i_DrawInfo.m_OmniStructSA.GetArrayPtr();
        for (i = 0; i < l_OmniFrustNb; i++, l_OmniFrustPtrs++) {
            OmniFrust_Z* l_OmniFrust = *l_OmniFrustPtrs;
            Omni_Z* l_Omni = l_OmniFrust->m_Omni;
            U32 l_OmniFlag = l_Omni->GetFlag();

            if (!(l_OmniFlag & i_NoFlag) && (l_OmniFlag & i_Flag)) {
                Float l_Radius;
                Float l_StartSqr;
                i_DrawInfo.m_ActiveOmniFlags |= l_OmniFlag;
                l_OmniStruct->m_Flag = l_OmniFlag;
                l_OmniStruct->m_OmniFrust = l_OmniFrust;
                l_StartSqr = l_OmniFrust->m_UniformStartSquared * l_InvScaleSqr;
                l_Radius = l_OmniFrust->m_BSphereCamSpace.Radius;

                l_OmniStruct->m_StartSqr = l_StartSqr;
                l_OmniStruct->m_Attn = 1.0f / ((l_InvScaleSqr * (l_Radius * l_Radius)) - l_StartSqr);
                l_OmniStruct->m_SpotAtt0 = l_Omni->m_SpotAtt0;
                l_OmniStruct->m_SpotAtt1 = l_Omni->m_SpotAtt1;
                l_OmniStruct->m_TranslationLocalSpace = l_InvWorldMat * l_OmniFrust->m_TranslationWorldSpace;
                l_OmniStruct->m_TranslationLocalSpace.w = l_Radius * l_InvScale;
                if (l_OmniFlag & FL_IS_OMNI_SPOTLIGHT) {
                    Mat3x3& l_InvRotInWorldMat = l_Node->GetInverseMatRotInWorld();
                    -l_OmniStruct->GetDirectionWorldSpace() = l_InvRotInWorldMat * l_OmniFrust->m_DirectionWorldSpace.xyz();
                    l_OmniStruct->m_InvCosOuterHalfRad = l_OmniFrust->m_InvCosOuterHalfRad;
                    l_OmniStruct->m_SinOuterHalfRad = l_OmniFrust->m_SinCosOuterHalfRad.x;
                    l_OmniStruct->m_CosOuterHalfRad = l_OmniFrust->m_SinCosOuterHalfRad.y;
                }
                else {
                    l_OmniStruct->m_DirectionWorldSpace = l_OmniFrust->m_DirectionWorldSpace.xyz();
                    l_OmniStruct->m_InvCosOuterHalfRad = 0.0f;
                }
                l_OmniStruct++;
                i_DrawInfo.m_ActiveOmniCount++;
            }
        }

        if (!i_OnlyTest) {
            l_ActiveOmniNb = i_DrawInfo.m_ActiveOmniCount;
            l_FirstOmniStruct = i_DrawInfo.m_OmniStructSA.GetArrayPtr();
            l_PrioOmniStruct = l_FirstOmniStruct;
            if (l_ActiveOmniNb > MAX_OMNI) {
                l_LowPrioOmniStruct = l_FirstOmniStruct;
                i_DrawInfo.m_ActiveOmniCount = 0;
                for (i = 0; i < l_ActiveOmniNb; i++, l_PrioOmniStruct++) {
                    if (l_PrioOmniStruct->m_OmniFrust->m_Omni->IsFlag(FL_IS_OMNI_PRIORITY_HIGH) && SphereVsBox(*(Sphere_Z*)&l_PrioOmniStruct->m_TranslationLocalSpace, i_BBox)) {

                        i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_PrioOmniStruct;
                    }
                }
                l_MidPrioOmniStruct = l_FirstOmniStruct;
                for (i = 0; i < l_ActiveOmniNb; i++, l_MidPrioOmniStruct++) {
                    if (l_MidPrioOmniStruct->m_OmniFrust->m_Omni->IsFlag(FL_IS_OMNI_PRIORITY_MID) && SphereVsBox(*(Sphere_Z*)&l_MidPrioOmniStruct->m_TranslationLocalSpace, i_BBox)) {

                        i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_MidPrioOmniStruct;
                    }
                }
                for (i = 0; i < l_ActiveOmniNb; i++, l_LowPrioOmniStruct++) {
                    if (!l_LowPrioOmniStruct->m_OmniFrust->m_Omni->IsFlag(FL_IS_OMNI_PRIORITY_MID | FL_IS_OMNI_PRIORITY_HIGH) && SphereVsBox(*(Sphere_Z*)&l_LowPrioOmniStruct->m_TranslationLocalSpace, i_BBox)) {

                        i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_LowPrioOmniStruct;
                    }
                }
                if (i_DrawInfo.m_ActiveOmniCount > 3) {
                    i_DrawInfo.m_ActiveOmniCount = 3;
                }
            }
            else {
                for (i = 0; i < i_DrawInfo.m_ActiveOmniCount; i++, l_PrioOmniStruct++) {
                    i_DrawInfo.m_OmniStructs[i] = l_PrioOmniStruct;
                }
            }
        }
        else {
            i_DrawInfo.m_OmniStructSA.SetSizeNoConstruct(i_DrawInfo.m_ActiveOmniCount);
        }
    }

    return i_DrawInfo.m_ActiveOmniCount;
}

// TODO: Finish matching
U32 Omni_Z::SubOmnis(const Sphere_Z& i_BSph, const Box_Z& i_BBox, DrawInfo_Z& i_DrawInfo, U32 i_Flag, U32 i_NoFlag) {
    S32 i;
    S32 l_ActiveOmniNb;
    S32 l_OmniNb;
    U32 l_OmniFlag;
    OmniStruct_Z* l_OmniStruct;
    OmniStruct_Z* l_HighPrioOmniStruct;
    OmniStruct_Z* l_LowPrioOmniStruct;

    l_OmniNb = i_DrawInfo.m_OmniStructSA.GetSize();
    if (!l_OmniNb) {
        return 0;
    }

    l_OmniStruct = i_DrawInfo.m_OmniStructSA.GetArrayPtr();
    i_DrawInfo.m_ActiveOmniCount = 0;
    for (i = 0; i < l_OmniNb; i++, l_OmniStruct++) {
        l_OmniFlag = l_OmniStruct->m_Flag;
        if ((l_OmniFlag & i_Flag) && !(l_OmniFlag & i_NoFlag)) {
            Float l_Radius = (*(Sphere_Z*)&l_OmniStruct->m_TranslationLocalSpace).Radius + i_BSph.Radius;
            if (((*(Sphere_Z*)&l_OmniStruct->m_TranslationLocalSpace).Center - i_BSph.Center).GetNorm2() < l_Radius * l_Radius && SphereVsBox(*(Sphere_Z*)&l_OmniStruct->m_TranslationLocalSpace, i_BBox)) {
                i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_OmniStruct;
            }
        }
    }

    l_ActiveOmniNb = i_DrawInfo.m_ActiveOmniCount;
    if (l_ActiveOmniNb > MAX_OMNI) {
        l_OmniStruct = i_DrawInfo.m_OmniStructSA.GetArrayPtr();
        i_DrawInfo.m_ActiveOmniCount = 0;
        l_HighPrioOmniStruct = l_OmniStruct;
        l_LowPrioOmniStruct = l_OmniStruct;

        i_Flag |= FL_IS_OMNI_PRIORITY_HIGH;
        for (i = 0; i < l_ActiveOmniNb; i++, l_HighPrioOmniStruct++) {
            l_OmniFlag = l_HighPrioOmniStruct->m_Flag;
            if ((l_OmniFlag & i_Flag) && !(l_OmniFlag & i_NoFlag) && SphereVsBox(*(Sphere_Z*)&l_HighPrioOmniStruct->m_TranslationLocalSpace, i_BBox)) {
                i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_HighPrioOmniStruct;
            }
        }

        i_Flag &= ~FL_IS_OMNI_PRIORITY_HIGH;
        i_Flag |= FL_IS_OMNI_PRIORITY_MID;
        for (i = 0; i < l_ActiveOmniNb; i++, l_OmniStruct++) {
            l_OmniFlag = l_OmniStruct->m_Flag;
            if ((l_OmniFlag & i_Flag) && !(l_OmniFlag & i_NoFlag) && (l_OmniFlag & FL_IS_OMNI_PRIORITY_MID) && SphereVsBox(*(Sphere_Z*)&l_OmniStruct->m_TranslationLocalSpace, i_BBox)) {
                i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_OmniStruct;
            }
        }

        i_Flag &= ~FL_IS_OMNI_PRIORITY_MID;
        i_NoFlag |= FL_IS_OMNI_PRIORITY_MID;
        for (i = 0; i < l_ActiveOmniNb; i++, l_LowPrioOmniStruct++) {
            l_OmniFlag = l_OmniStruct->m_Flag;
            if ((l_OmniFlag & i_Flag) && !(l_OmniFlag & i_NoFlag) && SphereVsBox(*(Sphere_Z*)&l_LowPrioOmniStruct->m_TranslationLocalSpace, i_BBox)) {
                i_DrawInfo.m_OmniStructs[i_DrawInfo.m_ActiveOmniCount++] = l_LowPrioOmniStruct;
            }
        }

        if (i_DrawInfo.m_ActiveOmniCount > MAX_OMNI) {
            i_DrawInfo.m_ActiveOmniCount = MAX_OMNI;
        }
    }

    return i_DrawInfo.m_ActiveOmniCount;
}
