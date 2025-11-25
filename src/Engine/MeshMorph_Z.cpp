#include "MeshMorph_Z.h"
#include "Sys_Z.h"
#include "Program_Z.h"

void Morpher_Z::Load(void** i_Data) {
    S32 l_Nb;

    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MorphValues.SetSize(l_Nb);
    MEMCPYFROM_Z(m_MorphValues.GetArrayPtr(), *i_Data, l_Nb * sizeof(MorphValue_Z));

    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MorphTargetDescDA.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        m_MorphTargetDescDA[i].Load(i_Data);
    }
}

void MorphTargetDesc_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadName(m_MeshName, i_Data);
    m_MeshID = gData.AnimMgr->GetMeshByName(m_MeshName);

    S32 l_Nb;
    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MorphValues.SetSize(l_Nb);
    MEMCPYFROM_Z(m_MorphValues.GetArrayPtr(), *i_Data, l_Nb * sizeof(MorphValue_Z));
}
