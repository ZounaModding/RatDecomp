#ifndef _DIALOGGROUP_G_H_
#define _DIALOGGROUP_G_H_
#include "BaseObject_Z.h"
#include "Handle_Z.h"
#include "Name_Z.h"
#include "Program_Z.h"

class DialogGroup_G : public BaseObject_Z {
public:
    static BaseObject_Z* NewObject() { return NewL_Z(39) DialogGroup_G; }

    void RandomizeTT();
    S32 GetTT();

private:
    Name_Z m_Name;
    Float m_MinDelay;
    Float m_MaxDelay;
    Float m_MinDistance;
    Float m_MaxDistance;
    Bool m_Repeat;
    U8 m_Pad_0x21[3];
    DynArray_Z<S32> m_TextIds;
    S32 m_CurrentTT;
    Float m_Timer;
};

HANDLE_Z(DialogGroup_G, BaseObject_Z);

typedef DynArray_Z<DialogGroup_GHdl, 4> DialogGroup_GHdlDA;
#endif // _DIALOGGROUP_G_H_
