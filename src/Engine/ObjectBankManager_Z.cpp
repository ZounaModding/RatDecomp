#include "ObjectBankManager_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "Program_Z.h"

ObjectBankManager_Z::ObjectBankManager_Z() {
    Char l_NodeNameStr[64];

    m_NodeHdls.SetSize(BANKMGR_MAX_NODE);
    m_NodeStatus.SetSize(BANKMGR_MAX_NODE);
    m_NodeStatus.ClearAllBits();

    for (S32 i = 0; i < BANKMGR_MAX_NODE; i++) {
        sprintf(l_NodeNameStr, "BankNode#%04d", i);
        Name_Z l_ClassName("Node_Z");
        Name_Z l_NodeName(l_NodeNameStr);
        m_NodeHdls[i] = gData.ClassMgr->NewObject(l_ClassName, l_NodeName);
    }
}

ObjectBankManager_Z::~ObjectBankManager_Z() {
}

void ObjectBankManager_Z::MarkHandles() {
    S32 i;

    for (i = m_Banks.FindFirst(); i >= 0; i = m_Banks.FindNext(i)) {
        for (S32 j = 0; j < m_Banks[i].m_ObjectDatasHdls.GetSize(); j++) {
            m_Banks[i].m_ObjectDatasHdls[j]->MarkHandles();
        }
    }

    for (i = 0; i < m_NodeHdls.GetSize(); i++) {
        m_NodeHdls[i]->MarkHandles();
    }
}

Bool ObjectBankManager_Z::Minimize() {
    m_Banks.Minimize();
    return FALSE;
}
