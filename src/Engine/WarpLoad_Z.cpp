#include "Warp_Z.h"
#include "ClassManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Node_Z.h"
#include "Program_Z.h"
#include "Sys_Z.h"

void Warp_Z::Load(void** i_Data) {
    S32 l_Nb;
    S32 i;

    LOADRANGE_Z(m_Flag, m_UnkVec3f_0xb4.z);

    for (i = 0; i < MAX_WARP_FACE; i++) {
        gData.ClassMgr->LoadLink(m_MaterialAnimHdls[i], i_Data);
    }

    gData.ClassMgr->LoadLink(m_RootNodeHdl, i_Data);

    LOADDYNARRAYSIZE_Z(m_AnimFrameHdls);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_AnimFrameHdls[i], i_Data);
    }
}

void Warp_Z::EndLoad() {
    S32 i;

    m_ResObjLink.EndLoad();

    for (i = 0; i < MAX_WARP_FACE; i++) {
        gData.ClassMgr->UpdateLink(m_MaterialAnimHdls[i]);
    }

    gData.ClassMgr->UpdateLink(m_RootNodeHdl);

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_AnimFrameHdls[i]);
    }
}

void Warp_Z::AfterEndLoad() {
    for (S32 i = 0; i < MAX_WARP_FACE; i++) {
        m_MaterialHdls[i] = m_MaterialAnimHdls[i]->GetMaterial();
    }
}

void Warp_Z::LoadDone() {
    if (m_RootNodeHdl.IsValid()) {
        m_RootNodeHdl->EnableFlag(FL_NODE_SCAN | FL_NODE_UPDATE_LIGHTING);
    }

    Update();
}
