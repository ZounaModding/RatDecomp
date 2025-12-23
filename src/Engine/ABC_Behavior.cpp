#include "ABC_Behavior.h"
#include "Memory_Z.h"
#include "Program_Z.h"
#include "ABC_ScriptManager.h"
Extern_Z "C" void memcpy(void* dst, const void* src, int n);

ABC_Behavior::ABC_Behavior() {
    m_LocalDataSize = 0;
    m_LocalData = NULL;
    Reset();
}

void ABC_Behavior::Reset() {
    m_BehaviorFlags &= 0xFFF8;
    m_BehaviorState = 0;
    if (m_LocalDataSize != 0) {
        Delete_Z m_LocalData;
    }
    m_LocalData = NULL;
    m_LocalDataSize = 0;
    m_Next = NULL;
    m_CategoryListHead = NULL;
}

ABC_Behavior::~ABC_Behavior() {
    if (m_LocalDataSize != 0) {
        Delete_Z m_LocalData;
    }
}

void ABC_Behavior::SetLocalDataSize(S32 i_Size) {
    if (i_Size) {
        m_LocalDataSize = i_Size;
        //TODO: WRITE MACRO FOR THIS
        m_LocalData = operator new[](m_LocalDataSize, "Anonymous New", __FILE__, 40);
    }
}

void ABC_Behavior::LoadLocalVariables(void* i_Data) {
    if (!i_Data)
        return;
    if (m_LocalDataSize)
        memcpy(i_Data, m_LocalData, m_LocalDataSize);
}

void ABC_Behavior::StoreLocalVariables(const void* i_Data) {
    if (!i_Data)
        return;
    if (m_LocalDataSize)
        memcpy(m_LocalData, i_Data, m_LocalDataSize);
}

void ABC_Behavior::AddCategory(abc_category i_Category, Bool i_IsPublic, const char* i_CategoryName) {
    ABC_CategoryData* l_Category = m_CategoryListHead;
    while (l_Category) {
        if (i_Category == l_Category->m_Category) {
            l_Category->m_IsPublic = i_IsPublic;
            return;
        }
        l_Category = l_Category->m_Next;
    }
    l_Category = gData.ScriptMgr->GetNewCategory();
    l_Category->m_Category = i_Category;
    l_Category->m_IsPublic = i_IsPublic;
    l_Category->m_Next = m_CategoryListHead;
    m_CategoryListHead = l_Category;
}

void ABC_Behavior::AddPublicCategory(abc_category i_Category, const char* i_CategoryName) {
    AddCategory(i_Category, TRUE, i_CategoryName);
}

void ABC_Behavior::AddPrivateCategory(abc_category i_Category, const char* i_CategoryName) {
    AddCategory(i_Category, FALSE, i_CategoryName);
}

void ABC_Behavior::Execute(ABC_Agent* i_Agent) {
    SetFlag(BHV_WAS_EXECUTED_THIS_CYCLE);

    if (!HasFlag(BHV_STARTED)) {
        if ((i_Agent->*m_Method)(behavior_start_condition, this)) {
            SetFlag(BHV_STARTED);
            SetFSMState(0);
            (i_Agent->*m_Method)(behavior_start_action, this);
        }
        else {
            return;
        }
    }

    behavior_result l_Result = (i_Agent->*m_Method)(behavior_finish_condition, this);
    if (l_Result == result_TRUE) {
        ResetFlag(BHV_STARTED);
        (i_Agent->*m_Method)(behavior_finish_action, this);
        return;
    }

    if ((i_Agent->*m_Method)(behavior_condition, this)) {
        (i_Agent->*m_Method)(behavior_action, this);
    }
    else {
        (i_Agent->*m_Method)(behavior_alternative, this);
    }
}
