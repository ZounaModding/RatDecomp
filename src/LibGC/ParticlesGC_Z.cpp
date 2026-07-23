#include "ParticlesGC_Z.h"
#include "GCMain_Z.h"

ParticlesGCData_Z::ParticlesGCData_Z() {
}

ParticlesGCData_Z::~ParticlesGCData_Z() {
}

void ParticlesGCData_Z::EndLoad() {
    ParticlesData_Z::EndLoad();

    S32 l_Size = m_ParticleEmittersData.GetSize();
    m_GDLHandles.SetSize(l_Size);
    for (S32 i = 0; i < l_Size; i++) {
        m_GDLHandles[i] = gData.GCParticlesManager->RegisterObject(1);
    }
}

void ParticlesGCData_Z::CloneEle(const ParticlesEle_ZDA& i_Elements, const Sphere_Z& i_BSphere) {
    ParticlesData_Z::CloneEle(i_Elements, i_BSphere);

    S32 l_Size = m_ParticleEmittersData.GetSize();
    m_GDLHandles.SetSize(l_Size);
    for (S32 i = 0; i < l_Size; i++) {
        m_GDLHandles[i] = gData.GCParticlesManager->RegisterObject(1);
    }
}
