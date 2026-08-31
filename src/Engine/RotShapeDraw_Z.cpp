#include "RotShape_Z.h"
#include "Renderer_Z.h"

void RotShape_Z::Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) {
    Float l_StartDistance = m_Scale;
    if (l_StartDistance || m_FadeOutDistance || i_Data->GetDfltColor().a < 1.0f) {
        Node_Z* l_Node = i_DrawInfo.m_Node;

        Vec4f l_Delta;
        l_Delta = l_Node->GetWorldTranslation() - i_DrawInfo.m_CameraTranslation.xyz();
        Float l_Distance = l_Delta.xyz().GetNorm();

        Float l_Alpha = i_Data->GetDfltColor().a;
        Float l_LodFadeDistance = i_DrawInfo.m_LodFadeDist;
        Float l_Scale = i_DrawInfo.m_VpInvDiagTangent * l_Node->GetUniformScale();
        Float l_Fade;

        if (m_FadeOutDistance) {
            Float l_FadeDistance = -(
                m_FadeOutDistance * (l_Scale * l_LodFadeDistance) - l_Distance
            );

            l_Fade = Clamp<Float>(1.0f - l_FadeDistance, 0.0f, 1.0f);
        }
        else {
            l_Fade = 1.0f;
        }

        if (l_Fade >= 1.0f) {
            if (l_StartDistance) {
                l_Fade = Clamp<Float>(-(l_Scale * l_StartDistance - l_Distance), 0.0f, 1.0f);
            }
            else {
                l_Fade = 1.0f;
            }
        }

        i_DrawInfo.m_FadeValue = l_Fade * l_Alpha;
    }
}
