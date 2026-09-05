#include "PlayParticles_Z.h"
#include "Node_Z.h"
#include "Particles_Z.h"

PlayParticles_Z::~PlayParticles_Z() {
}

void PlayParticles_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_particles);
}

void PlayParticles_Z::Reset() {
    Resume();
    ((ParticlesData_Z*)GetNode()->GetObjectDatas())->Reset();
}

void PlayParticles_Z::Replay() {
}

void PlayParticles_Z::Resume() {
    m_IsPlaying = TRUE;
    GetNode()->UnHide(FALSE);
}

Bool PlayParticles_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_NodeHdl.IsValid()) {
        m_NodeHdl->MarkHandles();
    }

    if (m_EmiterNodeHdl.IsValid()) {
        m_EmiterNodeHdl->MarkHandles();
    }

    return TRUE;
}

void PlayParticles_Z::SetNodeEmiter(const Node_ZHdl& a1) {
}
