#include "MaterialAnim_Z.h"
#include "MaterialManager_Z.h"
#include "Program_Z.h"

void MaterialAnim_Z::Load(void** i_Data) {
    LOAD_Z(m_PlayFlag);
    LOAD_Z(m_MaxTime);
    m_BmapHdlKfr.Load(i_Data);
    m_TransKfr.Load(i_Data);
    m_ScaleKfr.Load(i_Data);
    m_RotKfr.Load(i_Data);
    m_DiffuseColorKfr.Load(i_Data);
    m_EmissiveColorKfr.Load(i_Data);
    m_DiffuseAlphaKfr.Load(i_Data);
    m_SpecularKfr.Load(i_Data);
    m_ParamsKfr.Load(i_Data);
    m_CollisionFlagKfr.Load(i_Data);
    m_RenderFlagKfr.Load(i_Data);
    m_ObjectFlagKfr.Load(i_Data);
    gData.ClassMgr->LoadLink(m_MaterialHdl, i_Data);
}

void MaterialAnim_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    m_BmapHdlKfr.UpdateLinks();
    gData.ClassMgr->UpdateLink(m_MaterialHdl);
}

void MaterialAnim_Z::AfterEndLoad() {
    gData.MaterialMgr->AddMaterialAnim((BaseObject_ZHdl) * this);
}
