#ifndef _ANIMATEDAGENT_Z_H_
#define _ANIMATEDAGENT_Z_H_
#include "AnimatedMsgAgent_Z.h"
#include "PlayAnim_ZHdl.h"
#include "SequenceAnim_ZHdl.h"
#include "Animation_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(AnimatedAgent_Z, AnimatedMsgAgent_Z, 15)
public:
    AnimatedAgent_Z();
    virtual void Init();
    virtual ~AnimatedAgent_Z() { }
    virtual Bool MarkHandles();
    virtual void Suspend();
    virtual void Restore();
    virtual void Reset();
    virtual void UseNode(Node_ZHdl& a1);
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);
    virtual Bool GetCollisionMessageAction(abc_message& o_Msg);
    virtual void SetCollisionMessageAction(abc_message i_Msg);
    virtual void Drop() {}

    PlayAnim_Z* GetPlayAnim() { return m_PlayAnimHdl; }

    SequenceAnim_ZHdl& GetSequenceAnim() { return m_SequenceAnimHdl; }

    const PlayAnim_ZHdl& GetPlayAnim() const { return m_PlayAnimHdl; }

    const SequenceAnim_ZHdl& GetSequenceAnim() const { return m_SequenceAnimHdl; }

    Animation_ZHdl GetAnimation(anim_id i_AnimId = anim_default);

private:
    SequenceAnim_ZHdl m_SequenceAnimHdl;
    PlayAnim_ZHdl m_PlayAnimHdl;
    int m_ClassAnimationIndex;
    abc_message m_CollisionMessageAction;
END_AGENT_CLASS

// clang-format on
#endif // _ANIMATEDAGENT_Z_H_
