#ifndef _ABC_BEHAVIOR_H_
#define _ABC_BEHAVIOR_H_
#include "Types_Z.h"
#include "Memory_Z.h"
#include "ABC_Category_Engine.h"
class ABC_Behavior;
class ABC_Agent;
struct ABC_CategoryData;

typedef int behavior_result;

#define behavior_condition_state(s) (behavior_fsm_states + (s) * 2)
#define behavior_action_state(s) (behavior_fsm_states + (s) * 2 + 1)

#define behavior_fsm_state(c, s) ((c) + (s) * 2)

enum {
    result_FALSE = FALSE,
    result_TRUE = TRUE,
    result_MISSING_PART,
    result_OK,
    result_ERROR
};

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

typedef behavior_result (ABC_Agent::*behaviorMethodPtr)(behavior_state_type i_State, ABC_Behavior* i_Bhv);

struct ABC_CategoryData {
    ABC_CategoryData() {
        m_Category = cat_no_category;
        m_Next = NULL;
        m_IsPublic = FALSE;
    }

    Bool HasCategory(abc_category i_Cat);
    Bool HasPublicCategory(abc_category i_Cat);

    ABC_CategoryData* m_Next;
    abc_category m_Category;
    Bool m_IsPublic;
};

struct ABC_BehaviorHolder {
    ABC_BehaviorHolder* m_Next;
    behaviorMethodPtr m_Method;

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

struct ABC_CategoryHolder {
    abc_category m_Category;
    ABC_BehaviorHolder* m_PublicBehaviors;
    ABC_BehaviorHolder* m_PrivateBehaviors;
    ABC_CategoryHolder* m_Next;

    ABC_CategoryHolder() {
        m_Category = cat_no_category;
        m_PublicBehaviors = NULL;
        m_PrivateBehaviors = NULL;
        m_Next = NULL;
    }

    ABC_CategoryHolder(abc_category i_Category, const Char* i_CategoryName = NULL);
    ABC_CategoryHolder& operator=(const ABC_CategoryHolder& i_Holder);
    ABC_CategoryHolder(const ABC_CategoryHolder& i_Holder);
    ~ABC_CategoryHolder();
};

#define BHV_STARTED (U16)(1 << 0)
#define BHV_WAS_EXECUTED_THIS_CYCLE (U16)(1 << 1)
#define BHV_IS_ACTIVE (U16)(1 << 2)

class ABC_Behavior {
public:
    ABC_Behavior();
    void Reset();
    ~ABC_Behavior();
    void SetLocalDataSize(S32 i_Size);
    void LoadLocalVariables(void* i_Data);
    void StoreLocalVariables(const void* i_Data);
    void AddCategory(abc_category i_Category, Bool i_IsPublic, const Char* i_CategoryName = NULL);
    void AddPublicCategory(abc_category i_Category, const Char* i_CategoryName = NULL);
    void AddPrivateCategory(abc_category i_Category, const Char* i_CategoryName = NULL);
    void Execute(ABC_Agent* i_Agent);

    Bool HasFlag(U16 i_Flag) const { return (m_BehaviorFlags & i_Flag) ? TRUE : FALSE; }

    void SetFlag(U16 i_Flag) { m_BehaviorFlags |= i_Flag; }

    U16 GetFlag() const { return m_BehaviorFlags; }

    void ResetFlag(U16 i_Flag) { m_BehaviorFlags &= (U16)(~i_Flag); }

    int GetFSMState(void) const { return m_BehaviorState; }

    void SetFSMState(int i_State) { m_BehaviorState = (S16)i_State; }

    ABC_Behavior* m_Next;
    ABC_CategoryData* m_CategoryListHead;
    behaviorMethodPtr m_Method;

private:
    void* m_LocalData;
    S32 m_LocalDataSize;
    U16 m_BehaviorFlags;
    S16 m_BehaviorState;
};

#endif
