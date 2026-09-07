#ifndef _UNLOCK_G_H_
#define _UNLOCK_G_H_
#include "DynArray_Z.h"
class UnlockElem_G;
class Console_Z;

typedef DynArray_Z<UnlockElem_G, 4> UnlockElem_GDA;

class UnlockElem_G {
    U8 m_Pad_0x0[0xc0];
};

class UnLockEvents_G {
    UnlockElem_GDA m_UnlockElemDA;

public:
    void AddUnlockEvent(Console_Z* i_Console);
    void CheckUnlock(Bool i_Force);
};

#endif // _UNLOCK_G_H_
