#ifndef _XRAMMANAGER_Z_H_
#define _XRAMMANAGER_Z_H_

#include "Types_Z.h"

class XRamManager_Z {
public:
    XRamManager_Z() { }

    ~XRamManager_Z();

    virtual Bool Init() {
        return FALSE;
    }

    virtual void Shut() { }

    virtual void Update(Float i_DeltaTime) { }

    virtual void Flush() { }

    virtual void Minimize() { }

    virtual S32 GetMemAllocated() { return 0; }

    virtual S16 AllocBlock(S32 i_Size, S32 i_UserParam1, S32 i_UserParam2) {
        return 0;
    }

    virtual U32 GetBlockPtr(S16 i_BlockIdx) {
        return 0;
    }

    virtual S32 GetBlockSize(S16 i_BlockIdx) {
        return 0;
    }

    virtual void FreeBlock(S16 a1) { }

    virtual void LockBlockAt(S16 a1, S32 a2) { }

    virtual U8* Get(S16 a1, S32 a2) {
        return NULL;
    }

    virtual void EndGet(U8* i_Data);

    virtual void Read(U8* a1, S32 a2, U32 a3, Bool a4) { }

    virtual void Write(U8* a1, S32 a2, U32 a3, Bool a4) { }
};

#endif
