#include "CollisionVol_Z.h"
#include "ClassManager_Z.h"
#include "Program_Z.h"
#include "Sys_Z.h"

void CollisionVol_Z::Load(void** i_Data) {
    S32 l_Nb;
    S32 i;

    Object_Z::Load(i_Data);

    LOADDYNARRAYSIZE_Z(m_CollisionVolInfos);
    LOADARRAY_Z(m_CollisionVolInfos.GetArrayPtr(), m_CollisionVolInfos.GetSize());

    LOADBYTES_Z(&m_InMessage, sizeof(m_InMessage) + sizeof(m_OutMessage));

    for (i = 0; i < COLVOL_NB_PARAM; i++) {
        gData.ClassMgr->LoadLink(m_NodeParams[i], i_Data);
    }

    LOADARRAY_Z(m_FloatParams, COLVOL_NB_PARAM);

    LOADDYNARRAYSIZE_Z(m_AnimFrameHdls);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_AnimFrameHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_MaterialAnimHdls);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_MaterialAnimHdls[i], i_Data);
    }

    gData.ClassMgr->LoadName(m_AgentClassName, i_Data);

    LOAD_Z(m_AnimTime);
}

void CollisionVol_Z::EndLoad() {
    S32 i;

    Object_Z::EndLoad();

    for (i = 0; i < COLVOL_NB_PARAM; i++) {
        gData.ClassMgr->UpdateLink(m_NodeParams[i]);
    }

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_AnimFrameHdls[i]);
    }

    for (i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_MaterialAnimHdls[i]);
    }
}
