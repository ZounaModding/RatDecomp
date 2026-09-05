#include "MaterialManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Types_Z.h"
#include "WaterHeightMap_Z.h"

MaterialManager_Z::MaterialManager_Z() {
    m_IsPaused = FALSE;
    RegisterMaterialUser(FL_MTL_CODE_EAU, WaterHeightMap_Z::NewObject);
    RegisterMaterialUser(FL_MTL_CODE_WATER_OCEAN, WaterHeightMap_Z::NewObject);
    RegisterMaterialUser(FL_MTL_CODE_EAU_DYNAMIC, WaterHeightMap_Z::NewObject);
}

MaterialManager_Z::~MaterialManager_Z() {
}

void MaterialManager_Z::MarkHandles() {
    S32 i;

    for (i = 0; i < m_MaterialAnimHdls.GetSize();) {
        if (!m_MaterialAnimHdls[i].IsValid()) {
            m_MaterialAnimHdls.Remove(i);
        }
        else if (!gData.ClassMgr->IsMarked(m_MaterialAnimHdls[i])) {
            m_MaterialAnimHdls.Remove(i);
        }
        else {
            i++;
        }
    }

    for (i = 0; i < m_MaterialAnimHdlsReallyAnimated.GetSize();) {
        if (!m_MaterialAnimHdlsReallyAnimated[i].IsValid()) {
            m_MaterialAnimHdlsReallyAnimated.Remove(i);
        }
        else if (!gData.ClassMgr->IsMarked(m_MaterialAnimHdlsReallyAnimated[i])) {
            m_MaterialAnimHdlsReallyAnimated.Remove(i);
        }
        else {
            i++;
        }
    }
}

void MaterialManager_Z::Update(Float i_DeltaTime) {
    if (m_IsPaused) {
        return;
    }
    for (S32 i = 0; i < m_MaterialAnimHdlsReallyAnimated.GetSize(); i++) {
        m_MaterialAnimHdlsReallyAnimated[i]->Update(i_DeltaTime);
    }
}

void MaterialManager_Z::GetMaterialByName(const Name_Z& i_Name, MaterialAnim_ZHdl& o_MaterialAnimHdl) {
    o_MaterialAnimHdl = HANDLE_NULL;
    S32 l_Nb = m_MaterialAnimHdls.GetSize();
    for (S32 i = 0; i < l_Nb; i++) {
        MaterialAnim_Z* l_MatAnim = m_MaterialAnimHdls[i];
        if (l_MatAnim && l_MatAnim->GetName() == i_Name) {
            o_MaterialAnimHdl = m_MaterialAnimHdls[i];
            break;
        }
    }
}

void MaterialManager_Z::AddMaterialAnim(const MaterialAnim_ZHdl& i_MaterialAnimHdl) {
    if (!i_MaterialAnimHdl.IsValid()) return;

    Bool l_Found = FALSE;
    for (S32 i = 0; !l_Found && i < m_MaterialAnimHdls.GetSize(); i++) {
        if (i_MaterialAnimHdl == m_MaterialAnimHdls[i]) {
            l_Found = TRUE;
        }
    }
    if (l_Found) {
        return;
    }

    m_MaterialAnimHdls.Add(i_MaterialAnimHdl);
    if (i_MaterialAnimHdl->IsAnimated()) {
        m_MaterialAnimHdlsReallyAnimated.Add(i_MaterialAnimHdl);
    }
}

const MaterialAnim_ZHdl& MaterialManager_Z::GetMaterialAnimFromMaterial(const Material_ZHdl& i_MaterialHdl) {
    for (S32 i = 0; i < m_MaterialAnimHdls.GetSize(); i++) {
        if (i_MaterialHdl.GetGlobalID() == m_MaterialAnimHdls[i]->GetMaterial().GetGlobalID()) {
            return m_MaterialAnimHdls[i];
        }
    }

    return m_MaterialAnimHdls[0];
}

void MaterialManager_Z::RegisterMaterialUser(S32 i_MaterialCode, NewMaterialUserProc i_NewMaterialUserProc) {
    for (S32 i = 0; i < m_MaterialUsers.GetSize(); i++) {
        if (i_MaterialCode == m_MaterialUsers[i].m_Code) {
            return;
        }
    }
    S32 l_Idx = m_MaterialUsers.Add();
    m_MaterialUsers[l_Idx].m_Code = i_MaterialCode;
    m_MaterialUsers[l_Idx].m_NewMaterialUserProc = i_NewMaterialUserProc;
}

MaterialUser_Z* MaterialManager_Z::GetMaterialUser(S32 i_MaterialCode) {
    if (i_MaterialCode != FL_MTL_CODE_DIFFUSE) {
        for (S32 i = 0; i < m_MaterialUsers.GetSize(); i++) {
            if (i_MaterialCode == m_MaterialUsers[i].m_Code) {
                return m_MaterialUsers[i].m_NewMaterialUserProc();
            }
        }
    }
    return NULL;
}
