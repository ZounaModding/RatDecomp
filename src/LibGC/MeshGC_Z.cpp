#include "MeshGC_Z.h"
#include "AnimationManager_Z.h"
#include "GCMain_Z.h"
#include "Sys_Z.h"
#include <dolphin.h>

void MeshGC_Z::Load(void** i_Data) {
    Mesh_Z::Load(i_Data);

    S32 l_Size;
    LOADDYNARRAYNB_Z(m_OptimizedTextureCoordinates, l_Size);
    LOADDYNARRAYNB_Z(m_OptimizedNormals, l_Size);
    LOADDYNARRAYNBL_Z(m_DisplayLists, l_Size);
    LOADDYNARRAYNB_Z(m_MaterialIndices, l_Size);
}

void MeshGC_Z::AfterEndLoad() {
    Mesh_Z::AfterEndLoad();
    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (S32 i = 0; i < m_MaterialIndices.GetSize(); i++) {
            Name_Z l_MaterialName;
            gData.ClassMgr->GetNameFromId(m_MaterialIndices[i], l_MaterialName);
            m_MaterialIndices[i] = gData.AnimMgr->GetMaterialByName(l_MaterialName);
        }
    }
    m_StreamList.m_PosData = m_OptimizedVertices.GetArrayPtr();
    m_StreamList.m_NormalData = m_OptimizedNormals.GetArrayPtr();
    m_StreamList.m_Tex0Data = m_OptimizedTextureCoordinates.GetArrayPtr();
    m_StreamList.m_Tex1Data = m_OptimizedTextureCoordinates.GetArrayPtr();
    DCStoreRangeNoSync(m_StreamList.m_PosData, m_OptimizedVertices.GetSize() * sizeof(Vec3f_S16_Z));
    DCStoreRangeNoSync(m_StreamList.m_NormalData, m_OptimizedNormals.GetSize());
    DCStoreRangeNoSync(m_StreamList.m_Tex0Data, m_OptimizedTextureCoordinates.GetSize() * sizeof(S16));
    PPCSync();
}

void MeshGC_Z::Clean() {
    Mesh_Z::Clean();
    m_OptimizedNormals.Flush();
    m_OptimizedTextureCoordinates.Flush();
    m_MaterialIndices.Flush();
    m_DisplayLists.Flush();
}

void MeshDataGC_Z::AfterEndLoad() {
    m_MeshVolume.m_UnkVertices.Flush();
}
