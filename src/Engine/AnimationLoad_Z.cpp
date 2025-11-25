#include "Animation_Z.h"
#include "Sys_Z.h"

void Animation_Z::Load(void** i_Data) {
    LOAD_Z(m_MaxTime);
    LOAD_Z(m_SmoothRatio);
    LOAD_Z(m_Flag);

    AnimationNode_Z::Load(m_NodeKfr, i_Data);
    AnimationMaterial_Z::Load(m_MtlKfr, i_Data);
    AnimationMesh_Z::Load(m_MeshKfr, i_Data);
    AnimationMorph_Z::Load(m_MorphKfr, i_Data);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_NodeCct);
    LOADDYNARRAY_Z(m_MtlCct);
    LOADDYNARRAY_Z(m_MeshCct);
    LOADDYNARRAY_Z(m_MorphCct);
    if (GetMaxTime() <= 0.0f) {
        GetDebugName();
    }
}
