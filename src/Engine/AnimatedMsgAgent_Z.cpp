#include "AnimatedMsgAgent_Z.h"
#include "AnimMessage_Z.h"
#include "AnimationManager_Z.h"

BEGIN_INIT_AGENT_CLASS(AnimatedMsgAgent_Z, Agent_Z)
END_INIT_AGENT_CLASS

RegMessage_Z& AnimatedMsgAgent_Z::GetMessage() {
    S32 l_MaxRegMsg = gData.AnimMgr->GetMaxRegMessage();
    int l_MsgParam = (int)MsgParam();
    return gData.AnimMgr->GetRegMessage(l_MsgParam & l_MaxRegMsg - 1);
}
