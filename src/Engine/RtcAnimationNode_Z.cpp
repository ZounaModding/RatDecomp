#include "RtcAnimationNode_Z.h"
#include "Node_Z.h"
#include "Program_Z.h"

RtcAnimationNode_Z::RtcAnimationNode_Z() {
    m_ObjectId = -1;
    m_ScaleKfr.EnableFlag(FL_KEYFRAMER_UNK_0x10);
}

void RtcAnimationNode_Z::MarkHandles() {
    m_MsgKfr.MarkHandles();
}

void RtcAnimationNode_Z::UpdateFrame(Float i_StartTime, Float i_CurTime, Float i_MaxTime, Node_Z* i_Node, RegMessage_Z* o_Value, S32& io_MsgNb) {
    Vec3f l_VecValue;

    // Rotation
    if (m_RotKfr.GetNbKeys()) {
        Quat l_QuatValue;
        m_RotKfr.GetValue(i_CurTime, l_QuatValue);
        i_Node->SetRotation(l_QuatValue);
    }

    // Scale
    if (m_ScaleKfr.GetNbKeys()) {
        m_ScaleKfr.GetValue(i_CurTime, l_VecValue);
        i_Node->SetScale(l_VecValue.x);
        m_ScaleKfr.GetValue(i_CurTime, l_VecValue);
    }

    // Translation
    if (m_TransKfr.GetNbKeys()) {
        m_TransKfr.GetValue(i_CurTime, l_VecValue);
        i_Node->SetTranslation(l_VecValue);
    }

    // Message
    if (m_MsgKfr.GetNbKeys()) {
        m_MsgKfr.GetValue(i_StartTime, i_CurTime, i_MaxTime, o_Value, io_MsgNb);
    }

    i_Node->Changed();
}

void RtcAnimationNode_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadName(m_ObjectName, i_Data);
    LOAD_Z(m_ObjectId);
    m_RotKfr.Load(i_Data);
    m_ScaleKfr.Load(i_Data);
    m_TransKfr.Load(i_Data);
    m_MsgKfr.Load(i_Data);
}

void RtcAnimationNode_Z::EndLoad() {
    m_MsgKfr.EndLoad();
}
