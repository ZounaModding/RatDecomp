#include "Particles_Z.h"

Particles_Z::Particles_Z() {
}

Bool Particles_Z::MarkHandles() {
    return FALSE;
}

void Particles_Z::Clean() {
}

S32 Particles_Z::GetCollisions(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, const Vec3f& i_Dir, Float i_Len, ColLineResult_Z* o_Result, S32 i_MaxElem, U64 i_Flag, U64 i_NoFlag) {
    return 0;
}

void Particles_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
}

ParticlesData_Z::ParticlesData_Z() {
}

void ParticlesData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
}

void ParticlesData_Z::CloneEle(const ParticlesEle_ZDA& o_Elements, const Sphere_Z& i_BSphere) {
}
