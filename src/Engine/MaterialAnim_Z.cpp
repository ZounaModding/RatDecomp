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

void MaterialAnim_Z::Clone(MaterialAnim_ZHdl& i_MaterialAnimHdl) {
}

void MaterialAnim_Z::Reset() {
    SetAnimTime(0.0f);
    m_CurTime = 0.0f;
    m_PlayFlag &= FL_MTL_PLAYONCE | FL_MTL_NEVERAGAIN | FL_MTL_AUTOSTART;
    if ((m_PlayFlag & FL_MTL_AUTOSTART) && m_MaxTime != 0.0f) {
        m_PlayFlag |= FL_MTL_PLAY;
    }
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

void MaterialAnim_Z::DoStart() {
    if ((!(m_PlayFlag & FL_MTL_PLAY) && (m_PlayFlag & FL_MTL_NOT_RESTART) != FL_MTL_NOT_RESTART) || (m_PlayFlag & (FL_MTL_NOT_RESTART | FL_MTL_NEVERAGAIN)) == FL_MTL_NOT_RESTART) {
        m_PlayFlag |= FL_MTL_PLAY;
        m_PlayFlag &= ~FL_MTL_PLAYED;
        m_CurTime = 0.0f;
    }
}

void MaterialAnim_Z::Stop() {
    m_PlayFlag &= ~FL_MTL_PLAY;
}

void MaterialAnim_Z::SetAnimTime(Float i_Time) {
    Material_Z* l_Material = m_MaterialHdl;

    if (m_TransKfr.GetNbKeys()) {
        m_TransKfr.GetValue(i_Time, l_Material->m_Translation);
    }
    if (m_ScaleKfr.GetNbKeys()) {
        m_ScaleKfr.GetValue(i_Time, l_Material->m_Scale);
    }
    if (m_RotKfr.GetNbKeys()) {
        m_RotKfr.GetValue(i_Time, l_Material->m_Rotation);
    }
    if (m_DiffuseColorKfr.GetNbKeys()) {
        m_DiffuseColorKfr.GetValue(i_Time, l_Material->m_DiffuseColor);
    }
    if (m_EmissiveColorKfr.GetNbKeys()) {
        m_EmissiveColorKfr.GetValue(i_Time, l_Material->m_EmissiveColor);
    }
    if (m_SpecularKfr.GetNbKeys()) {
        m_SpecularKfr.GetValue(i_Time, l_Material->m_Specular);
    }
    if (m_ParamsKfr.GetNbKeys()) {
        m_ParamsKfr.GetValue(i_Time, l_Material->m_Params);
        l_Material->GetParams().x /= 100.0f;
    }
    if (m_CollisionFlagKfr.GetNbKeys()) {
        m_CollisionFlagKfr.GetValue(i_Time, l_Material->m_ColFlag);
    }
    if (m_ObjectFlagKfr.GetNbKeys()) {
        m_ObjectFlagKfr.GetValue(i_Time, l_Material->m_ObjectFlag);
    }

    Bool l_MaterialChanged = FALSE;
    if (m_BmapHdlKfr.GetNbKeys()) {
        m_BmapHdlKfr.GetValue(i_Time, l_Material->m_BmapHdls[Material_Z::mtl_diffuse]);
        l_MaterialChanged = TRUE;
    }
    if (m_DiffuseAlphaKfr.GetNbKeys()) {
        m_DiffuseAlphaKfr.GetValue(i_Time, l_Material->m_DiffuseOpacity);
        l_MaterialChanged = TRUE;
    }
    if (m_RenderFlagKfr.GetNbKeys()) {
        m_RenderFlagKfr.GetValue(i_Time, l_Material->m_RdrFlag);
        l_MaterialChanged = TRUE;
    }
    if (l_MaterialChanged) {
        l_Material->Changed();
    }
    l_Material->UpdateTMatrix();
}
