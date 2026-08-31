#include "ABC_ScriptManager.h"

void ABC_ScriptManager::MinimizeReserve() {
    m_LinkedMessageReserve.Minimize();
    m_LinkedBehaviorReserve.Minimize();
    m_LinkedBehaviorHolderReserve.Minimize();
    m_LinkedCategoryHolderReserve.Minimize();
    m_LinkedCategoryDataReserve.Minimize();
}

ABC_Message* ABC_ScriptManager::GetNewMessage() {
    ABC_Message* l_Result = m_LinkedMessageReserve.Get();
    if (l_Result) {
        l_Result->Reset();
    }
    return l_Result;
}

void ABC_ScriptManager::ReleaseMessage(ABC_Message* i_Msg) {
    i_Msg->m_Sender = HANDLE_NULL;
    m_LinkedMessageReserve.Release(i_Msg);
}

// TODO: Finish matching
void ABC_ScriptManager::ReleaseMessages(ABC_Message* i_Msg) {
    if (!i_Msg) {
        return;
    }
    m_LinkedMessageReserve.ReleaseAll(i_Msg);
}

ABC_Behavior* ABC_ScriptManager::GetNewBehavior() {
    ABC_Behavior* l_Result = m_LinkedBehaviorReserve.Get();
    l_Result->Reset();
    return l_Result;
}

// TODO: Finish matching
void ABC_ScriptManager::ReleaseBehaviors(ABC_Behavior* i_Behavior) {
    ABC_Behavior* l_Bhv = i_Behavior;
    ABC_Behavior* l_NextBhv;
    while (l_Bhv) {
        l_NextBhv = l_Bhv->m_Next;
        ReleaseCategories(l_Bhv->m_CategoryListHead);
        l_Bhv->m_CategoryListHead = NULL;
        l_Bhv = l_NextBhv;
    }
    m_LinkedBehaviorReserve.ReleaseAll(i_Behavior);
}

ABC_BehaviorHolder* ABC_ScriptManager::GetNewBehaviorHolder() {
    return m_LinkedBehaviorHolderReserve.Get();
}

ABC_CategoryHolder* ABC_ScriptManager::GetNewCategoryHolder() {
    return m_LinkedCategoryHolderReserve.Get();
}

// TODO: Finish matching
void ABC_ScriptManager::ReleaseCategoryHolders(ABC_CategoryHolder* i_CategoryHolder) {
    if (!i_CategoryHolder) {
        return;
    }

    ABC_CategoryHolder* l_CategoryHolder = i_CategoryHolder;
    while (l_CategoryHolder) {
        m_LinkedBehaviorHolderReserve.ReleaseAll(l_CategoryHolder->m_PublicBehaviors);
        m_LinkedBehaviorHolderReserve.ReleaseAll(l_CategoryHolder->m_PrivateBehaviors);
        l_CategoryHolder->m_PublicBehaviors = NULL;
        l_CategoryHolder->m_PrivateBehaviors = NULL;
        l_CategoryHolder = l_CategoryHolder->m_Next;
    }

    m_LinkedCategoryHolderReserve.ReleaseAll(i_CategoryHolder);
}

ABC_CategoryData* ABC_ScriptManager::GetNewCategory() {
    return m_LinkedCategoryDataReserve.Get();
}

// TODO: Finish matching
void ABC_ScriptManager::ReleaseCategories(ABC_CategoryData* i_CategoryData) {
    if (!i_CategoryData) {
        return;
    }
    m_LinkedCategoryDataReserve.ReleaseAll(i_CategoryData);
}
