#include "ABC_Agent.h"
#include "Program_Z.h"
#include "ABC_ScriptManager.h"

ABC_Behavior* ABC_Agent::currentBehavior = NULL;
S32 behaviorLocalVariablesSize;
void* behaviorLocalData;
S32 sequenceCurrentNesting;

ABC_Agent::ABC_Agent() {
    m_State = agent_neverActivated;
    m_Next = NULL;
    m_Prev = NULL;
    m_Sender = 0;
    m_MessageListHead = NULL;
    m_MessageListTail = NULL;
    m_BufferMessageListHead = NULL;
    m_BufferMessageListTail = NULL;
    m_BehaviorListHead = NULL;
    m_Flags = NULL;
}

ABC_Agent::~ABC_Agent() {
    if (gData.ScriptMgr) {
        ReleaseMessageList();
        SwapMessageList();
        ReleaseMessageList();
        ReleaseBehaviors();
        ABC_AgentHdl& l_Hdl = (ABC_AgentHdl&)GetHandle();
        gData.ScriptMgr->RemoveAgent(l_Hdl);
    }
}

ABC_Behavior* ABC_Agent::GetBehavior(behaviorMethodPtr i_BhvMethod) {
    ABC_Behavior* l_CurBhv = m_BehaviorListHead;
    while (l_CurBhv) {
        if (i_BhvMethod == l_CurBhv->m_Method) {
            return l_CurBhv;
        }
        else {
            l_CurBhv = l_CurBhv->m_Next;
        }
    }
    return NULL;
}
