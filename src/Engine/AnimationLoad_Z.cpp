#include "Animation_Z.h"
#include "Sys_Z.h"

void Animation_Z::Load(void** i_Data) {
    MEMCPYFROM_Z(&m_MaxTime, *i_Data, sizeof(m_MaxTime));
    MEMCPYFROM_Z(&m_SmoothRatio, *i_Data, sizeof(m_SmoothRatio));
    MEMCPYFROM_Z(&m_Flag, *i_Data, sizeof(m_Flag));

    AnimationNode_Z::Load(m_NodeKfr, i_Data);
    AnimationMaterial_Z::Load(m_MtlKfr, i_Data);
    AnimationMesh_Z::Load(m_MeshKfr, i_Data);
    AnimationMorph_Z::Load(m_MorphKfr, i_Data);
    S32 l_Nb;
    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_NodeCct.SetSize(l_Nb);
    MEMCPYFROM_Z(m_NodeCct.GetArrayPtr(), *i_Data, l_Nb * sizeof(AnimationConcatNode_Z));
    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MtlCct.SetSize(l_Nb);
    MEMCPYFROM_Z(m_MtlCct.GetArrayPtr(), *i_Data, l_Nb * sizeof(AnimationConcatMtl_Z));
    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MeshCct.SetSize(l_Nb);
    MEMCPYFROM_Z(m_MeshCct.GetArrayPtr(), *i_Data, l_Nb * sizeof(AnimationConcat_Z));
    MEMCPYFROM_Z(&l_Nb, *i_Data, 4);
    m_MorphCct.SetSize(l_Nb);
    MEMCPYFROM_Z(m_MorphCct.GetArrayPtr(), *i_Data, l_Nb * sizeof(AnimationConcat_Z));
    if (GetMaxTime() <= 0.0f) {
        GetDebugName();
    }
}
