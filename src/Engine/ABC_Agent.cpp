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

void ABC_Agent::_ActivateBehavior(behaviorMethodPtr i_BhvMethod, abc_category i_Category) {
    ABC_Behavior* l_Bhv = GetBehavior(i_BhvMethod);

    if (!l_Bhv) {
        void* l_SavedBehaviorLocalData;
        if (currentBehavior) {
            l_SavedBehaviorLocalData = behaviorLocalData;
            currentBehavior->StoreLocalVariables(behaviorLocalData);
        }

        l_Bhv = GetNewBehavior();
        l_Bhv->m_Method = i_BhvMethod;
        (this->*i_BhvMethod)(behavior_init, l_Bhv);
        (this->*i_BhvMethod)(behavior_get_name, l_Bhv);
        l_Bhv->m_Next = m_BehaviorListHead;
        m_BehaviorListHead = l_Bhv;

        if (currentBehavior) {
            behaviorLocalData = l_SavedBehaviorLocalData;
            currentBehavior->LoadLocalVariables(l_SavedBehaviorLocalData);
        }
    }

    if (i_Category != cat_no_category && (!l_Bhv->m_CategoryListHead || !l_Bhv->m_CategoryListHead->HasPublicCategory(i_Category)))
        return;

    if (!l_Bhv->HasFlag(BHV_IS_ACTIVE)) {
        l_Bhv->SetFlag(BHV_IS_ACTIVE);
        l_Bhv->ResetFlag(BHV_STARTED);
        m_ChangeInBehaviorList = TRUE;
    }
}

void ABC_Agent::_DeactivateBehavior(behaviorMethodPtr i_BhvMethod) {
    ABC_Behavior* l_Bhv = GetBehavior(i_BhvMethod);
    if (l_Bhv) {
        l_Bhv->ResetFlag(BHV_IS_ACTIVE);
    }
}

void ABC_Agent::_ActivateCategory(abc_category i_Category, ABC_CategoryHolder* i_CategoryHolder) {
    while (i_CategoryHolder) {
        if (i_Category == i_CategoryHolder->m_Category) {
            ABC_BehaviorHolder* l_BhvHolder = i_CategoryHolder->m_PublicBehaviors;
            while (l_BhvHolder) {
                _ActivateBehavior(l_BhvHolder->m_Method, i_Category);
                l_BhvHolder = l_BhvHolder->m_Next;
            }
            return;
        }
        i_CategoryHolder = i_CategoryHolder->m_Next;
    }
}

void ABC_Agent::DeactivateCategory(abc_category i_Category) {
    ABC_Behavior* l_Bhv = m_BehaviorListHead;
    while (l_Bhv) {
        if (l_Bhv->HasFlag(BHV_IS_ACTIVE) && l_Bhv->m_CategoryListHead->HasCategory(i_Category)) {
            l_Bhv->ResetFlag(BHV_IS_ACTIVE);
        }
        l_Bhv = l_Bhv->m_Next;
    }
}

void ABC_Agent::DeactivateAllBehaviors() {
    ABC_Behavior* l_Bhv = m_BehaviorListHead;

    while (l_Bhv) {
        l_Bhv->ResetFlag(BHV_IS_ACTIVE);
        l_Bhv = l_Bhv->m_Next;
    }
}

void ABC_Agent::ResetAllBehaviorsExceptCategories(CategoryArray& i_Categories) {
    ABC_Behavior* l_Bhv = m_BehaviorListHead;
    int l_Size = i_Categories.GetSize();

    if (l_Size == 0) {
        while (l_Bhv) {
            l_Bhv->ResetFlag(BHV_STARTED | BHV_WAS_EXECUTED_THIS_CYCLE | BHV_IS_ACTIVE);
            l_Bhv->SetFSMState(0);
            l_Bhv = l_Bhv->m_Next;
        }
        return;
    }

    while (l_Bhv) {
        Bool l_Reset = TRUE;
        for (S32 i = 0; i < l_Size; i++) {
            if (l_Bhv->m_CategoryListHead->HasCategory(i_Categories[i])) {
                l_Reset = FALSE;
                break;
            }
        }
        if (l_Reset) {
            l_Bhv->ResetFlag(BHV_STARTED | BHV_WAS_EXECUTED_THIS_CYCLE | BHV_IS_ACTIVE);
            l_Bhv->SetFSMState(0);
        }
        l_Bhv = l_Bhv->m_Next;
    }
}

void ABC_Agent::_RestartBehavior(behaviorMethodPtr i_BhvMethod) {
    ABC_Behavior* l_Bhv = GetBehavior(i_BhvMethod);
    if (l_Bhv) {
        l_Bhv->ResetFlag(BHV_STARTED);
    }
}

