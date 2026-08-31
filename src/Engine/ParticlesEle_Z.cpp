#include "ParticlesEle_Z.h"
#include "ClassManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Program_Z.h"
#include "Sys_Z.h"

ParticlesEle_Z::ParticlesEle_Z() {
    m_Speed = 1.0f;
    m_ConeRadius = 0.0f;
    m_ConeHeight = 0.0f;
    m_ConeRadius2 = 0.0f;
    m_OffPlaneSpread = 0.0f;
    m_PeriodTime = 1.0f;
    m_LifeTime = 1.0f;
    m_SizeX = 1.0f;
    m_SizeY = 1.0f;
    m_Gravity = 1.0f;
    m_Flag = 1;
    m_MaxQuantity = PARTICLES_ELE_MAX_QUANTITY;
}

void ParticlesEle_Z::Load(void** i_Data) {
    LOADRANGE_Z(m_MaxQuantity, m_Flag);
    m_EmitLifeVelocity.Load(i_Data);
    m_EmitSRQG.Load(i_Data);
    m_PartSizeXRVA.Load(i_Data);
    m_PartSizeY.Load(i_Data);
    m_EmitColor.Load(i_Data);
    m_PartColor.Load(i_Data);
    m_PartGravity.Load(i_Data);
    gData.ClassMgr->LoadLink(m_MaterialAnimHdl, i_Data);
}

void ParticlesEle_Z::EndLoad() {
    gData.ClassMgr->UpdateLink(m_MaterialAnimHdl);
}

void ParticlesEle_Z::AfterEndLoad() {
    m_MaterialHdl = m_MaterialAnimHdl->GetMaterial();
}

void ParticlesEle_Z::MarkHandles() {
    m_MaterialAnimHdl->MarkHandles();
}

void ParticlesDataEle_Z::GetBSphere(Sphere_Z& io_Sphere) {
    ParticleSphere_Z* l_Sphere;
    l_Sphere = GetSpheres();
    if (l_Sphere->m_UnkId >= 0) {
        S32 l_Count = 8;
        for (; l_Count != 0 && l_Sphere->m_UnkId >= 0; l_Sphere++) {
            io_Sphere = io_Sphere + l_Sphere->m_Sphere;
            l_Count--;
        }
    }
}
