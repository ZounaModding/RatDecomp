#include "Surface_Z.h"
#include "ClassManager_Z.h"
#include "Material_Z.h"
#include "MaterialAnim_Z.h"
#include "Memory_Z.h"
#include "SeadIndex_Z.h"
#include "Sys_Z.h"

void Surface_Z::Load(void** i_Data) {
    S32 l_Nb;
    Bool l_HasSead;

    Points_Z::Load(i_Data);

    l_Nb = m_EdgeColTab.GetSize();

    LOADDYNARRAY_Z(m_EdgeColTab);
    LOADDYNARRAY_Z(m_ClingEdgeColTab);

    LOAD_Z(l_Nb);
    m_PatchTab.SetSize(l_Nb);

    for (S32 i = 0; i < l_Nb; i++) {
        LOADRANGE_Z(m_PatchTab[i].m_Flag, m_PatchTab[i].m_UnkIndex_0xaa);
        gData.ClassMgr->LoadLink(m_PatchTab[i].m_MaterialAnimHdl, i_Data);
    }

    LOADDYNARRAY_Z(m_EdgeTab);
    LOADDYNARRAY_Z(m_Normals);
    LOADDYNARRAY_Z(m_Colors);
    LOADDYNARRAY_Z(m_UVs);
    LOADDYNARRAY_Z(m_OccluderZoneIndices);
    LOADDYNARRAY_Z(m_STreeTab);

    LOAD_Z(l_HasSead);

    if (l_HasSead) {
        m_Sead = NewL_Z(63) SeadIndex_Z;
        m_Sead->Load(i_Data);
    }
    else {
        m_Sead = NULL;
    }
}

void Surface_Z::EndLoad() {
    Object_Z::EndLoad();

    for (S32 i = 0; i < m_PatchTab.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_PatchTab[i].m_MaterialAnimHdl);
    }
}

void Surface_Z::AfterEndLoad() {
    for (S32 i = 0; i < m_PatchTab.GetSize(); i++) {
        m_PatchTab[i].m_Material = m_PatchTab[i].m_MaterialAnimHdl->GetMaterial();
    }
}
