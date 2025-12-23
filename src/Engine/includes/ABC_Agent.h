#ifndef _ABC_AGENT_H_
#define _ABC_AGENT_H_
#include "BaseObject_Z.h"
#include "Handle_Z.h"
#include "ABC_Category_Engine.h"
#include "ABC_Behavior.h"
#include "ABC_Message.h"
#include "ABC_AgentMacros.h"
#include "Agent_Flags_Engine.h"
#include "Anim_Symbol_Engine.h"

typedef DynArray_Z<abc_category, 4> CategoryArray;

enum abc_agentState {
    agent_neverActivated = 0,
    agent_inactive = 1,
    agent_active = 2,
    agent_toBeActivated = 3,
    agent_last
}; // Jimmy DWARF

/* sizeof(ABC_Agent) == 0x40 */
class ABC_Agent : public BaseObject_Z {
public:
    // clang-format off

    ABC_Agent();

    virtual ~ABC_Agent();
    static void InitClass() {}

    // Behavior methods

    ABC_BehaviorHolder* GetNewBehaviorHolder();
    ABC_Behavior* GetNewBehavior();
    ABC_Behavior* GetBehavior(behaviorMethodPtr i_BhvMethod);
    void DeclareBehavior(behaviorMethodPtr i_BhvMethod);
    void ReleaseBehaviors();
    void _ActivateBehavior(behaviorMethodPtr i_BhvMethod, abc_category i_Category = cat_no_category);
    void _DeactivateBehavior(behaviorMethodPtr i_BhvMethod);
    void _RestartBehavior(behaviorMethodPtr i_BhvMethod);
    void DeactivateAllBehaviors();
    void ResetAllBehaviorsExceptCategories(CategoryArray& i_Categories);

    // Category methods

    ABC_CategoryHolder* GetNewCategoryHolder();
    void AddBehaviorToCategory(behaviorMethodPtr i_BhvMethod, abc_category i_Category, Bool i_IsPublic, const Char* i_CategoryName);
    void ReleaseCategoryHolders(ABC_CategoryHolder* i_CategoryHolder);
    virtual ABC_CategoryHolder* GetCategories() { return NULL; }
    virtual void SetCategories(ABC_CategoryHolder* i_CategoryHolder) = 0;
    virtual void ActivateCategory(abc_category i_Category) { }
    virtual abc_category GetCategory(const Char* i_CategoryName) { return cat_no_category; }
    void _ActivateCategory(abc_category i_Category, ABC_CategoryHolder* i_CategoryHolder);
    abc_category _GetCategory(const Char* i_CategoryName, ABC_CategoryHolder* i_CategoryHolder);
    void DeactivateCategory(abc_category i_Category);

    // Message methods

    ABC_Message* GetNewMessage();
    void ReleaseMessageList();
    void SwapMessageList();
    inline Float MsgParam() const { return m_MessageParameter; }
    inline BaseObject_ZHdl MsgSender() { return m_Sender; }
    void Receive(abc_message i_Msg, const BaseObject_ZHdl& i_Sender, Float i_Param = UNDEFINED_FVALUE);
    void Receive(ABC_Message* i_Msg);
    Bool Message(abc_message i_Msg);
    Bool FlushMessage(abc_message i_Msg);
    Bool FlushEveryMessage(abc_message i_Msg, ABC_Agent* i_Sender = NULL);
    Bool FlushAllMessages();

    // Flag methods

    inline void CleanFlags() { m_Flags = 0; }
    inline void EnableFlag(U32 i_Flag) { m_Flags |= i_Flag; }
    inline void DisableFlag(U32 i_Flag) { m_Flags &= ~i_Flag; }
    inline Bool HasFlag(U32 i_Flag) const { return (m_Flags & i_Flag) ? TRUE : FALSE; }
    inline U32 GetFlags(void) const { return m_Flags; }

    // Execution methods

    void Execution();
    void StartCycle();
    void FinishCycle();
    ABC_Behavior* GetNextExecutableBehavior();

    // State methods

    void Activate();
    void Deactivate();
    inline void CleanMessage() { ReleaseMessageList(); }
    void SetState(const abc_agentState i_State);
    abc_agentState GetState() const { return m_State; }
    virtual void Suspend() { }
    virtual void Restore() { }

    // clang-format on
private:
    static ABC_Behavior* currentBehavior; // $SABE: We can't apply our naming conventions cause the symbol is named this way in the binary
    abc_agentState m_State;
    ABC_Agent* m_Next;
    ABC_Agent* m_Prev;
    Bool m_ChangeInBehaviorList;
    ABC_Behavior* m_BehaviorListHead;
    ABC_Message* m_MessageListHead;
    ABC_Message* m_MessageListTail;
    ABC_Message* m_BufferMessageListHead;
    ABC_Message* m_BufferMessageListTail;

protected:
    BaseObject_ZHdl m_Sender;
    Float m_MessageParameter;
    abc_message m_MessageID;
    U32 m_Flags;
};

class ABC_AgentList_Z {
    ABC_Agent* m_First;
    ABC_Agent* m_Last;
};

#endif
