#include "GCMain_Z.h"
#include "Material_Z.h"
#include "ResourceObject_Z.h"
#include "Sys_Z.h"

Extern_Z GCGlobals gData;

void Material_Z::Load(void** i_Data) {
    LOADRANGE_Z(m_DiffuseColor, m_Flag);
    for (S32 i = 0; i < mtl_nb_params; i++) {
        gData.ClassMgr->LoadLink(m_BmapHdl[i], i_Data);
    }
}

void Material_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    for (S32 i = 0; i < mtl_nb_params; i++) {
        gData.ClassMgr->UpdateLink(m_BmapHdl[i]);
    }
    Changed();
}