void ABC_Agent::Receive(abc_message i_Msg, const BaseObject_ZHdl& i_Sender, Float i_Param) {
    ABC_Message* l_Msg = GetNewMessage();
    if (l_Msg) {
        l_Msg->m_Id = i_Msg;
        l_Msg->m_Sender = i_Sender;
        l_Msg->m_Time = gData.ScriptMgr->GetTime();
        l_Msg->m_Param = i_Param;
        if (m_BufferMessageListTail) {
            m_BufferMessageListTail->m_Next = l_Msg;
        }
        else {
            m_BufferMessageListHead = l_Msg;
        }
        m_BufferMessageListTail = l_Msg;
        l_Msg->m_Next = NULL;
        Activate();
    }
}

void ABC_Agent::Receive(ABC_Message* i_Msg) {
    if (m_BufferMessageListTail) {
        m_BufferMessageListTail->m_Next = i_Msg;
    }
    else {
        m_BufferMessageListHead = i_Msg;
    }
    m_BufferMessageListTail = i_Msg;
    i_Msg->m_Next = NULL;
    Activate();
}

Bool ABC_Agent::Message(abc_message i_Msg) {
    ABC_Message* l_Msg = m_MessageListHead;
    while (l_Msg) {
        if (i_Msg == l_Msg->m_Id) {
            m_Sender = l_Msg->m_Sender;
            m_MessageParameter = l_Msg->m_Param;
            m_MessageID = l_Msg->m_Id;
            return TRUE;
        }
        else {
            l_Msg = l_Msg->m_Next;
        }
    }
    return FALSE;
}

void ABC_Agent::StartCycle() {
    ABC_Behavior* l_Bhv = m_BehaviorListHead;
    while (l_Bhv) {
        l_Bhv->ResetFlag(BHV_WAS_EXECUTED_THIS_CYCLE);
        l_Bhv = l_Bhv->m_Next;
    }

    SetState(agent_inactive);
    ReleaseMessageList();
    SwapMessageList();
    m_ChangeInBehaviorList = FALSE;
    currentBehavior = m_BehaviorListHead;
}

void ABC_Agent::FinishCycle() {
    currentBehavior = NULL;
    ReleaseMessageList();
}

void ABC_Agent::Execution() {
    StartCycle();

    do {
        if (m_ChangeInBehaviorList) {
            currentBehavior = m_BehaviorListHead;
        }
        while (currentBehavior) {
            if (currentBehavior->HasFlag(BHV_IS_ACTIVE) && !currentBehavior->HasFlag(BHV_WAS_EXECUTED_THIS_CYCLE)) {
                currentBehavior->Execute(this);
                break;
            }
            else {
                currentBehavior = currentBehavior->m_Next;
            }
        }
    } while (currentBehavior);

    FinishCycle();
}

void ABC_Agent::Activate() {
    if (m_State != agent_active) {
        SetState(agent_toBeActivated);
    }
}

void ABC_Agent::Deactivate() {
    if (m_State == agent_neverActivated) {
        return;
    }
    SetState(agent_inactive);
}

void ABC_Agent::SetState(const abc_agentState i_State) {
    gData.ScriptMgr->SetAgentState(this, i_State);
    m_State = i_State;
}

void ABC_Agent::AddBehaviorToCategory(behaviorMethodPtr i_BhvMethod, abc_category i_Category, Bool i_IsPublic, const Char* i_CategoryName) {
    ABC_CategoryHolder* l_CategoryHolder = GetCategories();
    while (l_CategoryHolder) {
        if (i_Category == l_CategoryHolder->m_Category) {
            break;
        }
        else {
            l_CategoryHolder = l_CategoryHolder->m_Next;
        }
    }
    if (!l_CategoryHolder) {
        l_CategoryHolder = GetNewCategoryHolder();
        l_CategoryHolder->m_Category = i_Category;
        l_CategoryHolder->m_Next = GetCategories();
        SetCategories(l_CategoryHolder);
    }

    ABC_BehaviorHolder* l_BehaviorHolder = GetNewBehaviorHolder();
    l_BehaviorHolder->m_Method = i_BhvMethod;
    if (i_IsPublic) {
        l_BehaviorHolder->m_Next = l_CategoryHolder->m_PublicBehaviors;
        l_CategoryHolder->m_PublicBehaviors = l_BehaviorHolder;
    }
    else {
        l_BehaviorHolder->m_Next = l_CategoryHolder->m_PrivateBehaviors;
        l_CategoryHolder->m_PrivateBehaviors = l_BehaviorHolder;
    }
}

// TODO: Finish matching (regswap)
void ABC_Agent::DeclareBehavior(behaviorMethodPtr i_BhvMethod) {
    ABC_Behavior l_Bhv;
    (this->*i_BhvMethod)(behavior_declaration, &l_Bhv);
    ABC_CategoryData* l_Category = l_Bhv.m_CategoryListHead;
    while (l_Category) {
        AddBehaviorToCategory(i_BhvMethod, l_Category->m_Category, l_Category->m_IsPublic, NULL);
        l_Category = l_Category->m_Next;
    }
}

abc_category ABC_Agent::_GetCategory(const Char* i_CategoryName, ABC_CategoryHolder* i_CategoryHolder) {
    return cat_no_category;
}
