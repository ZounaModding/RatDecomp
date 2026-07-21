#ifndef _DATA_MANAGER_Z_H
#define _DATA_MANAGER_Z_H

#include "DynArray_Z.h"

#define DATAMGR_INITIAL_SIZE 2

template <typename T>
class DataManager_Z {
protected:
    enum State {
        RUNNING_MEM = 0,
        RELEASED_MEM = 1,
        SLEEPING_MEM = 2
    };

    struct CList {
        S16 m_First, m_Last;
        S16 m_Nb;
    };

public:
    struct Hdl {
        Hdl()
            : m_Id(-1) { }

        Hdl(int i_Id)
            : m_Id(i_Id) { }

        void Invalidate() {
            m_Id = -1;
        }

        Bool IsValid() {
            return (m_Id != -1);
        }

        int m_Id;
    };

    struct Item : public T {
        S16 m_Prev, m_Next;
    };

    struct Entry {
    protected:
        friend class DataManager_Z<T>;
        Item* m_ArrayPtr;
        State m_State;
        CList m_UsedList;
        CList m_FreeList;
        Float m_LastUsedTime;

    public:
        S16 GetMax() { return m_UsedList.m_Nb + m_FreeList.m_Nb; }

        S16 GetNbUsed() { return m_UsedList.m_Nb; }

        S16 GetFirst() { return m_UsedList.m_First; }

        void FlushElements();

        void ReleaseEle(S16 i_EleId) {
            if (m_UsedList.m_Nb == 0) {
                return;
            }
            if (i_EleId == -1) {
                return;
            }

            S16* l_Prev = &m_ArrayPtr[i_EleId].m_Prev;
            S16* l_Next = &m_ArrayPtr[i_EleId].m_Next;

            if (*l_Prev == -1) {
                m_UsedList.m_First = *l_Next;
            }
            else {
                m_ArrayPtr[*l_Prev].m_Next = *l_Next;
            }

            if (*l_Next == -1) {
                m_UsedList.m_Last = *l_Prev;
            }
            else {
                m_ArrayPtr[*l_Next].m_Prev = *l_Prev;
            }

            *l_Next = m_FreeList.m_First;
            if (m_FreeList.m_Nb == 0) {
                m_FreeList.m_First = m_FreeList.m_Last = i_EleId;
            }
            else {
                m_FreeList.m_First = i_EleId;
            }

            m_UsedList.m_Nb -= 1;
            m_FreeList.m_Nb += 1;
        }

        S16 GetFreeEle() {
            if (m_FreeList.m_Nb == 0) {
                return -1;
            }

            int l_EleId = m_FreeList.m_First;
            if (m_FreeList.m_Nb == 1) {
                m_FreeList.m_First = m_FreeList.m_Last = -1;
            }
            else {
                m_FreeList.m_First = m_ArrayPtr[l_EleId].m_Next;
            }

            if (m_UsedList.m_Nb == 0) {
                m_ArrayPtr[l_EleId].m_Prev = -1;
                m_ArrayPtr[l_EleId].m_Next = -1;
                m_UsedList.m_First = m_UsedList.m_Last = l_EleId;
            }
            else {
                m_ArrayPtr[l_EleId].m_Prev = m_UsedList.m_Last;
                m_ArrayPtr[l_EleId].m_Next = -1;
                m_ArrayPtr[m_UsedList.m_Last].m_Next = l_EleId;
                m_UsedList.m_Last = l_EleId;
            }

            m_UsedList.m_Nb += 1;
            m_FreeList.m_Nb -= 1;
            return l_EleId;
        }

        Item* GetArrayPtr() { return m_ArrayPtr; }
    };

    DataManager_Z(Char* i_Name = NULL);

    virtual ~DataManager_Z();
    virtual void Update(Float i_DeltaTime);

    Hdl RegisterObject(S16 i_NbMax);
    void ReleaseObject(Hdl i_EntryHdl);
    Entry* GetObjectEntry(Hdl i_EntryHdl);
    Entry* UpdateObjectEntry(Hdl i_EntryHdl, Bool* o_JustAllocated = NULL);
    Float GetCullTime();
    void SetCullTime(Float i_CullTime);
    void DoObjectsCulling(S16 i_NbRequest);
    Hdl GetLRUObjectEntry(Float i_MinUnusedTime);
    void Defragment();
    void Minimize();

protected:
    CList LinkList(CList& i_CList1, CList& i_CList2);
    CList CreateList(S16 i_First, S16 i_Nb);

