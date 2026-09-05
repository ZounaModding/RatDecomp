#include "World_Z.h"
#include "Node_Z.h"
#include "ClassManager_Z.h"
#include "Sys_Z.h"
#include "Program_Z.h"
#include "SystemDatas_Z.h"

void World_Z::Load(void** i_Data) {
    S32 l_Nb;
    S32 i;
    S32 l_SubNb;

    ClassManager_Z* l_ClassMgr = gData.ClassMgr;

    LOADDYNARRAYSIZE_Z(m_NoSeadNodeHdls);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_NoSeadNodeHdls[i], i_Data);
    }

    l_ClassMgr->LoadLink(m_RootNodeHdl, i_Data);

    m_SeadDisplay.Load(i_Data);
    m_SeadCollide.Load(i_Data);

    LOADDYNARRAYSIZE_Z(m_AnimFrameHdls);
    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        l_ClassMgr->LoadLink(m_AnimFrameHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_CameraZoneHdls);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_CameraZoneHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_GraphHdls);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_GraphHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_OccluderHdls);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_OccluderHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_FlareHdls);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_FlareHdls[i], i_Data);
    }

    LOADDYNARRAYSIZE_Z(m_SubWorldDatas);

    SubWorldData_Z* l_SubWorldData = m_SubWorldDatas.GetArrayPtr();
    for (i = 0; i < l_Nb; i++, l_SubWorldData++) {
        LOADBYTES_Z(&l_SubWorldData->m_Type, (U8*)&l_SubWorldData->m_Range - (U8*)&l_SubWorldData->m_Type);
        l_SubWorldData->m_Range.Load(i_Data);

        LOAD_Z(l_SubNb);
        LOADBYTES_Z(l_SubWorldData->m_SubWorldName, l_SubNb);

        LOADDYNARRAYNB_Z(l_SubWorldData->m_SubIds, l_SubNb);
        LOADDYNARRAYNB_Z(l_SubWorldData->m_UnkIds_0x140, l_SubNb);
        LOADDYNARRAYNB_Z(l_SubWorldData->m_UnkIds_0x148, l_SubNb);
    }

    l_ClassMgr->LoadLink(m_WarpHdl, i_Data);
    l_ClassMgr->LoadLink(m_GameObjHdl, i_Data);
    l_ClassMgr->LoadLink(m_HFieldHdl, i_Data);
    l_ClassMgr->LoadLink(m_GenWorldHdl, i_Data);
}

void World_Z::EndLoad() {
    S32 i;

    m_ResObjLink.EndLoad();

    ClassManager_Z* l_ClassMgr = gData.ClassMgr;

    for (i = 0; i < m_NoSeadNodeHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_NoSeadNodeHdls[i]);
    }

    l_ClassMgr->UpdateLink(m_RootNodeHdl);

    m_SeadDisplay.EndLoad();
    m_SeadCollide.EndLoad();

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_AnimFrameHdls[i]);
    }

    for (i = 0; i < m_CameraZoneHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_CameraZoneHdls[i]);
    }

    for (i = 0; i < m_GraphHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_GraphHdls[i]);
    }

    for (i = 0; i < m_OccluderHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_OccluderHdls[i]);
    }

    for (i = 0; i < m_FlareHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_FlareHdls[i]);
    }

    l_ClassMgr->UpdateLink(m_WarpHdl);
    l_ClassMgr->UpdateLink(m_GameObjHdl);
    l_ClassMgr->UpdateLink(m_HFieldHdl);
    l_ClassMgr->UpdateLink(m_GenWorldHdl);

    if (m_GameObjHdl.IsValid()) {
        gData.SystemDatas->AddExtGameObj(m_GameObjHdl);
    }
}

void World_Z::AfterEndLoad() {
}

void World_Z::LoadDone() {
    gData.WorldMgr->AddWorld(GetHandle());
    NoClippingRoot(m_RootNodeHdl, TRUE);
    m_RootNodeHdl->EnableFlag(FL_NODE_UPDATE_LIGHTING);
    UpdateLighting(TRUE);
    UpdateLodData();
}

void World_Z::NoClippingRoot(const Node_ZHdl& i_NodeHdl, Bool i_NoClipping) {
    Node_ZHdlDA l_Nodes;
    S32 i;

    i_NodeHdl->GetByObjectType(l_Nodes, FLARE_Z);
    for (i = 0; i < l_Nodes.GetSize(); i++) {
        NoFrustrumClipping(l_Nodes[i], i_NoClipping);
    }

    l_Nodes.Empty();

    i_NodeHdl->GetNodesFromFlag(l_Nodes, FL_NODE_NO_OCCLUDER, 0);
    for (i = 0; i < l_Nodes.GetSize(); i++) {
        NoOccluderClipping(l_Nodes[i], i_NoClipping);
    }
}
