#ifndef _OBJECTCACHE_Z_H_
#define _OBJECTCACHE_Z_H_

#include "DynArray_Z.h"
#include "BaseObject_Z.h"
#include "Math_Z.h"
#include "Types_Z.h"
#include "DynArray_Z.h"
#include "Renderer_Z.h"

struct CacheStateFrame_Z {
    U16 m_Key;
    S8 m_Age;
    Bool m_UsedThisFrame;
    void* m_OwnerPtr;
};

typedef DynArray_Z<CacheStateFrame_Z, 8, FALSE> CacheStateFrame_ZDA;

template <class T, Bool DeleteObject = TRUE, S32 Align = _ALLOCDEFAULTALIGN>
class CacheEntryFrame_Z {
public:
    ~CacheEntryFrame_Z() {
        if (m_Data) {
            if (DeleteObject) {
                for (S32 i = 0; i < m_DataNb; i++) {
                    m_Data[i].~T();
                }
                Free_Z(m_Data);
                m_Data = NULL;
            }
        }
    }

    const CacheStateFrame_Z* GetEntryArray() const {
        return NULL;
    }

    S32 GetFree() const {
        return m_NbFree;
    }

    S32 GetSize() const {
        return m_EntryDA.GetSize();
    }

    void GetState(CacheState_Z& o_State) const {
        S32 l_TotalNb = m_EntryDA.GetSize();
        S32 l_FreeNb = m_NbFree;
        S32 l_MaxInUseNb = m_NbMaxUsed;
        S32 l_InUseNb = l_TotalNb - l_FreeNb;

        o_State.m_TotalNb += l_TotalNb;
        o_State.m_InUseNb += l_InUseNb;
        o_State.m_MaxInUseNb += l_MaxInUseNb;
    }

    void Update() {
        if (m_NbFree != m_EntryDA.GetSize()) {
            CacheStateFrame_Z* l_FirstEntry = m_EntryDA.GetArrayPtr();
            CacheStateFrame_Z* l_LastEntry = l_FirstEntry + m_HighestUsed;
            for (CacheStateFrame_Z* l_CurEntry = l_FirstEntry; l_CurEntry <= l_LastEntry; l_CurEntry++) {
                if (l_CurEntry->m_Key != 0xffff) {
                    if (l_CurEntry->m_Age == 2) {
                        U16 l_EntryIndex = l_CurEntry - l_FirstEntry;
                        m_Data[l_EntryIndex].FreeEntry(m_EntryDA[l_EntryIndex]);
                        CacheStateFrame_Z& l_Entry = m_EntryDA[l_EntryIndex];
                        l_Entry.m_Key = 0xffff;
                        l_Entry.m_Age = 0;
                        l_Entry.m_UsedThisFrame = TRUE;
                        l_Entry.m_OwnerPtr = NULL;
                        S32 l_FreeIndex = m_NbFree;
                        m_NbFree = l_FreeIndex + 1;
                        m_FreeDA[l_FreeIndex] = l_EntryIndex;
                    }
                    else {
                        l_CurEntry->m_UsedThisFrame = TRUE;
                        l_CurEntry->m_Age++;
                    }
                }
            }
        }
    }

private:
    CacheStateFrame_ZDA m_EntryDA;
    U16DA m_FreeDA;
    S32 m_NbFree;
    S32 m_Depth;
    S32 m_NbMaxUsed;
    S32 m_HighestUsed;
    T* m_Data;
    S32 m_DataNb;
};

template <class T, S32 Count, Bool DeleteObject, Bool InitObject, S32 Align>
class BnkCacheEntryFrame_Z {
public:
    struct BnkCacheEntryFrameEle_Z {
        BnkCacheEntryFrameEle_Z* m_NextBank;
        U16 m_BankEntryID;
        CacheEntryFrame_Z<T, DeleteObject, Align> m_Cache;
    };