    Char* m_Name;
    CList m_FreeList;
    Float m_CurTime;
    Float m_CullTime;
    U16 m_OutputCpt;
    Bool m_OutputEnable;

    DynArray_Z<Entry, 16> m_EntryDA;
    DynArray_Z<Item, 16> m_ItemDA;

    Bool AllocMem(Entry* i_Entry, S16 i_NbMax);
    void FreeMem(Entry* i_Entry, State i_ToState);
};

template <class T>
typename DataManager_Z<T>::CList
DataManager_Z<T>::LinkList(CList& i_CList1, CList& i_CList2) {
    if (i_CList1.m_Nb == 0) return i_CList2;
    if (i_CList2.m_Nb == 0) return i_CList1;
    m_ItemDA[i_CList1.m_Last].m_Next = i_CList2.m_First;
    m_ItemDA[i_CList2.m_First].m_Prev = i_CList1.m_Last;
    CList l_CList12;
    l_CList12.m_First = i_CList1.m_First;
    l_CList12.m_Last = i_CList2.m_Last;
    l_CList12.m_Nb = i_CList1.m_Nb + i_CList2.m_Nb;
    return l_CList12;
}

template <class T>
typename DataManager_Z<T>::CList
DataManager_Z<T>::CreateList(S16 i_First, S16 i_Nb) {
    CList l_CList;
    l_CList.m_Nb = i_Nb;
    l_CList.m_First = i_First;
    l_CList.m_Last = i_First + i_Nb - 1;
    for (int i = l_CList.m_First; i <= l_CList.m_Last; i++) {
        m_ItemDA[i].m_Prev = i - 1;
        m_ItemDA[i].m_Next = i + 1;
    }
    m_ItemDA[l_CList.m_First].m_Prev = -1;
    m_ItemDA[l_CList.m_Last].m_Next = -1;
    return l_CList;
}

template <class T>
DataManager_Z<T>::DataManager_Z(Char* i_Name) {
    m_Name = i_Name;
    m_CurTime = 0.f;
    m_CullTime = 2.0f;

    m_ItemDA.SetSize(DATAMGR_INITIAL_SIZE);

    m_FreeList = CreateList(0, DATAMGR_INITIAL_SIZE);

    m_OutputEnable = TRUE;
    m_OutputCpt = 0;
}

template <class T>
DataManager_Z<T>::~DataManager_Z() {
}

template <class T>
Float DataManager_Z<T>::GetCullTime() {
    return m_CullTime;
}

template <class T>
void DataManager_Z<T>::SetCullTime(Float i_CullTime) {
    if (i_CullTime < (1 / 60.0f)) return;
    m_CullTime = i_CullTime;
}

template <class T>
void DataManager_Z<T>::Update(Float i_DeltaTime) {
    m_CurTime += i_DeltaTime;

    if (m_CurTime > 10.0f) {
        S32 l_Size;
        S32 i;

        m_CurTime -= 10.0f;

        l_Size = m_EntryDA.GetSize();
        for (i = 0; i < l_Size; i++) {
            if (m_EntryDA[i].m_State == RUNNING_MEM) {
                m_EntryDA[i].m_LastUsedTime -= 10.0f;
            }
        }
    }
}

template <class T>
void DataManager_Z<T>::Defragment() {
}

