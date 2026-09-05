#include "Skin_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "Main_Z.h"
#include "SoundManager_Z.h"
#include "Sys_Z.h"

// TODO: Finish matching
void Skin_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);

    S32 l_Nb;
    S32 i;
    S32 j;
    S32 l_VertexNb;
    LOAD_Z(l_Nb);
    m_ObjectHdls.SetSize(l_Nb);
    for (i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->LoadLink(m_ObjectHdls[i], i_Data);
    }

    LOAD_Z(l_Nb);
    m_RotshapeObjects.SetSize(l_Nb);
    LOADARRAY_Z(m_RotshapeObjects.GetArrayPtr(), l_Nb);

    LOAD_Z(l_Nb);
    m_Bones.SetSize(l_Nb);
    for (i = 0; i < m_Bones.GetSize(); i++) {
        LOAD_Z(m_Bones[i].m_NameId);
        LOAD_Z(l_Nb);
        m_Bones[i].m_ObjectBlends.SetSize(l_Nb);
        for (j = 0; j < l_Nb; j++) {
            LOAD_Z(m_Bones[i].GetObjectBlends()[j].m_ObjectId);
            LOAD_Z(l_VertexNb);
            m_Bones[i].GetObjectBlends()[j].m_Vertices.SetSize(l_VertexNb);
            LOADARRAY_Z(m_Bones[i].GetObjectBlends()[j].m_Vertices.GetArrayPtr(), l_VertexNb);
            LOAD_Z(l_VertexNb);
            m_Bones[i].GetObjectBlends()[j].m_Normals.SetSize(l_VertexNb);
            LOADARRAY_Z(m_Bones[i].GetObjectBlends()[j].m_Normals.GetArrayPtr(), l_VertexNb);
        }
    }

    Bool l_IsClassId;
    LOAD_Z(l_IsClassId);
    if (l_IsClassId) {
        const Name_Z& l_ResourceName = gData.ClassMgr->GetHandleName(GetHandle());
        gData.AnimMgr->Load(GetName(), l_ResourceName, i_Data);
        gData.SoundMgr->Load(GetName(), l_ResourceName, i_Data);
    }
}

void Skin_Z::EndLoad() {
    Object_Z::EndLoad();

    for (S32 i = 0; i < m_ObjectHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_ObjectHdls[i]);
    }

    const Name_Z& l_ResourceName = gData.ClassMgr->GetHandleName(GetHandle());
    S32 l_ClassId;
    Bool l_IsClassId = gData.AnimMgr->GetClassID(GetName(), l_ResourceName, l_ClassId, FALSE);
    if (l_IsClassId) {
        gData.AnimMgr->UpdateLinks(GetName(), l_ResourceName);
        gData.SoundMgr->UpdateLinks(GetName(), l_ResourceName);
    }

    Name_Z l_Name;
    for (S32 i = 0; i < m_Bones.GetSize(); i++) {
        gData.ClassMgr->GetNameFromId(m_Bones[i].m_NameId, l_Name);
        m_Bones[i].m_NameId = gData.AnimMgr->GetNodeByName(l_Name);
    }
}

void Skin_Z::AfterEndLoad() {
}
