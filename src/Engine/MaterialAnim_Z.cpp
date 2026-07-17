#include "MaterialAnim_Z.h"
#include "Material_Z.h"

MaterialAnim_Z::MaterialAnim_Z() {
    m_CurTime = 0.0f;
    m_MaxTime = 0.0f;
    m_PlayFlag = FL_MTL_NONE;
}

Bool MaterialAnim_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    m_BmapHdlKfr.MarkHandles();
    if (m_MaterialHdl.IsValid()) {
        m_MaterialHdl->MarkHandles();
    }
    return TRUE;
}

void MaterialAnim_Z::Update(Float i_DeltaTime) {
    if (m_PlayFlag & FL_MTL_PLAY) {
        m_CurTime += i_DeltaTime;
        if (m_CurTime > m_MaxTime && m_PlayFlag & FL_MTL_PLAYONCE) {
            m_PlayFlag |= FL_MTL_PLAYED;
            m_PlayFlag &= ~FL_MTL_PLAY;
            m_CurTime = m_MaxTime;
        }
        else {
            while (m_CurTime > m_MaxTime) {
                m_CurTime -= m_MaxTime;
            }
        }
        SetAnimTime(m_CurTime);

        Material_Z* l_Material = m_MaterialHdl;
        MaterialUser_Z* l_MatUser = l_Material->GetMaterialUser();
        if (l_MatUser) {
            l_MatUser->Update(l_Material);
        }
    }
}
