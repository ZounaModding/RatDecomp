#include "ResourceObject_Z.h"
#include "Program_Z.h"

void ResourceObjectLink_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAYSIZE_Z(m_LinkedResourceDA);
    while (l_Nb--) {
        gData.ClassMgr->LoadLink(m_LinkedResourceDA[l_Nb], i_Data);
    }
}

void ResourceObjectLink_Z::EndLoad() {
    S32 i = m_LinkedResourceDA.GetSize();
    while (i--) {
        gData.ClassMgr->UpdateLink(m_LinkedResourceDA[i]);
    }
}

void ResourceObjectLink_Z::UpdateLinkFromId(BaseObject_ZHdl& o_Hdl, S32 i_ID) const {
    S32 i = m_LinkedResourceDA.GetSize();
    while (i--) {
        if (gData.ClassMgr->GetHandleName(m_LinkedResourceDA[i]).GetID() == i_ID) {
            o_Hdl = m_LinkedResourceDA[i];
            return;
        }
    }
    ASSERTL_Z(FALSE, "ResourceObjectLink_Z::UpdateLinkFromId", 39);
}

void ResourceObjectLink_Z::MarkHandles() {
    S32 i = m_LinkedResourceDA.GetSize();
    while (i--) {
        gData.ClassMgr->MarkHandles(m_LinkedResourceDA[i]);
    }
}

// TODO: Finish matching (regswap)
Bool ResourceObject_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }
    m_ResObjLink.MarkHandles();
    return TRUE;
}

void ResourceObject_Z::Load(void** i_Data) { }
