#include "Animation_Z.h"
#include "Sys_Z.h"

void Animation_Z::Load(void** i_Data) {
    LOAD_Z(m_MaxTime);
    LOAD_Z(m_SmoothRatio);
    LOAD_Z(m_Flag);

    AnimationNode_Z::Load(m_NodeKfr, i_Data);
    AnimationMaterial_Z::Load(m_MtlKfr, i_Data);
    AnimationMesh_Z::Load(m_MeshKfr, i_Data);
    AnimationMorph_Z::Load(m_MorphKfr, i_Data);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_NodeCct);
    LOADDYNARRAY_Z(m_MtlCct);
    LOADDYNARRAY_Z(m_MeshCct);
    LOADDYNARRAY_Z(m_MorphCct);
    if (GetMaxTime() <= 0.0f) {
        GetDebugName();
    }
}

void Animation_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    AnimationNode_Z::EndLoad(m_NodeKfr);
}

void Animation_Z::AfterEndLoad() {
    S32 i;
    S32 l_Nb = GetNbAnimationNode();
    for (i = 0; i < l_Nb; i++) {
        m_NodeCct[i].m_BoneID = (S16)gData.AnimMgr->GetNodeByName(m_NodeCct[i].m_BoneName);
    }
    l_Nb = GetNbAnimationMtl();
    for (i = 0; i < l_Nb; i++) {
        m_MtlCct[i].m_MtlID = (S16)gData.AnimMgr->GetMaterialByName(m_MtlCct[i].m_MtlName);
    }
    l_Nb = GetNbAnimationMesh();
    for (i = 0; i < l_Nb; i++) {
        m_MeshCct[i].m_ObjectID = (S16)gData.AnimMgr->GetMeshByName(m_MeshCct[i].m_ObjectName);
    }
    l_Nb = GetNbAnimationMorph();
    for (i = 0; i < l_Nb; i++) {
        m_MorphCct[i].m_ObjectID = (S16)gData.AnimMgr->GetMeshByName(m_MorphCct[i].m_ObjectName);
    }
}

void Animation_Z::EndLoadLinks() {
    m_NodeKfr.m_MsgKfr.EndLinks(m_ResObjLink);
}

void Animation_Z::Clean() {
    m_NodeCct.SetSize(0);
    m_MtlCct.SetSize(0);
    m_MeshCct.SetSize(0);
    m_MorphCct.SetSize(0);
    m_NodeKfr.Flush();
    m_MeshKfr.Flush();
    m_MtlKfr.Flush();
    m_MorphKfr.Flush();
}
