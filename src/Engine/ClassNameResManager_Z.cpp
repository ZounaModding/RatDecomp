#include "ClassNameResManager_Z.h"
#include "Sys_Z.h"
#include "Program_Z.h"

void ClassNameResManager_Z::Shut() {
    m_ClassHT.Flush();

    for (S32 i = m_ClassResHA.FindFirst(); i >= 0; i = m_ClassResHA.FindNext(i)) {
        m_ClassResHA[i].Flush();
    }
}

void ClassNameResManager_Z::Load(const Name_Z& i_Name, const Name_Z& i_FileName, void** i_Data) {
    S32 l_ClassID;
    GetClassID(i_Name, i_FileName, l_ClassID);
    m_ClassResHA[l_ClassID].Load(i_Data);
}

void ClassNameResManager_Z::UpdateLinks(const Name_Z& i_Name, const Name_Z& i_FileName) {
    S32 l_ClassID;
    GetClassID(i_Name, i_FileName, l_ClassID);
    m_ClassResHA[l_ClassID].UpdateLinks();
}

Bool ClassNameResManager_Z::GetClassID(const Name_Z& i_Name, const Name_Z& i_FileName, S32& o_ClassID, Bool i_AddClass) {
    o_ClassID = m_ClassResHA.FindFirst();

    while (o_ClassID >= 0) {
        if (i_Name == m_ClassResHA[o_ClassID].m_Name && i_FileName == m_ClassResHA.Get(o_ClassID).m_FileName) {
            return TRUE;
        }

        o_ClassID = m_ClassResHA.FindNext(o_ClassID);
    }

    if (i_AddClass) {
        o_ClassID = m_ClassResHA.Add();
        m_ClassResHA[o_ClassID].m_Name = i_Name;
        m_ClassResHA[o_ClassID].m_FileName = i_FileName;
    }

    return FALSE;
}

Bool ClassNameResManager_Z::MarkHandles() {
    S32 l_ClassID;
    l_ClassID = m_ClassResHA.FindFirst();

    while (l_ClassID >= 0) {
        m_ClassResHA[l_ClassID].MarkHandles();
        l_ClassID = m_ClassResHA.FindNext(l_ClassID);
    }
    return TRUE;
}

void ClassNameResManager_Z::MarkHandlesFromClass(const Name_Z& i_Name, const Name_Z& i_FileName) {
    S32 l_ClassID;
    if (GetClassID(i_Name, i_FileName, l_ClassID, FALSE)) {
        m_ClassResHA[l_ClassID].MarkHandles();
    }
}

Bool ClassNameResManager_Z::Minimize() {
    S32 l_ClassID;
    m_ClassHT.Minimize();
    m_ClassResHA.Minimize();
    l_ClassID = m_ClassResHA.FindFirst();
    while (l_ClassID >= 0) {
        m_ClassResHA[l_ClassID].Minimize();
        l_ClassID = m_ClassResHA.FindNext(l_ClassID);
    }
    return TRUE;
}

void ClassNameResManager_Z::RemoveClassId(const S32 i_ClassID) {
    if (!m_ClassResHA.IsElement(i_ClassID)) {
        return;
    }

    m_ClassResHA[i_ClassID].Flush();
    m_ClassResHA.Remove(i_ClassID);
}

void ClassNameResManager_Z::RemoveClassId(const Name_Z& i_Name, const Name_Z& i_FileName) {
    S32 l_ClassID;
    if (GetClassID(i_Name, i_FileName, l_ClassID, FALSE)) {
        RemoveClassId(l_ClassID);
    }
}

BaseObject_ZHdl ClassNameResManager_Z::GetObjectInClass(const S32 i_ClassID, const S32 i_EnumID) {
    S32Hash_Z l_HashElt(i_EnumID);
    const S32Hash_Z* l_Result = m_ClassResHA[i_ClassID].m_ResourceHT.Search(l_HashElt);

    if (!l_Result) {
        return HANDLE_NULL;
    }
    else {
        return gData.ClassMgr->U32ToHandle(l_Result->m_Ref);
    }
}

void ClassNameRes_Z::Load(void** i_Data) {
    S32 l_NbHashElt;
    S32Hash_Z l_HashElt;
    LOAD_Z(l_NbHashElt);

    for (S32 i = 0; i < l_NbHashElt; i++) {
        LOADBYTES_Z(&l_HashElt, 4);
        gData.ClassMgr->LoadLinkId(&l_HashElt.m_Ref, i_Data);
        m_ResourceHT.Insert(l_HashElt);
    }
}

void ClassNameRes_Z::UpdateLinks() {
    m_ResourceHT.InitScan();

    S32Hash_Z* l_HashElt = m_ResourceHT.NextScan();

    while (l_HashElt) {
        BaseObject_ZHdl l_Hdl;

        gData.ClassMgr->UpdateLinkFromId(l_Hdl, l_HashElt->m_Ref);

        l_HashElt->m_Ref = gData.ClassMgr->HandleToU32(l_Hdl);
        l_HashElt = m_ResourceHT.NextScan();
    }
}

void ClassNameRes_Z::MarkHandles() {
    m_ResourceHT.InitScan();

    S32Hash_Z* l_HashElt = m_ResourceHT.NextScan();

    while (l_HashElt) {
        BaseObject_ZHdl l_Hdl;
        gData.ClassMgr->MarkU32Handle(l_HashElt->m_Ref);
        l_HashElt = m_ResourceHT.NextScan();
    }
}

static const Char* s_Unused = "Unknow Enum Symbol..";
