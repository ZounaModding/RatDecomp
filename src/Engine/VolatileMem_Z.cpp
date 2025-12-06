#include "VolatileMem_Z.h"

void VolatileMgr_Z::Minimize() { }

void VolatileMgr_Z::SetState(int i_ArrayOffset, S32 i_State) {
    VolatileBlock& l_VolBlock = m_VolatileBlockArray[i_ArrayOffset];
    if (l_VolBlock.m_State != i_State) {
        switch (l_VolBlock.m_State) {
            case 0: {
                RemoveIndex(m_FirstFreeBlock, i_ArrayOffset);
                break;
            }
            case 1: {
                RemoveIndex(m_FirstAllocatedBlock, i_ArrayOffset);
                break;
            }
        }
        l_VolBlock.m_State = i_State;
        switch (l_VolBlock.m_State) {
            case 0: {
                AddIndex(m_FirstFreeBlock, i_ArrayOffset);
                break;
            }
            case 1: {
                AddIndex(m_FirstAllocatedBlock, i_ArrayOffset);
                break;
            }
        }
    }

    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
    FIXDEBUGINLINING_Z();
}

VolatileMgr_Z::VolatileMgr_Z() {
    m_FirstFreeBlock = -1;
    m_FirstAllocatedBlock = -1;
    for (int i = 0; i < 0x500; i++) {
        S32 l_BlockId = m_VolatileBlockArray.Add();
        VolatileBlock& l_VolBlock = m_VolatileBlockArray[l_BlockId];
        l_VolBlock.m_Pointer = NULL;
        l_VolBlock.m_Key = 1;
        l_VolBlock.m_State = -1;
        SetState(l_BlockId, 0);
    }
}

void VolatileMgr_Z::UpdateVolatileBlocks(Float i_DeltaTime) {
    if (i_DeltaTime > 0.05f) {
        i_DeltaTime = 0.05f;
    }
    S32 l_BlockId;
    int l_CurBlockId;
    for (l_BlockId = m_FirstAllocatedBlock, l_CurBlockId = l_BlockId; l_CurBlockId >= 0; l_CurBlockId = l_BlockId) {
        VolatileBlock& l_VolBlock = m_VolatileBlockArray[l_CurBlockId];
        if (!l_VolBlock.m_Pointer) {
            *(VU32*)(0x0) = NULL;
        }
        l_BlockId = l_VolBlock.m_NextBlockToUpdate;
        l_VolBlock.m_Timer = l_VolBlock.m_Timer + i_DeltaTime;
        if (l_VolBlock.m_Timer > l_VolBlock.m_TimeToFree) {
            FreeVolatileBlock_Z(l_CurBlockId);
        }
    }
}

void VolatileMgr_Z::FreeVolatileBlock_Z(U32 i_BlockIdx) {
    if (i_BlockIdx < m_VolatileBlockArray.GetSize()) {
        int l_BlockIdx = i_BlockIdx;
        VolatileBlock& l_VolBlock = m_VolatileBlockArray[l_BlockIdx];
        if (l_VolBlock.m_Pointer) {
            Free_Z(l_VolBlock.m_Pointer);
            l_VolBlock.m_Pointer = NULL;
        }
        l_VolBlock.m_Zero = 0;
        l_VolBlock.m_Key++;
        l_VolBlock.m_Key = l_VolBlock.m_Key & 0xFFFF;
        if (!l_VolBlock.m_Key) {
            l_VolBlock.m_Key += 1;
        }
        SetState(i_BlockIdx, 0);
    }
}
