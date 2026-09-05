#include "SurfaceCache_Z.h"

U16 ColSurfaceCache_Z::GetFreeEntry(Surface_Z* i_Surface, U16 i_PatchId) {
    LRU_Z<CacheStateLRU_Z, U16>::List_Ele* l_List = m_Cache.m_LRU.m_LRUList.GetArrayPtr();
    S32 l_LastIdx = m_Cache.m_LRU.m_LRUList.GetSize() - 1;
    U16 l_Id = l_List[l_LastIdx].m_PrevEntry - 1;
    LRU_Z<CacheStateLRU_Z, U16>::List_Ele& l_Ele = l_List[l_Id + 1];

    if (l_Ele.m_Val.m_OwnerId != U16_MAX) {
        ((Surface_Z*)l_Ele.m_Val.m_OwnerPtr)->m_PatchTab[l_Ele.m_Val.m_OwnerId].m_ColCacheId = -1;
    }

    l_Ele.m_Val.m_OwnerId = i_PatchId;
    l_Ele.m_Val.m_OwnerPtr = i_Surface;

    return l_Id;
}
