#ifndef _ANIMATIONNODE_Z_H_
#define _ANIMATIONNODE_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class BoneNode_Z;
class AnimationConcatNode_Z;
class RegMessage_Z;

struct AnimationNodeModifier_Z {
    Vec3f m_Translation;
    Vec3f m_Scale;
    Float m_BoneIndex;
};

struct AnimationNodeKeyId_Z {
    S16 m_CurTrans;
    S16 m_CurRot;
    S16 m_CurScale;
    AnimationNodeKeyId_Z();
    void Reset();
};

struct AnimationNodeData_Z {
    U16 m_Flag;
    KeyframerVec3fComp_Z m_TransKfr;
    KeyframerRot_Z m_RotKfr;
    KeyframerBezierRot_Z m_BezierRotKfr;
    KeyframerVec3fComp_Z m_ScaleKfr;
    KeyframerMessage_Z m_MsgKfr;
};

struct AnimationCctNodeFrame_Z {
    AnimationNodeModifier_Z* m_Modifier;
    AnimationNodeKeyId_Z* m_KeyId;
    BoneNode_Z* m_BoneNode;
    BoneNode_Z* m_UpdateBoneNode; // Points to same bone as m_BoneNode
    BoneNode_Z* m_OriginalBoneNode;
    RegMessage_Z* m_RegMsg;
    AnimationConcatNode_Z* m_Cct;
    AnimationNodeData_Z* m_Data;
};

class AnimationNode_Z {
private:
    Name_Z m_ObjectName;
    int m_ObjectId;
    AnimationNodeData_Z m_Data;

public:
    static void UpdateCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& i_RegMsgOffset);  // i_Times is Start, Cur, Max times
    static void GetCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& i_RegMsgOffset);     // i_Times is Start, Cur, Max times
    static void CctMessage(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& i_RegMsgOffset); // i_Times is Start, Cur, Max times
    static void Load(AnimationNodeData_Z& i_AnimData, void** i_Data);
};
#endif // _ANIMATIONNODE_Z_H_
