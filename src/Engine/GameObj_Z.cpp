#include "GameObj_Z.h"
#include "Node_Z.h"
#include "Sys_Z.h"
#include "SystemDatas_Z.h"

GameObj_Z::~GameObj_Z() { }

Bool GameObj_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    for (S32 i = 0; i < m_NodeHdls.GetSize(); i++) {
        m_NodeHdls[i]->MarkHandles();
    }
    return TRUE;
}

void GameObj_Z::Load(void** i_Data) {
    S32 l_NodeNb;
    LOAD_Z(l_NodeNb);
    m_NodeHdls.SetSize(l_NodeNb);

    for (S32 i = 0; i < l_NodeNb; i++) {
        gData.ClassMgr->LoadLink(m_NodeHdls[i], i_Data);
    }
}

void GameObj_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    for (S32 i = 0; i < m_NodeHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_NodeHdls[i]);
        m_NodeHdls[i]->SetLight(gData.SystemDatas->GetDefaultLight());
    }
}

Node_ZHdl GameObj_Z::GetObjectByName(const Name_Z& i_Name, S16 i_GameObjId) {
    for (S32 i = 0; i < m_NodeHdls.GetSize(); i++) {
        Node_Z* l_Node = m_NodeHdls[i];
        if (l_Node->GetName() == i_Name && (i_GameObjId == 0 || i_GameObjId == l_Node->GetObject()->GetGeometryType())) {
            return m_NodeHdls[i];
        }
    }
    return HANDLE_NULL;
}
