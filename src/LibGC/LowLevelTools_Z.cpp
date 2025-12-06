#include "Handle_Z.h"
#include "Program_Z.h"
#include "Node_Z.h"
#include "BoneNode_Z.h"
#include "Assert_Z.h"
#include "Math_Z.h"

void Node_Z::UpdateTM(Node_Z* i_Parent) {
    Mat4x4 l_LocalMatrix;
    m_Rotation.GetMatrix(l_LocalMatrix);
    l_LocalMatrix.m[0][0] *= m_Scale;
    l_LocalMatrix.m[1][0] *= m_Scale;
    l_LocalMatrix.m[2][0] *= m_Scale;
    l_LocalMatrix.m[3][0] = m_Translation.x;
    l_LocalMatrix.m[0][1] *= m_Scale;
    l_LocalMatrix.m[1][1] *= m_Scale;
    l_LocalMatrix.m[2][1] *= m_Scale;
    l_LocalMatrix.m[3][1] = m_Translation.y;
    l_LocalMatrix.m[0][2] *= m_Scale;
    l_LocalMatrix.m[1][2] *= m_Scale;
    l_LocalMatrix.m[2][2] *= m_Scale;
    l_LocalMatrix.m[3][2] = m_Translation.z;
    GetWorldMatrix() = i_Parent->GetWorldMatrix() * l_LocalMatrix;

    m_RotInWorld = i_Parent->GetRotInWorld() * m_Rotation;
    m_RotInWorld.GetMatrix(m_RotInWorldMatrix);
    EnableFlag(FL_NODE_INVALIDROT);

    m_UniformScale = m_Scale * i_Parent->GetUniformScale();
    m_InverseUniformScale = 1.0f / m_UniformScale;
}

void Node_Z::UpdateRootTM() {
    Mat4x4 l_LocalMatrix;
    m_Rotation.GetMatrix(l_LocalMatrix);
    l_LocalMatrix.m[0][0] *= m_Scale;
    l_LocalMatrix.m[1][0] *= m_Scale;
    l_LocalMatrix.m[2][0] *= m_Scale;
    l_LocalMatrix.m[3][0] = m_Translation.x;
    l_LocalMatrix.m[0][1] *= m_Scale;
    l_LocalMatrix.m[1][1] *= m_Scale;
    l_LocalMatrix.m[2][1] *= m_Scale;
    l_LocalMatrix.m[3][1] = m_Translation.y;
    l_LocalMatrix.m[0][2] *= m_Scale;
    l_LocalMatrix.m[1][2] *= m_Scale;
    l_LocalMatrix.m[2][2] *= m_Scale;
    l_LocalMatrix.m[3][2] = m_Translation.z;
    GetWorldMatrix() = l_LocalMatrix;

    m_RotInWorld = m_Rotation;
    m_RotInWorld.GetMatrix(m_RotInWorldMatrix);
    EnableFlag(FL_NODE_INVALIDROT);

    m_UniformScale = m_Scale;
    m_InverseUniformScale = 1.0f / m_Scale;
}

void BoneNode_Z::UpdateTM(BoneNode_Z* i_Parent) {
    Mat4x4 l_LocalMatrix;
    GetRotation().GetMatrix(l_LocalMatrix);
    l_LocalMatrix.m[0][0] *= GetScale().x;
    l_LocalMatrix.m[1][0] *= GetScale().y;
    l_LocalMatrix.m[2][0] *= GetScale().z;
    l_LocalMatrix.m[3][0] = GetTranslation().x;
    l_LocalMatrix.m[0][1] *= GetScale().x;
    l_LocalMatrix.m[1][1] *= GetScale().y;
    l_LocalMatrix.m[2][1] *= GetScale().z;
    l_LocalMatrix.m[3][1] = GetTranslation().y;
    l_LocalMatrix.m[0][2] *= GetScale().x;
    l_LocalMatrix.m[1][2] *= GetScale().y;
    l_LocalMatrix.m[2][2] *= GetScale().z;
    l_LocalMatrix.m[3][2] = GetTranslation().z;
    GetWorldMatrix() = i_Parent->GetWorldMatrix() * l_LocalMatrix;

    m_RotInWorld = i_Parent->GetRotInWorld() * GetRotation();
    m_RotInWorld.GetMatrix(m_RotInWorldMatrix);
    EnableFlag(FL_BONENODE_INVALIDROT);
}

void Node_Z::GetLocal(const Segment_Z& i_WorldSegment, Segment_Z& o_LocalSegment) {
    o_LocalSegment.Origin = GetInverseWorldMatrix() * i_WorldSegment.Origin;
    Quat l_InverseRotInWorld = GetInverseRotInWorld();
    o_LocalSegment.Direction = l_InverseRotInWorld * i_WorldSegment.Direction;
    o_LocalSegment.Length = i_WorldSegment.Length * m_InverseUniformScale;
}

void Node_Z::GetLocal(const Sphere_Z& i_WorldSph, Sphere_Z& o_LocalSph) {
    o_LocalSph.Center = GetInverseWorldMatrix() * i_WorldSph.Center;
    o_LocalSph.Radius = i_WorldSph.Radius * m_InverseUniformScale;
}

void Node_Z::GetLocal(const Capsule_Z& i_WorldCapsule, Capsule_Z& o_LocalCapsule) {
    o_LocalCapsule.Origin = GetInverseWorldMatrix() * i_WorldCapsule.Origin;
    Quat l_InverseRotInWorld = GetInverseRotInWorld();
    o_LocalCapsule.Direction = l_InverseRotInWorld * i_WorldCapsule.Direction;
    o_LocalCapsule.Length = i_WorldCapsule.Length * m_InverseUniformScale;
    o_LocalCapsule.Radius = i_WorldCapsule.Radius * m_InverseUniformScale;
}

#ifdef NONMATCHING_Z
int g_Locks;
#endif

#pragma global_optimizer off

BaseObject_Z* HandleManager_Z::GetPtr(const BaseObject_ZHdl& i_Hdl) const {
#ifdef NONMATCHING_Z
    g_Locks++;
#endif
    S32 l_ID = i_Hdl.GetID();
    if (m_HandleRecDA[l_ID].m_Key == i_Hdl.GetKey()) {
        if (m_HandleRecDA[l_ID].m_xRamBlock >= 0) {
            return gData.ClassMgr->GetPtrXRam(m_HandleRecDA[l_ID]);
        }
        return m_HandleRecDA[l_ID].m_ObjPtr;
    }
    return NULL;
}

#pragma global_optimizer on