template <class T>
void DataManager_Z<T>::Minimize() {
    S32 l_EntryCount;
    S32 l_LastEntry;

    for (S32 i = m_ItemDA.GetSize(); i > 0; --i) {
        m_ItemDA.GetArrayPtr()[i - 1].~Item();
    }

    DoObjectsCulling(30000);

    Bool l_Done = FALSE;
    while (!l_Done) {
        l_Done = TRUE;
        S16 l_Previous = -1;
        S16 l_Item = m_FreeList.m_First;

        while (l_Item != -1) {
            if (l_Item == m_ItemDA.GetSize() - 1) {
                if (l_Previous != -1) {
                    m_ItemDA[l_Previous].m_Next = m_ItemDA[l_Item].m_Next;
                }
                if (m_ItemDA[l_Item].m_Next != -1) {
                    m_ItemDA[m_ItemDA[l_Item].m_Next].m_Prev = l_Previous;
                }
                if (l_Item == m_FreeList.m_First) {
                    m_FreeList.m_First = m_ItemDA[l_Item].m_Next;
                }
                if (l_Item == m_FreeList.m_Last) {
                    m_FreeList.m_Last = l_Previous;
                }
                --m_FreeList.m_Nb;
                m_ItemDA.SetSize(m_ItemDA.GetSize() - 1, TRUE);
                l_Done = FALSE;
                break;
            }

            l_Previous = l_Item;
            l_Item = m_ItemDA[l_Item].m_Next;
        }
    }

    m_ItemDA.Minimize();

    l_EntryCount = m_EntryDA.GetSize();
    if (l_EntryCount) {
        for (l_LastEntry = l_EntryCount - 1; l_LastEntry >= 0; --l_LastEntry) {
            if (m_EntryDA[l_LastEntry].m_State != RELEASED_MEM) {
                break;
            }
        }
        m_EntryDA.SetSize(l_LastEntry + 1);
    }
    m_EntryDA.Minimize();
}

template <class T>
void DataManager_Z<T>::DoObjectsCulling(S16 i_NbRequest) {
    while (i_NbRequest > 0) {
        Hdl l_Hdl = GetLRUObjectEntry(m_CullTime);
        if (!l_Hdl.IsValid()) return;

        Entry* l_Lru = &m_EntryDA[l_Hdl.m_Id];
        i_NbRequest -= l_Lru->GetMax();
        FreeMem(l_Lru, SLEEPING_MEM);
    }
}

template <class T>
typename DataManager_Z<T>::Hdl
DataManager_Z<T>::RegisterObject(S16 i_NbMax) {
    if (i_NbMax == 0) return Hdl();

    Entry l_Entry;
    l_Entry.m_State = SLEEPING_MEM;
    l_Entry.m_UsedList.m_Nb = 0;
    l_Entry.m_FreeList.m_Nb = i_NbMax;

    int l_NbObj = m_EntryDA.GetSize();
    for (int i = 0; i < l_NbObj; i++) {
        if (m_EntryDA[i].m_State == RELEASED_MEM) {
            m_EntryDA[i] = l_Entry;
            return Hdl(i);
        }
    }
    return Hdl(m_EntryDA.Add(l_Entry));
}

template <class T>
void DataManager_Z<T>::ReleaseObject(Hdl i_EntryHdl) {
    if (i_EntryHdl.m_Id == U32_MINUS_ONE) {
        return;
    }
    Entry* l_Entry = &m_EntryDA[i_EntryHdl.m_Id];
    FreeMem(l_Entry, RELEASED_MEM);
}

template <class T>
Bool DataManager_Z<T>::AllocMem(Entry* i_Entry, S16 i_NbMax) {
    if (!i_Entry) return FALSE;
    if (i_Entry->m_State != SLEEPING_MEM) return FALSE;
    if (i_NbMax == 0) return FALSE;

    if (m_FreeList.m_Nb < i_NbMax) {
        DoObjectsCulling(i_NbMax);
    }

    if (m_FreeList.m_Nb < i_NbMax) {
        int l_Nb = i_NbMax + 16;
        int l_First = m_ItemDA.GetSize();
        if (l_First + l_Nb > 32000) return FALSE;

        m_ItemDA.SetSize(l_First + l_Nb);
        CList l_CList = CreateList(l_First, l_Nb);
        m_FreeList = LinkList(m_FreeList, l_CList);
    }

    i_Entry->m_State = RUNNING_MEM;
    i_Entry->m_UsedList.m_Nb = 0;
    i_Entry->m_UsedList.m_First = -1;
    i_Entry->m_UsedList.m_Last = -1;
    i_Entry->m_FreeList.m_Nb = i_NbMax;
    i_Entry->m_FreeList.m_First = m_FreeList.m_First;
    m_FreeList.m_Nb -= i_NbMax;
    while (i_NbMax--) {
        i_Entry->m_FreeList.m_Last = m_FreeList.m_First;
        m_FreeList.m_First = m_ItemDA[m_FreeList.m_First].m_Next;
    }
    m_ItemDA[i_Entry->m_FreeList.m_Last].m_Next = -1;
    if (m_FreeList.m_Nb == 0) {
        m_FreeList.m_First = m_FreeList.m_Last = -1;
    }
    return TRUE;
}

