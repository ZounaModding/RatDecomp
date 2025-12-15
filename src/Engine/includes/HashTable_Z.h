#ifndef _HASHTABLE_Z_H_
#define _HASHTABLE_Z_H_
#include "BitArray_Z.h"
#include "Name_Z.h"

Extern_Z "C" void memset(void* __s, S32 __c, U32 __n);

#define HASHTABLE_DEFAULT_SIZE 16

class S32Hash_Z {
public:
    S32 m_Value;
    S32 m_Ref;

    S32Hash_Z() { }

    S32Hash_Z(S32 i_Value) { m_Value = i_Value; }

    S32Hash_Z(S32 i_Value, S32 i_Ref) {
        m_Value = i_Value;
        m_Ref = i_Ref;
    }

    inline S32 HashBase() const { return m_Value; }

    inline S32 HashIncrement() const {
        return HashBase() == 0;
    }

    inline Bool IsEmpty() { return !m_Ref; }

    inline void SetShadow() { m_Ref = -1; }

    Bool operator==(const S32Hash_Z& i_Elem) { return i_Elem.m_Value == m_Value; }

    Bool operator!=(const S32Hash_Z& i_Elem) { return i_Elem.m_Value != m_Value; }
};

class Name_ZHash_Z {
public:
    Name_Z m_Value;
    S32 m_Ref;

    Name_ZHash_Z() { }

    Name_ZHash_Z(const Name_Z& i_Value) {
        m_Value = i_Value;
    }

    Name_ZHash_Z(const Name_Z& i_Value, S32 i_Ref) {
        m_Value = i_Value;
        m_Ref = i_Ref;
    }

    inline S32 HashBase() const { return m_Value.m_ID; }

    inline S32 HashIncrement() const {
        return HashBase() == 0;
    }

    inline Bool IsEmpty() { return !m_Ref; }

    inline void SetShadow() { m_Ref = -1; }

    Bool operator==(const Name_ZHash_Z& i_Elem) { return i_Elem.m_Value == m_Value; }

    Bool operator!=(const Name_ZHash_Z& i_Elem) { return i_Elem.m_Value != m_Value; }
};

/* sizeof(HashTableBase_Z) == 0x14*/
template <class T>
class HashTableBase_Z {
    BitArray_Z* m_Status;
    T* m_Hash;
    S32 m_NbElem;
    S32 m_NbFree;
    S32 m_ScanID;

public:
    void Resize(S32 i_NewSize) {
        S32 l_OldSize;
        S32 l_NextScan;

        l_OldSize = 1;
        while (l_OldSize < i_NewSize)
            l_OldSize <<= 1;
        l_OldSize <<= 1;
        i_NewSize = l_OldSize;

        BitArray_Z* l_OldStatus = m_Status;
        T* l_OldHash = m_Hash;
        m_Status = NewL_Z(62) BitArray_Z(i_NewSize);
        m_Status->ClearAllBits();
        m_Hash = (T*)AllocCL_Z(sizeof(T) * i_NewSize, "HashTable_Z::Resize", 65);
        memset(m_Hash, 0, sizeof(T) * i_NewSize);

        S32 OldNbElem = m_NbElem;

        m_NbElem = 0;
        m_NbFree = i_NewSize;

        if (!l_OldStatus)
            return;

        l_NextScan = 0;
        while ((l_NextScan = l_OldStatus->FindFirstBit(TRUE, l_NextScan)) >= 0) {
            Insert(*(l_OldHash + l_NextScan));
            l_NextScan++;
        }
        Free_Z(l_OldHash);
        Delete_Z l_OldStatus;
    }

    HashTableBase_Z() {
        m_NbElem = 0;
        m_NbFree = 0;
        m_ScanID = -1;
        m_Status = NULL;
        m_Hash = NULL;
        Resize(HASHTABLE_DEFAULT_SIZE);
    }

    HashTableBase_Z(S32 i_Size) {
        m_NbElem = 0;
        m_NbFree = 0;
        m_ScanID = -1;
        m_Status = NULL;
        m_Hash = NULL;
        Resize(i_Size);
    }

    ~HashTableBase_Z() {
        Flush();
    }

