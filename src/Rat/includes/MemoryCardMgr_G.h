#ifndef _MEMORYCARDMGR_G_H_
#define _MEMORYCARDMGR_G_H_
#include "Manipulator_Z.h"

class MemoryCardManager_C : public Manipulator_Z {
public:
    virtual ~MemoryCardManager_C();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(134) MemoryCardManager_C; }

private:
    U8 m_Unk_0x20[0x2f0];
};
#endif // _MEMORYCARDMGR_G_H_
