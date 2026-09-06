#include "CameraEngine_Z.h"
#include "CameraAgent_Z.h"
#include "Camera_Z.h"
#include "Game_Z.h"
#include "LodMove_Z.h"
#include "ParticlesAgent_Z.h"
#include "Renderer_Z.h"

Bool CameraEngine_Z::MarkHandles() {
    if (!Manipulator_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_LodMoveHdl) {
        m_LodMoveHdl->MarkHandles();
    }

    if (m_GameHdl) {
        m_GameHdl->MarkHandles();
    }

    return TRUE;
}

void CameraEngine_Z::ActionOnActivate() {
    gData.MainRdr->GetViewport(m_ViewportId).UpdateFrustrum();
}

const World_ZHdl& CameraEngine_Z::GetWorld() {
    return gData.MainRdr->GetViewport(m_ViewportId).GetWorld();
}

const Node_ZHdl& CameraEngine_Z::GetCameraNode() {
    return gData.MainRdr->GetViewport(m_ViewportId).GetCamera();
}

// TODO: Finish matching
void CameraEngine_Z::UpdateFX(Float i_DeltaTime) {
    if (i_DeltaTime) {
        Node_Z* l_CameraNode = GetCameraNode();
        Camera_Z* l_Camera = (Camera_Z*)l_CameraNode->GetObject();
        CameraAgent_Z* l_CameraAgent = (CameraAgent_Z*)GETPTR(l_CameraNode->GetAgent());
        ParticlesAgent_ZHdlDA& l_ParticleAgents = l_CameraAgent->GetParticlesAgents();
        S32 l_ParticleAgentNb = l_ParticleAgents.GetSize();

        Vec3f l_NearPos(0.0f, 0.0f, l_Camera->GetNearClip() + 3.0f);
        const Vec3f& l_LocalPos = l_NearPos / l_NearPos.GetNorm();
        const Vec3f& l_Plane = l_Camera->GetFrustrum().m_ViewPlanes.m_PlaneNormals[2].xyz();
        Float l_PlaneScale = 1.0f / (l_LocalPos * l_Plane);
        Vec3f l_Position = l_CameraNode->GetWorldMatrix() * (l_NearPos + l_Plane * l_PlaneScale);
        l_Position.y += 2.0f;

        Vec3f l_FirstAxis(0.0f, 0.0f, 1.0f);
        Quat l_FirstRotation;
        l_FirstRotation.SetAngleAxis(Pi, l_FirstAxis);

        Vec3f l_Direction = l_CameraNode->GetWorldTranslation() - l_Position;
        if (l_Direction.CNormalize()) {
            Vec3f l_SecondAxis(0.0f, -1.0f, 0.0f);
            Quat l_Rotation;
            l_Rotation = l_FirstRotation * Quat().SetAngleAxis(O_Atan2(l_Direction.x, l_Direction.z), l_SecondAxis);

            for (S32 i = 0; i < l_ParticleAgentNb; i++) {
                ParticlesAgent_ZHdl& l_AgentHdl = l_ParticleAgents[i];
                if (l_AgentHdl) {
                    l_AgentHdl->SetPosAndRot(l_Position, l_Rotation);
                }
            }
        }
    }
}
