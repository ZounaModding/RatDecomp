#include "ParticlesAgent_Z.h"
#include "ClassManager_Z.h"
#include "Game_Z.h"
#include "Node_Z.h"
#include "Particles_Z.h"
#include "PlayParticles_Z.h"
#include "Skel_Z.h"

BEGIN_INIT_AGENT_CLASS(ParticlesAgent_Z, Agent_Z)
END_INIT_AGENT_CLASS

ParticlesAgent_Z::ParticlesAgent_Z() {
    EnableFlag(FL_AGENT_PARTICLES);
}

void ParticlesAgent_Z::Init() {
    Agent_Z::Init();

    if (HasFlag(FL_AGENT_PARTICLES)) {
        SetPlayParticles(gData.ClassMgr->NewObject("PlayParticles_Z"));
    }
}

void ParticlesAgent_Z::Reset() {
    Agent_Z::Reset();
    m_PlayParticlesHdl->Reset();
    m_PlayParticlesHdl->Deactivate();
}

void ParticlesAgent_Z::Pause() {
    m_PlayParticlesHdl->Pause();
}

void ParticlesAgent_Z::Suspend() {
    if (!(GetFlags() & (FL_AGENT_NO_SUSPEND | FL_AGENT_HAS_EMITTER_FLAG_0x100))) {
        m_PlayParticlesHdl->Deactivate();
    }
}

void ParticlesAgent_Z::Restore() {
}

Bool ParticlesAgent_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_PlayParticlesHdl.IsValid()) {
        m_PlayParticlesHdl->MarkHandles();
    }

    return TRUE;
}

void ParticlesAgent_Z::SetPlayParticles(const BaseObject_ZHdl& i_Hdl) {
    m_PlayParticlesHdl = i_Hdl;
}

void ParticlesAgent_Z::UseNode(Node_ZHdl& i_NodeHdl) {
    Agent_Z::UseNode(i_NodeHdl);
    m_PlayParticlesHdl->SetNode(i_NodeHdl);

    if (((Particles_Z*)i_NodeHdl->GetObjectA())->HasEmittersFlag(FL_PARTICLES_EMITTER_UNK_0x100)) {
        EnableFlag(FL_AGENT_HAS_EMITTER_FLAG_0x100);
    }

    Reset();
}

void ParticlesAgent_Z::RegisterWithGame(Game_ZHdl& i_GameHdl) {
    Agent_Z::RegisterWithGame(i_GameHdl);
    i_GameHdl->DeclareParticlesManipulator(m_PlayParticlesHdl);
}

void ParticlesAgent_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}

void ParticlesAgent_Z::Follow(Node_Z* i_Node, Skel_Z* i_Skel, S32 i_BoneId) {
    if (!i_Node) {
        m_FollowNodeHdl = HANDLE_NULL;
        return;
    }

    m_FollowNodeHdl = i_Node->GetHandle();
    m_FollowSkelHdl = HANDLE_NULL;

    if (i_Skel && i_BoneId >= 0 && i_BoneId < i_Skel->GetNbBone() && i_Skel->GetBone(i_BoneId)) {
        m_FollowSkelHdl = i_Skel->GetHandle();
        m_FollowBoneId = i_BoneId;
    }

    DoFollow();
}

void ParticlesAgent_Z::DoFollow() {
}

void ParticlesAgent_Z::SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot) {
}