    BnkCacheEntryFrame_Z() {
        m_HeadBank = NULL;
        m_LastBankEntryID = 0;
        m_MaxUsed = 0;
    }

    void Minimize() {
        while (TRUE) {
            BnkCacheEntryFrameEle_Z* l_FirstBank = m_HeadBank;
            BnkCacheEntryFrameEle_Z* l_LastBank = l_FirstBank;
            BnkCacheEntryFrameEle_Z* l_PrevBank = l_FirstBank;

            while (l_LastBank && l_LastBank->m_NextBank) {
                l_PrevBank = l_LastBank;
                l_LastBank = l_LastBank->m_NextBank;
            }

            if (l_LastBank != l_PrevBank && l_LastBank->m_Cache.GetFree() == Count) {
                m_LastBankEntryID -= Count;

                BnkCacheEntryFrameEle_Z* l_BankToDelete = l_PrevBank->m_NextBank;

                Delete_Z l_BankToDelete;

                l_PrevBank->m_NextBank = NULL;
            }
            else {
                if (l_FirstBank && !l_FirstBank->m_NextBank && l_FirstBank->m_Cache.GetFree() == Count) {
                    Delete_Z l_FirstBank;

                    m_HeadBank = NULL;
                    m_LastBankEntryID = 0;
                    m_MaxUsed = 0;
                }

                return;
            }
        }
    }

    void Update() {
        BnkCacheEntryFrameEle_Z* l_CurBank = m_HeadBank;

        while (l_CurBank) {
            l_CurBank->m_Cache.Update();

            l_CurBank = l_CurBank->m_NextBank;
        }

        Minimize();
    }

    void GetState(CacheState_Z& o_State) {
        BnkCacheEntryFrameEle_Z* l_Cur = m_HeadBank;

        while (l_Cur) {
            l_Cur->m_Cache.GetState(o_State);
            l_Cur = l_Cur->m_NextBank;
        }

        if (o_State.m_MaxInUseNb > m_MaxUsed) {
            m_MaxUsed = o_State.m_MaxInUseNb;
        }
    }

private:
    BnkCacheEntryFrameEle_Z* m_HeadBank;
    U16 m_LastBankEntryID;
    S32 m_MaxUsed;
};

// LRU Cache

struct CacheStateLRU_Z {
    BaseObject_Z* m_OwnerPtr;
    U16 m_OwnerId;

    CacheStateLRU_Z() {
        m_OwnerPtr = NULL;
        m_OwnerId = -1;
    }
};

template <class T, class V>
class LRU_Z {
public:
    struct List_Ele {
        T m_Val;
        V m_PrevEntry;
        V m_NextEntry;
    };

    void Init(S32 i_Size) {
        m_LRUList.SetSize(i_Size + 2);

        S32 l_PlusOne = i_Size + 1;
        for (S32 i = 0; i < i_Size + 2; i++) {
            m_LRUList[i].m_PrevEntry = Max(i - 1, (S32)0);
            m_LRUList[i].m_NextEntry = Min(i + 1, l_PlusOne);
        }
        FIXDEBUGINLINING_Z();
    }

private:
    DynArray_Z<List_Ele, 32, FALSE> m_LRUList;
};

template <class T>
class CacheEntryLRU_Z {
public:
    CacheEntryLRU_Z() {
        m_CacheData = NULL;
    }

    inline void SetSize(S32 i_LRUCount, S32 i_EntryCount) {
        m_CacheEntryCount = i_EntryCount;
        m_LRU.Init(i_LRUCount);
        if (m_CacheData) {
            if (TRUE) {
                Delete_Z[] m_CacheData;
            }
        }
        m_CacheData = NewL_Z(34) T[(i_LRUCount * m_CacheEntryCount)];
        return;
    }

private:
    LRU_Z<CacheStateLRU_Z, U16> m_LRU;
    S32 m_CacheEntryCount;
    T* m_CacheData;
};

#endif