    void Flush() {
        if (m_Status) {
            Delete_Z m_Status;
            m_Status = NULL;
        }
        if (m_Hash) {
            Free_Z(m_Hash);
            m_Hash = NULL;
        }
        m_NbElem = 0;
        m_NbFree = 0;
        m_ScanID = -1;
    }

    void Minimize() {
        Resize(m_NbElem);
    }

    Bool Insert(const T& i_Ele) {
        if (!m_Status) {
            Resize(HASHTABLE_DEFAULT_SIZE);
        }

        S32 l_HashSize = m_Status->GetSize() - 1;
        S32 l_HashID = i_Ele.HashBase() & l_HashSize;
        S32 l_HashInc = i_Ele.HashIncrement();
        S32 l_ShadowHashID = -1;
        if (!(l_HashInc & 0x1)) {
            l_HashInc++;
        }

        for (;;) {
            if (!m_Status->GetBit(l_HashID)) {
                if (!m_Hash[l_HashID].m_Ref) {
                    //if (m_Hash[l_HashID].IsEmpty()) {
                    if (l_ShadowHashID < 0)
                        m_NbFree--;
                    else
                        l_HashID = l_ShadowHashID;

                    m_Status->SetBit(l_HashID);
                    *(m_Hash + l_HashID) = i_Ele;

                    m_NbElem++;

                    if ((m_NbFree == 0) || (m_NbFree < (m_Status->GetSize() >> 2))) {
                        Resize(m_NbElem);
                    }
                    return TRUE;
                }
                else if (l_ShadowHashID < 0) {
                    l_ShadowHashID = l_HashID;
                }
            }
            else if (m_Hash[l_HashID] == i_Ele) {
                return FALSE;
            }
            l_HashID = (l_HashID + l_HashInc) & l_HashSize;
        }
    }

    const T* Search(const T& i_Element) const {
        if (!m_NbElem)
            return NULL;
        S32 l_HashSize = m_Status->GetSize() - 1;
        S32 l_HashID = i_Element.HashBase() & l_HashSize;
        S32 l_HashInc = i_Element.HashIncrement();
        if (!(l_HashInc & 0x1))
            l_HashInc++;

        for (;;) {
            if (!m_Status->GetBit(l_HashID)) {
                if (m_Hash[l_HashID].IsEmpty())
                    return NULL;
            }
            else {
                T* l_Ptr = m_Hash + l_HashID;
                if (*l_Ptr == i_Element)
                    return l_Ptr;
            }
            l_HashID = (l_HashID + l_HashInc) & l_HashSize;
        }
        return NULL;
    }

    Bool Suppress(const T& i_Element) {
        if (!m_NbElem)
            return FALSE;
        S32 l_HashSize = m_Status->GetSize() - 1;
        S32 l_HashID = i_Element.HashBase() & l_HashSize;
        S32 l_HashInc = i_Element.HashIncrement();
        if (!(l_HashInc & 0x1))
            l_HashInc++;

        for (;;) {
            if (!m_Status->GetBit(l_HashID)) {
                if (m_Hash[l_HashID].IsEmpty())
                    return FALSE;
            }
            else {
                T* l_Ptr = m_Hash + l_HashID;
                if (*l_Ptr == i_Element) {
                    m_Status->ClearBit(l_HashID);
                    l_Ptr->SetShadow();
                    m_NbElem--;
                    return TRUE;
                }
            }
            l_HashID = (l_HashID + l_HashInc) & l_HashSize;
        }
        return FALSE;
    }

    inline void InitScan() {
        if (!m_NbElem)
            m_ScanID = -1;
        else
            m_ScanID = 0;
    }

    inline T* NextScan() {
        if (m_ScanID < 0) return NULL;
        if (m_ScanID >= m_Status->GetSize()) {
            m_ScanID = -1;
            return NULL;
        }

        S32 NextScan = m_Status->FindFirstBit(TRUE, m_ScanID);

        if (NextScan < 0) {
            m_ScanID = -1;
            return NULL;
        }

        m_ScanID = NextScan + 1;
        return m_Hash + NextScan;
    }

    inline S32 GetNbElem() const {
        return m_NbElem;
    }
};

typedef HashTableBase_Z<S32Hash_Z> HashS32Table_Z;
typedef HashTableBase_Z<Name_ZHash_Z> HashName_ZTable_Z;

#endif
