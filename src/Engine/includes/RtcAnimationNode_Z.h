#ifndef _RTCANIMATIONNODE_Z_H_
#define _RTCANIMATIONNODE_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"
#include "AnimMessage_Z.h"

class Node_Z;

class RtcAnimationNode_Z {
private:
    Name_Z m_ObjectName; // Node name
    S16 m_ObjectId;      // Node index
    KeyframerExtVec3f_Z m_TransKfr;
    KeyframerRot_Z m_RotKfr;
    KeyframerExtVec3f_Z m_ScaleKfr;
    KeyframerMessage_Z m_MsgKfr;

public:
    RtcAnimationNode_Z();
    void MarkHandles();
    void UpdateFrame(Float i_StartTime, Float i_CurTime, Float i_MaxTime, Node_Z*, RegMessage_Z* o_Value, S32& io_MsgNb);
    void Load(void** i_Data);
    void EndLoad();
};

#endif // _RTCANIMATIONNODE_Z_H_
