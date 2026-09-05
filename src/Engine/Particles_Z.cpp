#include "Particles_Z.h"
#include "Node_Z.h"

Particles_Z::Particles_Z() {
    m_Type = PARTICLES_Z;
    m_UnkFloat_0x90 = 0.0f;
    m_Flag |= FL_OBJECT_NO_DISPLAY;
    m_Flag &= ~(FL_OBJECT_UNK_0x10 | FL_OBJECT_DISPLAY);
}

Bool Particles_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    int l_Nb = m_ParticleEmitters.GetSize();

    for (S32 i = 0; i < l_Nb; i++) {
        m_ParticleEmitters[i].MarkHandles();
    }

    return TRUE;
}

void Particles_Z::Clean() {
}

U32 Particles_Z::HasEmittersFlag(U32 i_Flag) {
    for (S32 i = 0; i < m_ParticleEmitters.GetSize(); i++) {
        if (m_ParticleEmitters[i].HasFlag(i_Flag)) {
            return TRUE;
        }
    }

    return FALSE;
}

S32 Particles_Z::GetCollisions(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, const Vec3f& i_Dir, Float i_Len, ColLineResult_Z* o_Result, S32 i_MaxElem, U64 i_Flag, U64 i_NoFlag) {
    return 0;
}

void Particles_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
    ParticlesData_Z* l_Data = (ParticlesData_Z*)i_Data;
    l_Data->GetBSphere().Center = i_Node->GetWorldTranslation();
    l_Data->GetBSphere().Radius = 1.0f;

    S32 l_EmitterNb = l_Data->GetParticleEmittersData().GetSize();
    ParticlesDataEle_Z* l_EmitterData = l_Data->GetParticleEmittersData().GetArrayPtr();
    while (l_EmitterNb-- != 0) {
        l_EmitterData->GetBSphere(l_Data->GetBSphere());
        l_EmitterData++;
    }

    i_Node->SetBSphere(l_Data->GetBSphere());
    if (!l_Data->IsFlag(FL_PARTICLES_DISABLE_PLANE_UPDATE)) {
        UpdatePlane(i_Node->GetWorldMatrix(), m_LocalPlanes, l_Data->GetWorldSpacePlanes());
    }
}

// TODO: Finish matching
void Particles_Z::UpdatePlane(const Mat4x4& i_NodeWorldMatrix, const DynArray_Z<Mat4x4, 32, FALSE, FALSE>& i_LocalPlanes, Vec3fDA& o_WorldPlanes) {
    S32 l_PlaneNb = i_LocalPlanes.GetSize();
    if (l_PlaneNb != 0) {
        o_WorldPlanes.SetSize(l_PlaneNb + l_PlaneNb, FALSE);

        const Mat4x4* l_LocalPlane = i_LocalPlanes.GetArrayPtr();
        Vec3f* l_WorldPlane = o_WorldPlanes.GetArrayPtr();
        while (l_PlaneNb-- != 0) {
            Mat4x4 l_PlaneMatrix = i_NodeWorldMatrix * *l_LocalPlane;
            l_WorldPlane[0] = l_PlaneMatrix.GetMatrixTrans();
            l_PlaneMatrix.MulWithoutTrans(VEC3F_UP, l_WorldPlane[1]);
            l_WorldPlane[1].Normalize();

            l_LocalPlane++;
            l_WorldPlane += 2;
        }
    }
}

ParticlesData_Z::ParticlesData_Z() {
    m_Flag |= FL_OBJECTDATAS_DISPLAY;
    m_BSphere.Center = VEC3F_NULL;
    m_BSphere.Radius = 1.0f;
    m_FadeDist = 0.0f;
    m_UnkFloat_0x44 = 0.0f;
}

void ParticlesData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("ParticlesData_Z", 0));
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());
    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);

    Particles_Z* l_Particles = (Particles_Z*)(Object_Z*)o_ObjectHdl;
    ParticlesData_Z* l_Data = (ParticlesData_Z*)(ObjectDatas_Z*)o_ObjectDatasHdl;
    l_Data->CloneEle(l_Particles->GetParticleEmitters(), m_BSphere);
    l_Data->m_FadeDist = ((Object_Z*)o_ObjectHdl)->GetFadeDist();
    l_Data->m_UnkFloat_0x44 = m_UnkFloat_0x44;
}

// TODO: Finish matching
void ParticlesData_Z::CloneEle(const ParticlesEle_ZDA& o_Elements, const Sphere_Z& i_BSphere) {
    m_BSphere = i_BSphere;
    S32 i;
    S32 l_EmitterNb = o_Elements.GetSize();
    m_ParticleEmittersData.SetSize(l_EmitterNb, FALSE);

    for (i = 0; i < l_EmitterNb; i++) {
        ParticlesDataEle_Z& l_EmitterData = m_ParticleEmittersData[i];
        l_EmitterData.m_MaxQuantity = o_Elements[i].m_MaxQuantity;
        l_EmitterData.Reset();
    }
}

// TODO: Finish matching
void ParticlesData_Z::Reset() {
    S32 i = 0;
    S32 l_EmitterNb = m_ParticleEmittersData.GetSize();
    while (i < l_EmitterNb) {
        m_ParticleEmittersData[i].Reset();
        i++;
    }
}
