#include "Mesh_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "Main_Z.h"

void Mesh_Z::EndLoad() {
    Object_Z::EndLoad();
    for (S32 i = 0; i < m_Materials.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_Materials[i]);
    }
}

void Mesh_Z::AfterEndLoad() {
    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (S32 i = 0; i < m_Strips.GetSize(); i++) {
            Name_Z l_MaterialName;
            gData.ClassMgr->GetNameFromId(m_Strips[i].m_MaterialId, l_MaterialName);
            m_Strips[i].m_MaterialId = gData.AnimMgr->GetMaterialByName(l_MaterialName);
        }
    }
}

void Mesh_Z::EndLoadLinks() {
    Object_Z::EndLoadLinks();
    for (S32 i = 0; i < m_Materials.GetSize(); i++) {
        m_ResObjLink.UpdateLinkFromId(m_Materials[i], m_Materials[i].GetGlobalID());
    }
}

void MeshData_Z::Load(void** i_Data) {
    ObjectDatas_Z::Load(i_Data);
    m_MeshVolume.Load(i_Data);
}
