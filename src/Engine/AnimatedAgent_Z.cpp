#include "AnimatedAgent_Z.h"
#include "ABC_Message_Engine.h"

BEGIN_INIT_AGENT_CLASS(AnimatedAgent_Z, AnimatedMsgAgent_Z)
END_INIT_AGENT_CLASS

AnimatedAgent_Z::AnimatedAgent_Z() {
    EnableFlag(FL_AGENT_ANIMATED | FL_AGENT_ANIMATED_SEQ | FL_AGENT_UNK_0x400 | FL_AGENT_NEED_SOUND);
    m_ClassAnimationIndex = -1;
}

void AnimatedAgent_Z::Init() {
}

void AnimatedAgent_Z::Reset() { }

void AnimatedAgent_Z::UseNode(Node_ZHdl&) { }

void AnimatedAgent_Z::RegisterWithGame(Game_ZHdl&) { }

void AnimatedAgent_Z::UnRegisterFromGame(Game_ZHdl&) { }

Bool AnimatedAgent_Z::MarkHandles() {
    return TRUE;
}

Bool AnimatedAgent_Z::GetCollisionMessageAction(abc_message& o_Msg) { }

void AnimatedAgent_Z::SetCollisionMessageAction(abc_message i_Msg) { }

Animation_ZHdl AnimatedAgent_Z::GetAnimation(anim_id i_AnimId) {
    return Animation_ZHdl();
}

void AnimatedAgent_Z::Suspend() { }

void AnimatedAgent_Z::Restore() { }
