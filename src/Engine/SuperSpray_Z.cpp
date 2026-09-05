#include "SuperSpray_Z.h"
#include "ClassManager_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "Particles_Z.h"
#include "ParticlesEle_Z.h"

void SuperSpray_Z::SetNode(const Node_ZHdl& i_NodeHdl) {
    m_Data = NULL;
    m_NodeHdl = i_NodeHdl;

    ASSERTLE_Z(
        i_NodeHdl->GetObject()->GetGeometryType() == PARTICLES_Z,
        "PlayParticles sur une particules, hein !!",
        16,
        "_NodeHdl->GetObject()->GetGeometryType()==PARTICLES_Z"
    );

    Check((ParticlesData_Z*)i_NodeHdl->GetObjectDatas());
}

void SuperSpray_Z::Check(ParticlesData_Z* i_Data) {
    if (m_Data != i_Data) {
        m_Data = i_Data;

        S32 l_Nb;
        Particles_Z* l_Particles = (Particles_Z*)m_NodeHdl->GetObject();

        l_Nb = l_Particles->GetNbEle();

        m_ParticleEmitters.SetSize(l_Nb);

        for (S32 i = 0; i < l_Nb; i++) {
            ParticlesDataEle_Z* l_DataEle = &i_Data->GetDataEle(i);
            ParticlesEle_Z* l_Ele = &l_Particles->GetEle(i);
            ParticlesEleState_Z* l_State = &m_ParticleEmitters[i];

            l_State->m_ParticlesEle = l_Ele;
            l_State->m_ParticlesDataEle = l_DataEle;
            l_State->m_PeriodTime = l_Ele->m_PeriodTime;
            l_State->m_StopEmit = FALSE;
            l_State->m_UsedNb = 0;
            l_State->m_Num = 0.0f;
            l_State->m_TimeElapsed = 0.0f;
            l_State->m_UseDefaultColor = FALSE;
        }
    }
}

void SuperSpray_Z::Reset() {
    PlayParticles_Z::Reset();

    S32 l_Nb = m_ParticleEmitters.GetSize();
    Bool l_AllHidden = TRUE;

    for (S32 i = 0; i < l_Nb; i++) {
        ParticlesEleState_Z* l_State = &m_ParticleEmitters[i];

        l_State->m_StopEmit = FALSE;
        l_State->m_UsedNb = 0;
        l_State->m_Num = 0.0f;
        l_State->m_TimeElapsed = 0.0f;
        l_State->m_UseDefaultColor = FALSE;

        if (!l_State->m_ParticlesEle->HasFlag(FL_PARTICLES_ELE_UNK_0x10)) {
            l_AllHidden = FALSE;
        }
    }

    if (l_AllHidden) {
        Pause();
        m_NodeHdl->Hide(FALSE);
    }
}

void SuperSpray_Z::Pause() {
    m_IsPlaying = FALSE;

    S32 l_Nb = m_ParticleEmitters.GetSize();
    S32 i = 0;
    while (l_Nb-- > 0) {
        m_ParticleEmitters[i++].m_StopEmit = TRUE;
    }
}

void SuperSpray_Z::Replay() {
}

void SuperSpray_Z::Resume() {
    PlayParticles_Z::Resume();

    S32 i;
    S32 l_Nb = m_ParticleEmitters.GetSize();

    for (i = 0; i < l_Nb; i++) {
        ParticlesEleState_Z* l_State = &m_ParticleEmitters[i];

        l_State->m_StopEmit = FALSE;

        if (0.0f == l_State->m_TimeElapsed) {
            if (l_State->m_ParticlesEle->HasFlag(FL_PARTICLES_ELE_UNK_0x8)) {
                Vec4f l_Value;

                l_State->m_ParticlesEle->m_EmitSRQG.GetValue(0.0f, l_Value, 1);
                l_State->m_Num = l_Value.z * l_State->m_ParticlesEle->m_Rate;
            }

            l_State->m_TimeElapsed = 0.0f;
        }
    }
}

void SuperSpray_Z::GetEmitRange(S32 a1) {
}

void SuperSpray_Z::GetEmitTime(S32 a1) {
}

void SuperSpray_Z::SetEmitColor(const Vec3f& a1, S32 a2) {
}

void SuperSpray_Z::Update(Float a1) {
}
