#include "SkinAgent_Z.h"
#include "ABC_ScriptManager.h"
#include "PlayAnim_ZHdl.h"
#include "PlayAnim_Z.h"
#include "Animation_Z.h"

BEGIN_INIT_AGENT_CLASS(SkinAgent_Z, AnimatedAgent_Z)
ADD_BEHAVIOR(Act)
END_INIT_AGENT_CLASS

BEHAVIOR(SkinAgent_Z, Act)
CATEGORY(cat_default)
BEGIN_BEHAVIOR
FINISH_COND
Message(msg_skin_act_end) && !Message(msg_skin_act_begin) FINISH_ACTION GetPlayAnim()->SetAnim(GetAnimation(anim_default));
CONDITION
Message(msg_skin_act_begin)
    ACTION
    gData.ScriptMgr->FlushTimedMessagesTo(this, msg_skin_act_end);
Animation_ZHdl l_Anim = GetAnimation(anim_action);
GetPlayAnim()->SetAnim(l_Anim);
this->SendTimed(msg_skin_act_end, this, l_Anim->GetMaxTime());
END_BEHAVIOR

SkinAgent_Z::SkinAgent_Z() {
    EnableFlag(FL_AGENT_UNK_0x20000000);
    DisableFlag(FL_AGENT_UNK_0x200);
}
