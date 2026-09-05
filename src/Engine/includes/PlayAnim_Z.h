#ifndef _PLAYANIM_Z_H_
#define _PLAYANIM_Z_H_
#include "Manipulator_Z.h"
#include "Animation_ZHdl.h"
#include "Animation_Z.h"
#include "AnimationNode_Z.h"
#include "DynamicAnim2_ZHdl.h"
#include "Math_Z.h"
#include "Node_ZHdl.h"
#include "SequenceAnim_ZHdl.h"
#include "Skel_ZHdl.h"

class SequenceAnim_ZHdl;

#define MAX_MIX_ANIM 3

#define FL_MIX_ANIM_NONE (U16)(0 << 0)
#define FL_MIX_ANIM_UNK_0x1 (U16)(1 << 0)
#define FL_MIX_ANIM_UNK_0x2 (U16)(1 << 1)
#define FL_MIX_ANIM_UNK_0x4 (U16)(1 << 2)
#define FL_MIX_ANIM_UNK_0x8 (U16)(1 << 3)
#define FL_MIX_ANIM_UNK_0x10 (U16)(1 << 4)
#define FL_MIX_ANIM_RESETED (U16)(1 << 5)
#define FL_MIX_ANIM_UNK_0x40 (U16)(1 << 6)
#define FL_MIX_ANIM_OVERWRITE (U16)(1 << 7)

struct MixAnim_Z {
    Animation_ZHdl m_AnimHdl;
    Float m_CurTime;
    Float m_NoSmoothSpeedMult;
    Float m_LoopStartTime;
    Float m_SmoothTime;
    Float m_StartTime;
    Float m_SmoothSpeedMult;
    U16 m_LoopCountdown;
    U16 m_Flag;
    Float m_EndTime;
    AnimationConcatNode_ZDA m_SmoothNodeCcts;
    AnimationNodeData_Z m_SmoothNodeData;
    AnimationKeyId_Z m_AnimKeyId;
};

class PlayAnim_Z : public Manipulator_Z {
public:
    virtual void Init();

    virtual ~PlayAnim_Z() { }

    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();
    virtual void Update(Float i_DeltaTime);

    virtual void SetSequenceManager(const SequenceAnim_ZHdl& a1) { }

    virtual void AddConicForce(const Vec3f& a1, const Vec3f& a2, Float a3, Float a4, Float a5, Float a6, Float a7);
    virtual void AddSphericForce(const Sphere_Z& a1, Float a2, Float a3);

    static BaseObject_Z* NewObject() {
        return NewL_Z(152) PlayAnim_Z;
    }

    void ResetDynamic();
    Bool SetNode(const Node_ZHdl& i_NodeHdl);
    void SetAnim(const Animation_ZHdl& i_AnimHdl, S32 i_Priority = 0, Bool i_Smooth = TRUE, Float i_CurTime = 0.0f);

    inline const Skel_ZHdl& GetSkel() const {
        return m_SkelHdl;
    }

    inline const Node_ZHdl& GetNode() const {
        return m_NodeHdl;
    }

    inline void SetStartTime(Float i_StartTime, S32 i_Index = 0) {
        m_MixAnims[i_Index].m_StartTime = i_StartTime;
    }

    inline void SetSmoothTime(Float i_SmoothTime, S32 i_Index = 0) {
        m_MixAnims[i_Index].m_SmoothTime = i_SmoothTime;
    }

    inline void SetSpeed(Float i_Speed, S32 i_Index = 0) {
        m_MixAnims[i_Index].m_NoSmoothSpeedMult = i_Speed;
        m_MixAnims[i_Index].m_SmoothSpeedMult = i_Speed;
    }

protected:
    Float m_UpdateTimer;
    Float m_UpdateRate;
    MixAnim_Z m_MixAnims[MAX_MIX_ANIM];
    Skel_ZHdl m_SkelHdl;
    Node_ZHdl m_NodeHdl;
    DynamicAnim2_ZHdl m_DynamicAnimHdl;
    AnimationNodeModifier_ZDA m_NodeModifiers;
    SequenceAnim_ZHdl m_SequenceManagerHdl;
};

#endif // _PLAYANIM_Z_H_
