#include "ObjectsGame_Z.h"
#include "ABC_Message_Engine.h"
#include "AnimatedAgent_Z.h"
#include "AnimationManager_Z.h"
#include "Game_Z.h"
#include "Node_Z.h"
#include "PlayAnim_Z.h"
#include "World_Z.h"

#define MAX_SKEL_MSG 32

void ObjectsGame_Z::UpdateSkelCollision() {
    World_Z* l_World = GetGameHdl()->GetWorld();

    m_CachedSkelPtrs.SetSize(0, TRUE);
    m_CachedNodePtrs.SetSize(0, TRUE);
    m_CachedAnimatedAgentPtrs.SetSize(0, TRUE);

    S32 l_NbPlayAnim = m_PlayAnimsDPA.GetSize();

    for (S32 i = 0; i < l_NbPlayAnim; i++) {
        PlayAnim_Z* l_PlayAnim = m_PlayAnimsDPA[i];

        if (l_PlayAnim->IsActive()) {
            Node_Z* l_Node = l_PlayAnim->GetNode();
            AnimatedAgent_Z* l_Agent = (AnimatedAgent_Z*)(Agent_Z*)l_Node->GetAgent();
            Skel_Z* l_Skel;

            if (l_Agent && l_Agent->HasFlag(FL_AGENT_UNK_0x400) && (l_Skel = l_PlayAnim->GetSkel(), l_Skel->GetHitSpheres().GetSize())) {
                m_CachedSkelPtrs.Add(l_Skel);
                m_CachedNodePtrs.Add(l_Node);
                m_CachedAnimatedAgentPtrs.Add(l_Agent);

                for (S32 j = m_CachedAnimatedAgentPtrs.GetSize() - 1; --j >= 0;) {
                    Node_Z* l_VsNode = m_CachedNodePtrs[j];
                    Mat4x4& l_VsMatrix = l_VsNode->GetWorldMatrix();
                    Sphere_Z l_Sphere = l_Node->GetWorldMatrix() * l_Node->GetObject()->GetBSphere();
                    Sphere_Z l_VsSphere = l_VsMatrix * l_VsNode->GetObject()->GetBSphere();
                    Float l_Radius = l_Sphere.Radius + l_VsSphere.Radius;

                    if ((l_Sphere.Center - l_VsSphere.Center).GetNorm2() < l_Radius * l_Radius) {
                        Mat4x4 l_LocalMatrix = l_Node->GetInverseWorldMatrix() * l_VsMatrix;
                        Skel_Z* l_VsSkel = m_CachedSkelPtrs[j];
                        AnimatedAgent_Z* l_VsAgent = m_CachedAnimatedAgentPtrs[j];
                        SkelMessage_Z l_Msgs[MAX_SKEL_MSG];
                        S32 l_NbMsg = GetCollisionSkelMsg(l_Skel->GetHitSpheres(), l_VsSkel->GetHitSpheres(), l_LocalMatrix, l_Msgs);

                        if (l_NbMsg) {
                            SkelMessage_Z* l_Msg = l_Msgs;
                            S32 l_MsgId = gData.AnimMgr->GetSkelMessageNb();
                            SkelMessage_Z* l_Dest = gData.AnimMgr->GetSkelMessage() + l_MsgId;
                            SkelMessage_Z* l_LastMsg = NULL;
                            SkelMessage_Z* l_LastVsMsg = NULL;
                            S32 l_FirstMsgId = -1;
                            S32 l_FirstVsMsgId = -1;

                            for (S32 k = 0; k < l_NbMsg; k++, l_Msg++) {
                                if (l_Msg->m_CanSphereCollide || l_Msg->m_CanVsSphereCollide) {
                                    if (l_Msg->m_CanSphereCollide) {
                                        if (l_LastMsg) {
                                            l_LastMsg->m_NextId = l_MsgId;
                                        }
                                        else {
                                            l_FirstMsgId = l_MsgId;
                                        }

                                        l_Dest->m_NextId = 0;
                                        l_MsgId++;
                                        l_Dest->m_CanSphereCollide = l_Msg->m_CanVsSphereCollide;
                                        l_Dest->m_SphereId = l_Msg->m_VsSphereId;
                                        l_Dest->m_SphereName = l_Msg->m_VsSphereName;
                                        l_Dest->m_CanVsSphereCollide = TRUE;
                                        l_Dest->m_VsSphereId = l_Msg->m_SphereId;
                                        l_Dest->m_VsSphereName = l_Msg->m_SphereName;
                                        l_Dest->m_VsAgentPtr = l_Agent;
                                        l_Dest->m_SkelPtr = l_VsSkel;
                                        l_Dest->m_VsSkelPtr = l_Skel;
                                        l_Dest->m_BoneNodePtr = l_Msg->m_VsBoneNodePtr;
                                        l_Dest->m_VsBoneNodePtr = l_Msg->m_BoneNodePtr;
                                        l_Dest->m_VsName = l_Node->GetObject()->GetName();
                                        l_LastMsg = l_Dest;
                                        l_Dest++;
                                        gData.AnimMgr->GetSkelMessageNb()++;
                                    }

                                    if (l_Msg->m_CanVsSphereCollide) {
                                        if (l_LastVsMsg) {
                                            l_LastVsMsg->m_NextId = l_MsgId;
                                        }
                                        else {
                                            l_FirstVsMsgId = l_MsgId;
                                        }

                                        l_Dest->m_NextId = 0;
                                        l_MsgId++;
                                        l_Dest->m_CanSphereCollide = TRUE;
                                        l_Dest->m_SphereId = l_Msg->m_SphereId;
                                        l_Dest->m_SphereName = l_Msg->m_SphereName;
                                        l_Dest->m_CanVsSphereCollide = l_Msg->m_CanSphereCollide;
                                        l_Dest->m_VsSphereId = l_Msg->m_VsSphereId;
                                        l_Dest->m_VsSphereName = l_Msg->m_VsSphereName;
                                        l_Dest->m_VsAgentPtr = l_VsAgent;
                                        l_Dest->m_SkelPtr = l_Skel;
                                        l_Dest->m_VsSkelPtr = l_VsSkel;
                                        l_Dest->m_BoneNodePtr = l_Msg->m_BoneNodePtr;
                                        l_Dest->m_VsBoneNodePtr = l_Msg->m_VsBoneNodePtr;
                                        l_Dest->m_VsName = l_VsNode->GetObject()->GetName();
                                        l_LastVsMsg = l_Dest;
                                        l_Dest++;
                                        gData.AnimMgr->GetSkelMessageNb()++;
                                    }
                                }
                            }

                            abc_message l_Message = msg_anim_hit_skel_collide;
                            if (l_FirstMsgId >= 0) {
                                l_Agent->GetCollisionMessageAction(l_Message);
                                l_Agent->Send(l_Message, l_VsAgent, (Float)l_FirstMsgId);
                            }

                            l_Message = msg_anim_hit_skel_collide;
                            if (l_FirstVsMsgId >= 0) {
                                l_VsAgent->GetCollisionMessageAction(l_Message);
                                l_VsAgent->Send(l_Message, l_Agent, (Float)l_FirstVsMsgId);
                            }
                        }
                    }
                }
            }
        }
    }
}

S32 ObjectsGame_Z::GetCollisionSkelMsg(const SphereColBone_ZDA& i_Spheres, const SphereColBone_ZDA& i_VsSpheres, const Mat4x4& i_Matrix, SkelMessage_Z* o_Msgs) {
    return 0;
}
