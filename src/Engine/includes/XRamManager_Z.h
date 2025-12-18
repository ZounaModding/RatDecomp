#ifndef _XRAMMANAGER_Z_H_
#define _XRAMMANAGER_Z_H_

#include "Types_Z.h"

class XRamManager_Z {
public:
    XRamManager_Z();
    ~XRamManager_Z();

    virtual void Init();                                 /* 0x08 */
    virtual void Shut();                                 /* 0x0C */
    virtual void Update(Float i_DeltaTime);              /* 0x10 */
    virtual void Flush();                                /* 0x14 */
    virtual void Minimize();                             /* 0x18 */
    virtual void GetMemAllocated();                      /* 0x1C */
    virtual void AllocBlock(S32 a1, S32 a2, S32 a3);     /* 0x20 */
    virtual void GetBlockPtr(S16 a1);                    /* 0x24 */
    virtual void GetBlockSize(S16 a1);                   /* 0x28 */
    virtual void FreeBlock(S16 a1);                      /* 0x2C */
    virtual void LockBlockAt(S16 a1, S32 a2);            /* 0x30 */
    virtual void Get(S16 a1, S32 a2);                    /* 0x34 */
    virtual void EndGet(U8* a1);                         /* 0x38 */
    virtual void Read(U8* a1, S32 a2, U32 a3, Bool a4);  /* 0x3C */
    virtual void Write(U8* a1, S32 a2, U32 a3, Bool a4); /* 0x40 */
};

#endif
