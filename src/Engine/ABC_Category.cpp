#include "ABC_Behavior.h"

Bool ABC_CategoryData::HasCategory(abc_category i_Cat) {
    ABC_CategoryData* m_Curr = this;
    while (m_Curr != NULL) {

        if (i_Cat == m_Curr->m_Category) return TRUE;
        m_Curr = m_Curr->m_Next;
    }
    return FALSE;
}

Bool ABC_CategoryData::HasPublicCategory(abc_category i_Cat) {
    ABC_CategoryData* m_Curr = this;
    while (m_Curr != NULL) {
        if (i_Cat == m_Curr->m_Category) return m_Curr->m_IsPublic;
        m_Curr = m_Curr->m_Next;
    }
    return FALSE;
}
