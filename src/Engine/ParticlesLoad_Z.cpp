#include "Particles_Z.h"
#include "Sys_Z.h"

void Particles_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);

    S32 l_Nb;
    LOAD_Z(l_Nb);
    m_ParticleEmitters.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        m_ParticleEmitters[i].Load(i_Data);
    }

    LOADDYNARRAY_Z(m_LocalPlanes);
    LOAD_Z(m_UnkFloat_0x90);
}

void Particles_Z::EndLoad() {
    Object_Z::EndLoad();
    int l_Nb = m_ParticleEmitters.GetSize();
    for (S32 i = 0; i < l_Nb; i++) {
        m_ParticleEmitters[i].EndLoad();
    }
}

void Particles_Z::AfterEndLoad() {
    int l_Nb = m_ParticleEmitters.GetSize();
    for (S32 i = 0; i < l_Nb; i++) {
        m_ParticleEmitters[i].AfterEndLoad();
    }
}

void ParticlesData_Z::Load(void** i_Data) {
    ObjectDatas_Z::Load(i_Data);
    LOADBYTES_Z(&m_BSphere, sizeof(m_BSphere) + sizeof(m_FadeDist) + sizeof(m_UnkFloat_0x44));

    S32 l_Nb;
    LOAD_Z(l_Nb);
    m_ParticleEmittersData.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        LOAD_Z(m_ParticleEmittersData[i].m_MaxQuantity);
    }

    LOADDYNARRAY_Z(m_WorldSpacePlanes);
}
