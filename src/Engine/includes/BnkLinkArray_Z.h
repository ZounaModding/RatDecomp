#ifndef _BNKLINKARRAY_Z_H_
#define _BNKLINKARRAY_Z_H_
#include "Types_Z.h"

template <class T, S32 Granularity = 256, S32 Alignment = 4>
class BnkLinkArrayEle_Z {
public:
private:
    BnkLinkArrayEle_Z<T>* m_NextBank;
    T m_Ele[Granularity];
    T* m_HeadEle;
    T* m_FirstEle;
    T* m_LastEle;
    S32 m_NbFree;
};

template <class T, S32 Granularity = 256, S32 Alignment = 4>
class BnkLinkArray_Z {
public:
private:
    BnkLinkArrayEle_Z<T, Granularity, Alignment>* m_HeadBank;
    S32 m_NbMaxUsed;
    S32 m_NbUsed;
    S32 m_NbElement;
};

#endif
