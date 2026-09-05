#include "Lod_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "LightData_Z.h"
#include "Main_Z.h"
#include "Memory_Z.h"
#include "Mesh_Z.h"
#include "MeshCollision_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "SoundManager_Z.h"
#include "Sys_Z.h"
#include "UserDefine_Z.h"

void Lod_Z::Load(void** i_Data) {
    S32 l_Nb;

    Object_Z::Load(i_Data);

    Mesh_Z::Load(i_Data, &m_ShadowSphere);
    Mesh_Z::Load(i_Data, m_SphereCollisions);
    Mesh_Z::Load(i_Data, m_BoxCollisions);
    Mesh_Z::Load(i_Data, m_CylindreCollisions);

    // $SABE: Might be worth making a macro for this
    l_Nb = (U8*)&m_LodDistanceScale - (U8*)&m_NearFadeThreshold + sizeof(m_LodDistanceScale);
    LOADBYTES_Z(&m_NearFadeThreshold, l_Nb);
    LOADDYNARRAYSIZE_Z(m_ObjectHdls);

    for (S32 i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->LoadLink(m_ObjectHdls[i], i_Data);
    }

    gData.ClassMgr->LoadLink(m_VolumeObjectHdl, i_Data);

    const Name_Z& l_RscName = gData.ClassMgr->GetHandleName(GetHandle());

    if (IsFlag(FL_IS_LOD_SKIN)) {
        gData.AnimMgr->Load(GetName(), l_RscName, i_Data);
    }

    gData.SoundMgr->Load(GetName(), l_RscName, i_Data);
    gData.ClassMgr->LoadLinkId(&m_UserDefine, i_Data);
}

void Lod_Z::EndLoad() {
    Object_Z::EndLoad();

    for (S32 i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_ObjectHdls[i]);
    }

    gData.ClassMgr->UpdateLink(m_VolumeObjectHdl);

    const Name_Z& l_RscName = gData.ClassMgr->GetHandleName(GetHandle());

    if (IsFlag(FL_IS_LOD_SKIN)) {
        gData.AnimMgr->UpdateLinks(GetName(), l_RscName);
    }

    gData.SoundMgr->UpdateLinks(GetName(), l_RscName);
    gData.ClassMgr->UpdateLinkPtr(&m_UserDefine);
}

void LodData_Z::Load(void** i_Data) {
    S32 l_Nb;
    Bool l_HasActorData;

    ObjectDatas_Z::Load(i_Data);

    LOADDYNARRAYSIZE_Z(m_ObjectDatasHdls);

    for (S32 i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_ObjectDatasHdls[i], i_Data);
    }

    gData.ClassMgr->LoadLink(m_VolumeObjectDatasHdl, i_Data);

    LOAD_Z(l_HasActorData);

    if (l_HasActorData) {
        m_ActorData = NewL_Z(81) ActorData_Z;
        m_ActorData->Load(i_Data);
    }
}

void LodData_Z::EndLoad() {
    ObjectDatas_Z::EndLoad();

    if (m_ActorData) {
        m_ActorData->EndLoad();
    }

    for (S32 i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_ObjectDatasHdls[i]);
    }

    gData.ClassMgr->UpdateLink(m_VolumeObjectDatasHdl);
}

void ActorData_Z::Load(void** i_Data) {
    S32 l_LightFlag;
    S32 l_NbIndices;
    S32 l_Nb;
    S32 i;

    LOADRANGE_Z(m_LocalShadowPos, m_LocalShadowBounds);
    LOAD_Z(l_LightFlag);
    LOADRANGE_Z(m_LightData.GetDirection(), m_LightData.GetAmbient());

    m_LightData.SetFlag(l_LightFlag);
    gData.ClassMgr->LoadLink(m_CurLightDataHdl, i_Data);

    LOAD_Z(l_Nb);
    m_ShadowReceivers.SetSize(l_Nb);

    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_ShadowReceivers[i].m_NodeHdl, i_Data);
        gData.ClassMgr->LoadLink(m_ShadowReceivers[i].m_ObjectHdl, i_Data);
        gData.ClassMgr->LoadLink(m_ShadowReceivers[i].m_ObjectDataHdl, i_Data);

        LOAD_Z(l_NbIndices);
        m_ShadowReceivers[i].m_ShadowReceiverElementIndices.SetSize(l_NbIndices);
        LOADARRAY_Z(m_ShadowReceivers[i].m_ShadowReceiverElementIndices.GetArrayPtr(), l_NbIndices);
    }
}

void ActorData_Z::EndLoad() {
    gData.ClassMgr->UpdateLink(m_CurLightDataHdl);

    for (S32 i = 0; i < m_ShadowReceivers.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_ShadowReceivers[i].m_NodeHdl);
        gData.ClassMgr->UpdateLink(m_ShadowReceivers[i].m_ObjectHdl);
        gData.ClassMgr->UpdateLink(m_ShadowReceivers[i].m_ObjectDataHdl);
    }
}
