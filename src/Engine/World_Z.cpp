#include "World_Z.h"
#include "Node_Z.h"
#include "Warp_Z.h"
#include "Manipulator_Z.h"
#include "ABC_ScriptManager.h"
#include "CameraZone_Z.h"
#include "SystemDatas_Z.h"
#include "HField_Z.h"
#include "GenWorld_Z.h"
#include "Occluder_Z.h"
#include "Flare_Z.h"
#include "GameObj_Z.h"
#include "AnimFrame_Z.h"
#include "Graph_Z.h"
#include "SubWorld_Z.h"

WorldManager_Z::WorldManager_Z() { }

WorldManager_Z::~WorldManager_Z() { }

void WorldManager_Z::AddWorld(World_ZHdl i_WorldHdl) {
    S16 l_WorldId = m_WorldHdls.Add(i_WorldHdl);
    i_WorldHdl->RefWorldId(l_WorldId);
    gData.ScriptMgr->WorldAdded(i_WorldHdl);
}

World_Z::~World_Z() {
    if (gData.SystemDatas) {
        if (m_GameObjHdl.IsValid()) {
            gData.SystemDatas->RemoveExtGameObj(m_GameObjHdl);
        }
    }
}

Bool World_Z::MarkHandles() {
    S32 i;

    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_RootNodeHdl.IsValid()) {
        m_RootNodeHdl->MarkHandles();
        Assume();
    }

    if (GetHField().IsValid()) {
        GetHField()->MarkHandles();
    }

    if (GetHField().IsValid()) {
        GetHField()->MarkHandles();
    }

    if (GetGenWorld().IsValid()) {
        GetGenWorld()->MarkHandles();
    }

    for (i = 0; i < m_NoSeadNodeHdls.GetSize(); i++) {
        if (m_NoSeadNodeHdls[i].IsValid()) {
            m_NoSeadNodeHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_AnimFrameHdls.GetSize(); i++) {
        if (m_AnimFrameHdls[i].IsValid()) {
            m_AnimFrameHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_CameraZoneHdls.GetSize(); i++) {
        m_CameraZoneHdls[i]->MarkHandles();
    }

    for (i = 0; i < m_GraphHdls.GetSize(); i++) {
        m_GraphHdls[i]->MarkHandles();
    }

    for (i = 0; i < m_OccluderHdls.GetSize(); i++) {
        m_OccluderHdls[i]->MarkHandles();
    }

    for (i = 0; i < m_FlareHdls.GetSize(); i++) {
        m_FlareHdls[i]->MarkHandles();
    }

    if (m_WarpHdl.IsValid()) {
        m_WarpHdl->MarkHandles();
    }

    if (m_GameObjHdl.IsValid()) {
        m_GameObjHdl->MarkHandles();
    }

    for (i = 0; i < m_NoFrustumClippingNodeHdls.GetSize(); i++) {
        if (m_NoFrustumClippingNodeHdls[i].IsValid()) {
            m_NoFrustumClippingNodeHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_SubWorldDatas.GetSize(); i++) {
        if (m_SubWorldDatas[i].m_SubWorldHdl.IsValid()) {
            m_SubWorldDatas[i].m_SubWorldHdl->MarkHandles();
        }
    }

    for (i = 0; i < m_ManipulatorSceneDrawHdls.GetSize(); i++) {
        m_ManipulatorSceneDrawHdls[i]->MarkHandles();
    }

    return TRUE;
}

void World_Z::RefWorldId(S16 i_WorldId) {
    m_RootNodeHdl->SetWorldId(i_WorldId, TRUE);
}

void World_Z::Assume() { }

void World_Z::Update(Float i_DeltaTime) {
    UpdateAnims(i_DeltaTime);
    UpdateLighting(FALSE);
    m_RootNodeHdl->Update();
    GetWarp()->Update();
}

void World_Z::ActionOnActivate() {
    m_SeadDisplay.SetActive(TRUE);
    m_SeadCollide.SetActive(TRUE);

    for (S32 i = 0; i < m_ManipulatorSceneDrawHdls.GetSize(); i++) {
        m_ManipulatorSceneDrawHdls[i]->Activate();
    }
}

Node_ZHdl World_Z::GetNodeByName(const Name_Z& i_Name) const {
    Node_ZHdl l_NodeHdl;
    m_RootNodeHdl->GetByName(i_Name, l_NodeHdl, FALSE);

    if (!l_NodeHdl.IsValid()) {
        for (S32 i = 0; i < m_NoSeadNodeHdls.GetSize(); i++) {
            if (m_NoSeadNodeHdls[i]->GetName() == i_Name) {
                return m_NoSeadNodeHdls[i];
            }
        }
    }

    return l_NodeHdl;
}

S32 World_Z::GetNodesByObjectType(Node_ZHdlDA& o_Nodes, S32 i_Type) const {
    o_Nodes.Flush();
    m_RootNodeHdl->GetByObjectType(o_Nodes, i_Type, FALSE);
    return o_Nodes.GetSize();
}

void World_Z::NoFrustrumClipping(const Node_ZHdl& i_NodeHdl, Bool i_NoClipping) {
    for (S32 i = 0; i < m_NoFrustumClippingNodeHdls.GetSize(); i++) {
        if (m_NoFrustumClippingNodeHdls[i] == i_NodeHdl) {
            if (!i_NoClipping) {
                m_NoFrustumClippingNodeHdls.Remove(i);
                return;
            }
            break;
        }
    }

    i_NodeHdl->UnRef();
    i_NodeHdl->EnableFlag(FL_NODE_NO_SEADCOLLIDE | FL_NODE_NO_SEADDISPLAY);
    m_NoFrustumClippingNodeHdls.Add(i_NodeHdl);
}

Bool World_Z::GetCameraZone(const Vec3f& i_WorldPosition, CameraZoneData_Z& o_Data, S32 i_CameraZoneId) {
    S32 l_CameraZoneNb = m_CameraZoneHdls.GetSize();
    while (--l_CameraZoneNb >= 0) {
        if (i_CameraZoneId >= 0 && i_CameraZoneId != l_CameraZoneNb) {
            continue;
        }

        CameraZone_ZHdl l_CameraZoneHdl = m_CameraZoneHdls[l_CameraZoneNb];
        if (l_CameraZoneHdl->GetCameraZoneData(i_WorldPosition, o_Data)) {
            o_Data.m_CameraZoneHdl = l_CameraZoneHdl;
            return TRUE;
        }
    }

    return FALSE;
}

void World_Z::NoOccluderClipping(const Node_ZHdl& i_NodeHdl, Bool i_NoClipping) {
}
