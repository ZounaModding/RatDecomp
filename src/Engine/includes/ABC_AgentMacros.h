#ifndef ABC_AGENT_MACROS_H_
#define ABC_AGENT_MACROS_H_
#include "Types_Z.h"

#define BEGIN_AGENT_CLASS(ClassName, SuperClassName, NewObjLine)                             \
    class ClassName : public SuperClassName {                                                \
    private:                                                                                 \
        static ABC_CategoryHolder* categories;                                               \
        typedef SuperClassName SUPER;                                                        \
                                                                                             \
    public:                                                                                  \
        static void InitClass();                                                             \
        static void SuperInitClass() {                                                       \
            SuperClassName::InitClass();                                                     \
        }                                                                                    \
        static void CleanClass() {                                                           \
            if (categories) Delete_Z categories;                                             \
        }                                                                                    \
        static void RegisterClass_Z() {                                                      \
            REGISTER_CLASS(#ClassName, #SuperClassName, &ClassName::NewObject);              \
        }                                                                                    \
        static BaseObject_Z* NewObject() {                                                   \
            return NewL_Z(NewObjLine) ClassName;                                             \
        }                                                                                    \
        virtual ABC_CategoryHolder* GetCategories() {                                        \
            return categories;                                                               \
        }                                                                                    \
        virtual void SetCategories(ABC_CategoryHolder* CategoryHolder) {                     \
            categories = CategoryHolder;                                                     \
        }                                                                                    \
        virtual void ActivateCategory(abc_category Category) {                               \
            _ActivateCategory(Category, ClassName::GetCategories());                         \
            SuperClassName::ActivateCategory(Category);                                      \
        }                                                                                    \
        virtual abc_category GetCategory(const char* aCategoryName) {                        \
            abc_category Category = _GetCategory(aCategoryName, ClassName::GetCategories()); \
            if (cat_no_category != Category)                                                 \
                return Category;                                                             \
            else                                                                             \
                return SuperClassName::GetCategory(aCategoryName);                           \
        }

#define END_AGENT_CLASS \
    }                   \
    ;

#define INIT_AGENT_CLASS(ClassName) \
    ClassName::InitClass();         \
    ClassName::RegisterClass_Z();

#define BEGIN_INIT_AGENT_CLASS(ClassName, baseClassname) \
    ABC_CategoryHolder* ClassName::categories = NULL;    \
    void ClassName::InitClass() {                        \
        SuperInitClass();                                \
        ClassName Agent;                                 \
        typedef ClassName THIS_Z;

#define ADD_BEHAVIOR(BhvName)                                          \
    {                                                                  \
        behaviorMethodPtr Ptr = (behaviorMethodPtr) & THIS_Z::BhvName; \
        Agent.DeclareBehavior(Ptr);                                    \
    }

#define END_INIT_AGENT_CLASS }

#define ActivateBhv(BhvName) BhvName(behavior_activate)
#define DeactivateBhv(BhvName) BhvName(behavior_deactivate)
#define IsBhvActive(BhvName) BhvName(behavior_is_active)
#define IsBhvStarted(BhvName) BhvName(behavior_is_started)
#define RestartBhv(BhvName) BhvName(behavior_restart)
#define RestartCurrentBhv() Behavior->ResetFlag(BHV_STARTED)
#define DeactivateCurrentBhv() Behavior->ResetFlag(BHV_IS_ACTIVE)
#define ActivateOrRestartBhv(BhvName) \
    ActivateBhv(BhvName);             \
    RestartBhv(BhvName)

#define DECL_BHV(BhvName) \
    virtual behavior_result BhvName(behavior_state_type BehaviorState, ABC_Behavior* Behavior = NULL);

extern S32 behaviorLocalVariablesSize;
extern void* behaviorLocalData;
extern S32 sequenceCurrentNesting;

#define SETBHVNAME(Behavior, BhvName)

#define BEHAVIOR(ClassName, BhvName)                                                                       \
    behavior_result                                                                                        \
    ClassName::BhvName(behavior_state_type BehaviorState, ABC_Behavior* Behavior) {                        \
        behavior_result BehaviorReturnValue = result_TRUE;                                                 \
        switch ((U32)BehaviorState) {                                                                      \
            case behavior_activate: {                                                                      \
                _ActivateBehavior((behaviorMethodPtr) & ClassName::BhvName);                               \
                return result_TRUE;                                                                        \
            }                                                                                              \
            case behavior_deactivate: {                                                                    \
                _DeactivateBehavior((behaviorMethodPtr) & ClassName::BhvName);                             \
                return result_TRUE;                                                                        \
            }                                                                                              \
            case behavior_restart: {                                                                       \
                _RestartBehavior((behaviorMethodPtr) & ClassName::BhvName);                                \
                return result_TRUE;                                                                        \
            }                                                                                              \
            case behavior_is_active: {                                                                     \
                if (Behavior)                                                                              \
                    return Behavior->HasFlag(BHV_IS_ACTIVE);                                               \
                else {                                                                                     \
                    ABC_Behavior* AnotherBehavior = GetBehavior((behaviorMethodPtr) & ClassName::BhvName); \
                    if (AnotherBehavior)                                                                   \
                        return AnotherBehavior->HasFlag(BHV_IS_ACTIVE);                                    \
                    else                                                                                   \
                        return result_FALSE;                                                               \
                }                                                                                          \
            }                                                                                              \
            case behavior_is_started: {                                                                    \
                if (Behavior)                                                                              \
                    return Behavior->HasFlag(BHV_STARTED);                                                 \
                else {                                                                                     \
                    ABC_Behavior* AnotherBehavior = GetBehavior((behaviorMethodPtr) & ClassName::BhvName); \
                    if (AnotherBehavior)                                                                   \
                        return AnotherBehavior->HasFlag(BHV_STARTED);                                      \
                    else                                                                                   \
                        return result_FALSE;                                                               \
                }                                                                                          \
            }                                                                                              \
            case behavior_get_state: {                                                                     \
                if (Behavior)                                                                              \
                    return Behavior->GetFSMState();                                                        \
                else                                                                                       \
                    return -9999;                                                                          \
            }                                                                                              \
        }                                                                                                  \
        behaviorLocalVariablesSize = 0;                                                                    \
        behaviorLocalData = NULL;                                                                          \
        sequenceCurrentNesting = -1;                                                                       \
        if (behavior_declaration == BehaviorState || behavior_init == BehaviorState) {

#define CATEGORY(Category) Behavior->AddPublicCategory(Category, #Category);

#define CATEGORY_PRIVATE(Category) Behavior->AddPrivateCategory(Category, #Category);

// Head of behavior body
#define BEGIN_BEHAVIOR                                                                                                                         \
    }                                                                                                                                          \
    enum { STATE_DEFAULT = 0,                                                                                                                  \
           STATE_FINISH = -1 };                                                                                                                \
    if ((BehaviorState == behavior_finish_condition) && (Behavior->GetFSMState() == STATE_FINISH)) return result_TRUE;                         \
    Behavior->LoadLocalVariables(behaviorLocalData);                                                                                           \
    int LocalBhvState = ((BehaviorState >= behavior_fsm_states) ? behavior_fsm_state(BehaviorState, Behavior->GetFSMState()) : BehaviorState); \
    switch (((U32)LocalBhvState)) {                                                                                                            \
        case behavior_init:                                                                                                                    \
            Behavior->SetLocalDataSize(behaviorLocalVariablesSize);                                                                            \
        case behavior_declaration: {
#define BEGIN_BEHAVIOR_WITHUPDATE BEGIN_BEHAVIOR

// Start condition
#define START_COND                   \
    ;                                \
    break;                           \
    }                                \
    case behavior_start_condition: { \
        BehaviorReturnValue =

// Start action
#define START_ACTION \
    ;                \
    break;           \
    }                \
    case behavior_start_action: {

// Finish condition
#define FINISH_COND                   \
    ;                                 \
    break;                            \
    }                                 \
    case behavior_finish_condition: { \
        BehaviorReturnValue =

// Finish action
#define FINISH_ACTION \
    ;                 \
    break;            \
    }                 \
    case behavior_finish_action: {

// Condition
#define CONDITION              \
    ;                          \
    break;                     \
    }                          \
    case behavior_condition: { \
        BehaviorReturnValue =

// Action
#define ACTION \
    ;          \
    break;     \
    }          \
    case behavior_action: {
// Alternative
#define ALTERNATIVE \
    ;               \
    break;          \
    }               \
    case behavior_alternative: {
#define END_BEHAVIOR                                      \
    ;                                                     \
    break;                                                \
    }                                                     \
    default:                                              \
        BehaviorReturnValue = result_MISSING_PART;        \
        break;                                            \
        }                                                 \
        if (BehaviorState == behavior_is_started) {       \
        }                                                 \
        Behavior->StoreLocalVariables(behaviorLocalData); \
        return BehaviorReturnValue;                       \
        }

#endif //ABC_AGENT_MACROS_H
