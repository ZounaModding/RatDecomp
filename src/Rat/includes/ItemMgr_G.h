#ifndef _ITEMMGR_G_H_
#define _ITEMMGR_G_H_
#include "ObjectGame_Z.h"

Extern_Z U32 ItemMgr_FrameCounter;

class ItemMgr_G : public ObjectGame_Z {
public:
    virtual void Update(Float i_DeltaTime);

    void FlushUnusedItems(U32 i_Count, U32 i_MaxAge);

    U8 m_Unk_0x2c[8];
};
#endif // _ITEMMGR_G_H_
