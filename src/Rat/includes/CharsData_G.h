#ifndef _CHARSDATA_G_H_
#define _CHARSDATA_G_H_
#include "BaseObject_Z.h"
#include "Handle_Z.h"
#include "Program_Z.h"

class CharsData_G : public BaseObject_Z {
    U8 m_Unk_0x0c[12];

public:
    static BaseObject_Z* NewObject() { return NewL_Z(24) CharsData_G; }
};

HANDLE_Z(CharsData_G, BaseObject_Z);

typedef DynArray_Z<CharsData_GHdl, 4> CharsData_GHdlDA;
#endif // _CHARSDATA_G_H_
