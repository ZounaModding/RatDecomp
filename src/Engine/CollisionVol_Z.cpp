#include "CollisionVol_Z.h"
#include "AnimFrame_Z.h"
#include "MaterialAnim_Z.h"

CollisionVol_Z::CollisionVol_Z() {
    SetGeometryType(COLLISIONVOL_Z);
}

Bool CollisionVol_Z::MarkHandles() {
    S32 i;

    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        if (m_AnimFrameHdls[i].IsValid()) {
            m_AnimFrameHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        if (m_MaterialAnimHdls[i].IsValid()) {
            m_MaterialAnimHdls[i]->MarkHandles();
        }
    }

    return TRUE;
}

Bool CollisionVol_Z::HasAgentClass(Name_Z& o_AgentClassName) {
    if (m_AgentClassName.GetID()) {
        o_AgentClassName = m_AgentClassName;
        return TRUE;
    }
    return FALSE;
}

void CollisionVol_Z::StartNodeAnims() {
}

void CollisionVol_Z::StartMatAnims() {
}

void CollisionVol_Z::ResetNodeAnims() {
}

void CollisionVol_Z::ResetMatAnims() {
}