template <class T>
void DataManager_Z<T>::FreeMem(Entry* i_Entry, State i_ToState) {
    if (i_Entry->m_State == RUNNING_MEM) {
        m_FreeList = LinkList(m_FreeList, i_Entry->m_UsedList);
        m_FreeList = LinkList(m_FreeList, i_Entry->m_FreeList);
    }

    i_Entry->m_State = i_ToState;
    i_Entry->m_ArrayPtr = NULL;
}

template <class T>
typename DataManager_Z<T>::Entry*
DataManager_Z<T>::UpdateObjectEntry(Hdl i_EntryHdl, Bool* o_JustAllocated) {
    if (i_EntryHdl.m_Id == U32_MINUS_ONE) return NULL;
    if (i_EntryHdl.m_Id >= m_EntryDA.GetSize()) return NULL;
    Entry* l_Entry = &m_EntryDA[i_EntryHdl.m_Id];
    if (o_JustAllocated) *o_JustAllocated = FALSE;

    if (l_Entry->m_State == RELEASED_MEM) {
        return NULL;
    }
    else if (l_Entry->m_State == SLEEPING_MEM) {
        S16 l_NbMax = l_Entry->GetMax();
        if (!AllocMem(l_Entry, l_NbMax)) return NULL;
        if (o_JustAllocated) *o_JustAllocated = TRUE;
    }

    l_Entry->m_LastUsedTime = m_CurTime;
    l_Entry->m_ArrayPtr = m_ItemDA.GetArrayPtr();
    return l_Entry;
}

template <class T>
typename DataManager_Z<T>::Entry*
DataManager_Z<T>::GetObjectEntry(Hdl i_EntryHdl) {
    if (i_EntryHdl.m_Id == U32_MINUS_ONE) return NULL;
    if ((U32)i_EntryHdl.m_Id >= m_EntryDA.GetSize()) return NULL;
    Entry* l_Entry = &m_EntryDA[i_EntryHdl.m_Id];

    if (l_Entry->m_State != RUNNING_MEM) return NULL;

    l_Entry->m_LastUsedTime = m_CurTime;
    l_Entry->m_ArrayPtr = m_ItemDA.GetArrayPtr();
    return l_Entry;
}

template <class T>
typename DataManager_Z<T>::Hdl
DataManager_Z<T>::GetLRUObjectEntry(Float i_MinUnusedTime) {
    int l_LruId = -1;
    Float l_LruAging = i_MinUnusedTime;

    int l_NbObj = m_EntryDA.GetSize();
    for (int i = 0; i < l_NbObj; i++) {
        Entry* l_Entry = &m_EntryDA[i];
        if (l_Entry->m_State != RUNNING_MEM) continue;

        Float l_Aging = m_CurTime - l_Entry->m_LastUsedTime;
        if (l_Aging <= l_LruAging) continue;

        l_LruId = i;
        l_LruAging = l_Aging;
    }

    if (l_LruId != -1) return Hdl(l_LruId);
    return Hdl();
}

template <typename T>
void DataManager_Z<T>::Entry::FlushElements() {
    S16 l_Index = m_UsedList.m_First;
    S16 l_NextIndex = -1;
    while (-1 != l_Index) {
        l_NextIndex = m_ArrayPtr[l_Index].m_Next;

        m_ArrayPtr[l_Index].m_Next = m_FreeList.m_First;
        m_FreeList.m_First = l_Index;
        if (-1 == m_FreeList.m_Last) {
            m_FreeList.m_Last = l_Index;
        }
        ++m_FreeList.m_Nb;

        l_Index = l_NextIndex;
    }

    m_UsedList.m_First = -1;
    m_UsedList.m_Last = -1;
    m_UsedList.m_Nb = 0;
}

#endif
