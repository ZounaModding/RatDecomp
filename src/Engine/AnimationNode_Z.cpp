#include "AnimationNode_Z.h"
#include "Sys_Z.h"

void AnimationNode_Z::Load(AnimationNodeData_Z& i_AnimData, void** i_Data) {
    MEMCPYFROM_Z(&i_AnimData.m_Flag, *i_Data, sizeof(i_AnimData.m_Flag));
    i_AnimData.m_RotKfr.Load(i_Data);
    i_AnimData.m_BezierRotKfr.Load(i_Data);
    i_AnimData.m_ScaleKfr.Load(i_Data);
    i_AnimData.m_TransKfr.Load(i_Data);
    i_AnimData.m_MsgKfr.Load(i_Data);
}
