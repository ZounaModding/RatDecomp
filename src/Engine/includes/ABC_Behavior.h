#ifndef _ABC_BEHAVIOR_H_
#define _ABC_BEHAVIOR_H_
#include "Types_Z.h"
#include "Memory_Z.h"
#include "ABC_Category_Engine.h"
class ABC_Behavior;
class ABC_Agent;
class ABC_CategoryData;

typedef int behavior_result;

enum behavior_state_type {
    behavior_activate,
    behavior_deactivate,
    behavior_restart,
    behavior_is_active,
    behavior_is_started,
    behavior_get_name,
    behavior_get_state,
    behavior_init,
    behavior_declaration,
    behavior_start_condition,
    behavior_start_action,
    behavior_finish_condition,
    behavior_finish_action,
    behavior_alternative,

    behavior_condition,
    behavior_action,

    behavior_fsm_states = behavior_condition
};

typedef behavior_result (ABC_Agent::*behaviorMethodPtr)(behavior_state_type behavior_state, ABC_Behavior* aBehavior);

class ABC_CategoryData {
    ABC_CategoryData() {
        m_Category = -1;
        m_Next = NULL;
        m_IsPublic = FALSE;
    }

    Bool HasCategory(abc_category i_Cat);
    Bool HasPublicCategory(abc_category i_Cat);

private:
    ABC_CategoryData* m_Next;
    S32 m_Category;
    Bool m_IsPublic;
};

class ABC_BehaviorHolder {
public:
    behaviorMethodPtr m_Method;
    ABC_BehaviorHolder* m_Next;

    ABC_BehaviorHolder() {
        m_Method = NULL;
        m_Next = NULL;
    }

    ABC_BehaviorHolder(behaviorMethodPtr i_Method) {
        m_Method = i_Method;
        m_Next = NULL;
    }

    ABC_BehaviorHolder& operator=(const ABC_BehaviorHolder& i_Holder) {
        m_Method = i_Holder.m_Method;
        m_Next = NULL;
        return *this;
    }

    ~ABC_BehaviorHolder() {
        if (m_Next) Delete_Z m_Next;
    }
};

class ABC_CategoryHolder {
public:
    abc_category m_Category;
    ABC_BehaviorHolder* m_PublicBehaviors;
    ABC_BehaviorHolder* m_PrivateBehaviors;
    ABC_CategoryHolder* m_Next;

    ABC_CategoryHolder();
    ABC_CategoryHolder(abc_category i_Category, const Char* i_CategoryName = NULL);
    ABC_CategoryHolder& operator=(const ABC_CategoryHolder& i_Holder);
    ABC_CategoryHolder(const ABC_CategoryHolder& i_Holder);
    ~ABC_CategoryHolder();
};

class ABC_Behavior {
public:
    ABC_Behavior();
    void Reset();
    ~ABC_Behavior();
    void SetLocalDataSize(S32 i_Size);
    void LoadLocalVariables(void* i_Data);
    void StoreLocalVariables(const void* i_Data);

private:
    ABC_Behavior* m_Next;
    ABC_CategoryData* m_CategoryListHead;
    behaviorMethodPtr m_Behavior;
    void* m_LocalData;
    S32 m_LocalDataSize;
    U16 m_BehaviorFlags;
    S16 m_BehaviorState;
};

#endif
