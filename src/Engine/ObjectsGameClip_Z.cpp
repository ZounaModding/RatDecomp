#include "ObjectsGame_Z.h"
#include "Agent_Z.h"
#include "Game_Z.h"
#include "ObjectMove_Z.h"
#include "ObjectMoveCollFlag_Z.h"
#include "Particles_Z.h"
#include "Renderer_Z.h"
#include "World_Z.h"

struct ObjectsGameClipViewport_Z {
    Frustrum_Z* m_Frustrum;
    Mat4x4* m_InverseWorldMatrix;
    Float m_InvDiagTangent;
};

void ObjectsGameClip_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_game_manager);
    SetNbVp(0);
    SetFirstVp(-1);
    SetGroup(ag_clipping);
    m_UnkBool_0x2c = FALSE;
}

void ObjectsGameClip_Z::Update(Float i_DeltaTime) {
    m_ObjectsGameHdl->UpdateBuffer();
    m_ObjectsGameHdl->SetRunningUpdateFromManager(TRUE);
    Game_Z* l_Game = GetGameHdl();
    UpdateObjectActivity(l_Game->GetWorld());
    m_ObjectsGameHdl->SetRunningUpdateFromManager(FALSE);
}

// TODO: Finish matching
void ObjectsGameClip_Z::UpdateObjectActivity(World_Z* i_World) {
    ObjectsGameClipViewport_Z l_Viewports[8];
    Sphere_Z l_Sphere;
    U32 l_ClipFlags;
    S32 i;

    for (i = 0; i < GetNbVp(); i++) {
        Viewport_Z& l_RenderViewport = gData.MainRdr->GetViewport(i + GetFirstVp());
        Camera_Z* l_Camera = (Camera_Z*)l_RenderViewport.GetCamera()->GetObject();
        l_Viewports[i].m_Frustrum = &l_Camera->GetFrustrum();
        l_Viewports[i].m_InverseWorldMatrix = &l_Camera->GetInverseWorldMatrix();
        l_Viewports[i].m_InvDiagTangent = l_RenderViewport.GetInvDTang();
    }

    S32 l_ObjectCount = m_ObjectsGameHdl->m_ObjectMoveGameDA.GetSize();
    i = l_ObjectCount - 1;
    ObjectMoveGame_Z* l_ObjectMoveGame = m_ObjectsGameHdl->m_ObjectMoveGameDA.GetArrayPtr() + i;
    for (; i >= 0; i--, l_ObjectMoveGame--) {
        ObjectMove_Z* l_ObjectMove = l_ObjectMoveGame->m_ObjectMove;
        Agent_Z* l_Agent = l_ObjectMoveGame->m_Agent;
        Node_Z* l_Node = l_ObjectMoveGame->m_Node;
        U32 l_AgentFlags = l_Agent->GetFlags();
        U32 l_Active = l_Node->HasFlag(FL_NODE_WAS_VISIBLE);
        l_Node->DisableFlag(FL_NODE_WAS_VISIBLE);

        if (!l_ObjectMove->IsFlagEnable(INT_BOL_PLAYER)) {
            if (!(l_AgentFlags & (FL_AGENT_NO_SUSPEND | FL_AGENT_NO_RESTORE))) {
                if (l_Active) {
                    Float l_MaxSqrDistance = l_Agent->GetMaxSqrDistActive();
                    if (l_MaxSqrDistance > 0.0f) {
                        Float l_Distance = l_MaxSqrDistance;
                        for (S32 j = 0; j < GetNbVp(); j++) {
                            l_Sphere = *l_Viewports[j].m_InverseWorldMatrix * l_Node->GetBSphere();
                            if (SphereVsFrustrum(*l_Viewports[j].m_Frustrum, l_Sphere, l_ClipFlags)) {
                                Float l_SqrDistance = l_Sphere.Center.GetNorm2();
                                if (l_SqrDistance < l_Distance) {
                                    l_Distance = l_SqrDistance;
                                }
                            }
                        }
                        if (l_Distance >= l_MaxSqrDistance) {
                            l_Active = FALSE;
                        }
                    }
                }
                else {
                    Float l_MinSqrDistance = l_Agent->GetMinSqrDistActive();
                    if (l_MinSqrDistance > 0.0f) {
                        Node_Z* l_AgentNode = l_Agent->GetNode();
                        for (S32 j = 0; j < GetNbVp(); j++) {
                            l_Sphere = *l_Viewports[j].m_InverseWorldMatrix * l_AgentNode->GetBSphere();
                            if (l_Sphere.Center.GetNorm2() < l_MinSqrDistance) {
                                l_Active = TRUE;
                                break;
                            }
                        }
                    }
                }
            }

            if (!(l_AgentFlags & FL_AGENT_NO_RESTORE) && !(l_AgentFlags & FL_AGENT_NO_SUSPEND)) {
                if (l_Active) {
                    if (l_Agent) {
                        l_Agent->Restore();
                    }
                    else {
                        l_ObjectMove->Activate();
                    }
                }
                else {
                    if (l_Agent) {
                        l_Agent->Suspend();
                    }
                    else {
                        l_ObjectMove->Deactivate();
                    }
                }
            }
        }
    }

    l_ObjectCount = m_ObjectsGameHdl->m_NodeDPA.GetSize();
    for (i = 0; i < l_ObjectCount; i++) {
        Node_Z* l_Node = m_ObjectsGameHdl->m_NodeDPA[i];
        Agent_Z* l_Agent = l_Node->GetAgent();
        if (!(l_Agent->GetFlags() & (FL_AGENT_NO_SUSPEND | FL_AGENT_NO_RESTORE))) {
            Float l_MaxSqrDistance = l_Agent->GetMaxSqrDistActive();
            U32 l_Hidden = l_Node->HasFlag(FL_NODE_HIDE);
            Float l_Fade = 0.0f;
            Float l_FadeDistance = l_Node->GetObject()->GetFadeDist();
            Float l_ParticlesFadeDistance = gData.MainRdr->GetParticlesFadeDist();
            Float l_Distance = l_MaxSqrDistance;

            if (!l_Hidden) {
                S32 l_NodeViewport = 0;
                for (; l_NodeViewport < GetNbVp(); l_NodeViewport++) {
                    l_Sphere = *l_Viewports[l_NodeViewport].m_InverseWorldMatrix * l_Node->GetBSphere();
                    if (SphereVsFrustrum(*l_Viewports[l_NodeViewport].m_Frustrum, l_Sphere, l_ClipFlags)) {
                        if (l_MaxSqrDistance > 0.0f) {
                            l_Distance = Min<Float>(l_Distance, l_Sphere.Center.GetNorm2());
                        }
                        if (l_FadeDistance > 0.0f) {
                            Float l_Scale = l_Viewports[l_NodeViewport].m_InvDiagTangent * l_Node->GetUniformScale();
                            Float l_CameraDistance = l_Sphere.Center.GetNorm();
                            Float l_ViewportFade = GetFadeValue_Z(l_CameraDistance, l_FadeDistance, l_Scale, l_ParticlesFadeDistance);
                            l_Fade = Max<Float>(l_ViewportFade, l_Fade);
                        }
                        else {
                            l_Fade = 1.0f;
                        }
                    }
                }
            }

            if (l_Fade == 0.0f || l_Distance >= l_MaxSqrDistance || l_Hidden) {
                l_Agent->Suspend();
            }
            else {
                l_Agent->Restore();
            }
        }
    }

    l_ObjectCount = m_ObjectsGameHdl->m_PlayParticlesGameDA.GetSize();
    S32 l_ParticlesIndex = l_ObjectCount - 1;
    S32 l_ParticlesViewport;
    PlayParticlesGame_Z* l_PlayParticlesGame = m_ObjectsGameHdl->m_PlayParticlesGameDA.GetArrayPtr();
    for (; l_ParticlesIndex >= 0; l_ParticlesIndex--, l_PlayParticlesGame++) {
        Node_Z* l_Node = l_PlayParticlesGame->m_Node;
        if (!l_Node->HasFlag(FL_NODE_HIDE)) {
            Agent_Z* l_Agent = l_PlayParticlesGame->m_Agent;
            if (!(l_Agent->GetFlags() & (FL_AGENT_NO_SUSPEND | FL_AGENT_NO_RESTORE))) {
                Float l_Fade = 0.0f;
                Float l_ParticlesFadeDistance = gData.MainRdr->GetParticlesFadeDist();
                ParticlesData_Z* l_ObjectDatas = (ParticlesData_Z*)l_Node->GetObjectDatas();
                Float l_FadeDistance = l_PlayParticlesGame->m_Particles->GetFadeDist();
                for (l_ParticlesViewport = 0; l_ParticlesViewport < GetNbVp(); l_ParticlesViewport++) {
                    const Sphere_Z& l_ParticlesSphere = l_ObjectDatas->GetBSphere();
                    l_Sphere = *l_Viewports[l_ParticlesViewport].m_InverseWorldMatrix * l_ParticlesSphere;
                    if (SphereVsFrustrum(*l_Viewports[l_ParticlesViewport].m_Frustrum, l_Sphere, l_ClipFlags)) {
                        if (l_FadeDistance > 0.0f) {
                            Float l_Scale = l_Viewports[l_ParticlesViewport].m_InvDiagTangent * l_Node->GetUniformScale();
                            Float l_CameraDistance = l_Sphere.Center.GetNorm();
                            Float l_ViewportFade = GetFadeValue_Z(l_CameraDistance, l_FadeDistance, l_Scale, l_ParticlesFadeDistance);
                            l_Fade = Max<Float>(l_ViewportFade, l_Fade);
                        }
                        else {
                            l_Fade = 1.0f;
                            break;
                        }
                    }
                }
                if (l_Fade != 0.0f) {
                    l_Agent->Restore();
                }
                else {
                    l_Agent->Suspend();
                }
            }
        }
    }
}
