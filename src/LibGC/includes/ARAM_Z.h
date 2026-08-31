#ifndef _ARAMMANAGER_Z_H_
#include "XRamManager_Z.h"
#include "AramXAllocator_Z.h"
#include "HoleArray_Z.h"

#define ARAM_MGR_BLOCK_COUNT 1024
#define ARAM_MGR_ALIGNMENT 32
#define ARAM_MGR_TOTAL_SIZE 0

class AramManager_Z : public XRamManager_Z {
public:
    static HoleArray_Z<U8, 256> ReqTab;

    virtual Bool Init();
    virtual void Shut();
    virtual void Update(Float a1);
    virtual void Flush();
    virtual void Minimize();
    virtual S16 AllocBlock(S32 i_Size, S32 i_UserParam1, S32 i_UserParam2);

    virtual S32 GetBlockPtr(S16 i_BlockIdx) { return m_ARamAllocator.GetBlockPtr(i_BlockIdx); }

    virtual S32 GetBlockSize(S16 i_BlockIdx) { return m_ARamAllocator.GetBlockSize(i_BlockIdx); }

    virtual void FreeBlock(S16 a1);

    virtual void LockBlockAt(S16 i_BlockIdx, S32 i_User) { m_ARamAllocator.SetBlockDate(i_BlockIdx, i_User); }

    virtual U8* Get(S16 a1, S32 a2);
    virtual void Read(U8* a1, S32 a2, U32 a3, Bool a4);
    virtual void Write(U8* a1, S32 a2, U32 a3, Bool a4);

private:
    U8 m_Unk_0x4[4096];
    ARamXAllocator_Z<ARAM_MGR_BLOCK_COUNT, ARAM_MGR_ALIGNMENT, ARAM_MGR_TOTAL_SIZE> m_ARamAllocator;
};

void* AramManager_Alloc(S32 i_Size);
void AramManager_Free(void* i_Ptr);

#endif
