#include "MeshMorph_Z.h"
#include "Sys_Z.h"
#include "Program_Z.h"
#include "AnimationManager_Z.h"

void Morpher_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAY_Z(m_MorphValues);
    LOADDYNARRAYL_Z(m_MorphTargetDescDA);
}

void MorphTargetDesc_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadName(m_MeshName, i_Data);
    m_MeshID = gData.AnimMgr->GetMeshByName(m_MeshName);

    S32 l_Nb;
    LOADDYNARRAY_Z(m_MorphValues);
}
