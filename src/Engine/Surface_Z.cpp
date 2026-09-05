#include "Surface_Z.h"
#include "ClassManager_Z.h"
#include "SeadIndex_Z.h"
#include "SurfaceCache_Z.h"
#include "MaterialAnim_Z.h"

Surface_Z::Surface_Z() {
    SetGeometryType(SURFACE_Z);
    m_Sead = NULL;
    EnableFlag(FL_IS_SURFACE_UNK_0x200000);
}

Surface_Z::~Surface_Z() {
    Clean();
}

Bool Surface_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_PatchTab.GetSize(); i++) {
        m_PatchTab[i].m_MaterialAnimHdl->MarkHandles();
    }

    return TRUE;
}

void Surface_Z::Clean() {
    Points_Z::Clean();
    if (m_Sead) {
        Delete_Z m_Sead;
    }
    m_Sead = NULL;

    if (gData.ColSurfaceCache) {
        Patch* l_Patch = m_PatchTab.GetArrayPtr();
        S32 i;

        for (i = m_PatchTab.GetSize(); i != 0; i--) {
            if (l_Patch->m_ColCacheId != U16_MAX) {
                gData.ColSurfaceCache->FreeEntry(l_Patch->m_ColCacheId);
            }
            l_Patch++;
        }
    }

    m_EdgeTab.Flush();
    m_EdgeColTab.Flush();
    m_ClingEdgeColTab.Flush();
    m_PatchTab.Flush();
    m_STreeTab.Flush();
    m_Normals.Flush();
    m_Colors.Flush();
    m_UVs.Flush();
    m_OccluderZoneIndices.Flush();
    DisableFlag(FL_IS_SURFACE_UNK_0x800000);
}

void SurfaceDatas_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("SurfaceDatas_Z", 0));
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());
    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);
}
