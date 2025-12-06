#ifndef _ANIMATIONNODE_Z_H_
#define _ANIMATIONNODE_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"
#include "AnimMessage_Z.h"

#define ANIM_NODE_RESET_TRANS 1
#define ANIM_NODE_RESET_ROT 2
#define ANIM_NODE_RESET_SCALE 4

class BoneNode_Z;
struct BoneTRS_Z;
class AnimationConcatNode_Z;
class RegMessage_Z;

struct AnimationNodeModifier_Z {
    Vec3f m_Scale;
    Vec3f m_Translation;
    Float m_BoneIndex;
};

struct AnimationNodeKeyId_Z {
    AnimationNodeKeyId_Z() {
        Reset();
    }

    inline void Reset() {
        m_CurTrans = 1;
        m_CurRot = 1;
        m_CurScale = 1;
    }

    S16 m_CurTrans;
    S16 m_CurRot;
    S16 m_CurScale;
};

struct AnimationNodeData_Z {
    U16 m_Flag;
    KeyframerVec3fComp_Z m_TransKfr;
    KeyframerRot_Z m_RotKfr;
    KeyframerBezierRot_Z m_BezierRotKfr;
    KeyframerVec3fComp_Z m_ScaleKfr;
    KeyframerMessage_Z m_MsgKfr;

    AnimationNodeData_Z() {
        m_Flag = 0;
    }

    inline void Flush() {
        m_Flag = 0;
        m_TransKfr.Flush();
        m_RotKfr.Flush();
        m_ScaleKfr.Flush();
        m_MsgKfr.Flush();
        m_BezierRotKfr.Flush();
    }
};

struct AnimationCctNodeFrame_Z {
    AnimationNodeModifier_Z* m_Modifier;
    AnimationNodeKeyId_Z* m_KeyId;
    BoneNode_Z* m_BoneNode;
    BoneTRS_Z* m_BoneTrs;
    BoneNode_Z* m_OriginalBoneNode;
    RegMessage_Z* m_RegMsg;
    const AnimationConcatNode_Z* m_Cct;
    const AnimationNodeData_Z* m_Data;
};

class AnimationNode_Z {
private:
    Name_Z m_ObjectName;
    int m_ObjectId;
    AnimationNodeData_Z m_Data;

public:
    static void UpdateCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_MsgNb);  // i_Times is Start, Cur, Max times
    static void GetCct(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_MsgNb);     // i_Times is Start, Cur, Max times
    static void CctMessage(const Vec3f& i_Times, AnimationCctNodeFrame_Z& i_Frame, S32& io_MsgNb); // i_Times is Start, Cur, Max times
    static void Load(AnimationNodeData_Z& i_AnimData, void** i_Data);

    inline static void EndLoad(AnimationNodeData_Z& i_AnimData) {
        i_AnimData.m_MsgKfr.EndLoad();
    }
};

typedef DynArray_Z<AnimationNodeModifier_Z, 1, 0, 0, 4> AnimationNodeModifier_ZDA;

#endif // _ANIMATIONNODE_Z_H_
