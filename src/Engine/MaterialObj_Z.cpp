#include "MaterialObj_Z.h"
#include "MaterialAnim_Z.h"
#include "Sys_Z.h"

Bool MaterialObj_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        if (m_MaterialAnimHdls[i].IsValid()) {
            m_MaterialAnimHdls[i]->MarkHandles();
        }
    }

    return TRUE;
}

void MaterialObj_Z::Load(void** i_Data) {
    S32 l_MaterialAnimNb;
    LOAD_Z(l_MaterialAnimNb);
    m_MaterialAnimHdls.SetSize(l_MaterialAnimNb);

    for (S32 i = 0; i < l_MaterialAnimNb; i++) {
        gData.ClassMgr->LoadLink(m_MaterialAnimHdls[i], i_Data);
    }
}

void MaterialObj_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    m_MaterialAnimHdls.SetSize(m_MaterialAnimHdls.GetSize());

    for (S32 i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_MaterialAnimHdls[i]);
    }
}

MaterialAnim_ZHdl MaterialObj_Z::GetMaterialByName(const Name_Z& i_Name) {
    for (S32 i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        MaterialAnim_Z* l_MaterialAnim = m_MaterialAnimHdls[i];
        if (l_MaterialAnim->GetName() == i_Name) {
            return l_MaterialAnim->GetHandle();
        }
    }

    return HANDLE_NULL;
}
