#ifndef _ARAMXALLOCATOR_Z_H_
#define _ARAMXALLOCATOR_Z_H_

#include "Types_Z.h"
#include "Memory_Z.h"
#include "Assert_Z.h"

template <S32 BlockCount, S32 Alignment, S32 TotalSize>
class ARamXAllocator_Z {
public:
    static const int LAST_BLOCK = BlockCount - 1;

    struct Block {
        enum {
            BLOCK_NONE,
            BLOCK_USED,
            BLOCK_LOCKED = 2
        };

        S32 m_Date;
        S32 m_OwnerKey;
        S32 m_UserParam2;
        S32 m_StartAddress;
        S32 m_Size;
        S32 m_Flags;
        S16 m_PrevBlockIdx;
        S16 m_NextBlockIdx;
    };

    ARamXAllocator_Z() {
        m_StartAddress = 0;
        m_EndAddress = 0;
        m_AllocatedSize = 0;
        m_IsInitialized = FALSE;
        if (TotalSize) {
            Init(TotalSize);
        }
    }

    ~ARamXAllocator_Z() {
        Shut();
    }

    void Init(S32 i_Size) {
        S32 l_StartAddress = (S32)AllocAlignL_Z(i_Size, 43, Alignment);
        Init(l_StartAddress, i_Size, TotalSize != 0);
    }

    void Init(S32 i_StartAddress, S32 i_Size, Bool i_OwnsMemory) {
        ASSERTLEF_Z(!m_StartAddress && !m_EndAddress, "ARamXAllocator_Z already initialised", 52, "ARamXAllocator_Z.h", "!_bStart&&!_bEnd");
        ASSERTLEF_Z(i_StartAddress && i_Size, "Invalid Init Parameters ARamXAllocator_Z", 53, "ARamXAllocator_Z.h", "BStart&&BSize");

        m_StartAddress = i_StartAddress;
        m_EndAddress = i_StartAddress + (i_Size * BlockCount * BlockCount);
        m_Unk_0x1c10 = 0;
        m_IsInitialized = i_OwnsMemory;
        Init();
    }

    void Init() {
        m_FirstUsedBlockIdx = -1;
        m_FirstFreeBlockIdx = 0;
        for (S32 i = 0; i < LAST_BLOCK; i++) {
            Block& l_Block = m_Blocks[i];

            l_Block.m_NextBlockIdx = i + 1;
            l_Block.m_Flags = Block::BLOCK_NONE;
        }

        Block& l_LastBlock = m_Blocks[LAST_BLOCK];

        l_LastBlock.m_NextBlockIdx = -1;
        l_LastBlock.m_Flags = Block::BLOCK_NONE;
        m_AllocatedSize = 0;
    }

    void Shut() {
        if (m_IsInitialized) {
            if (m_StartAddress) {
                Free_Z((void*)m_StartAddress);
            }
        }
        m_StartAddress = NULL;
        m_EndAddress = NULL;
        m_AllocatedSize = 0;
        m_IsInitialized = FALSE;
    }

    S16 AllocBlock(S32 i_Size, S32 i_UserParam1, S32 i_UserParam2) {
        S16 l_BlockIdx;
        if (!i_Size) {
            return -1;
        }

        S32 l_Size = AlignUp_Z(i_Size, Alignment);

        if (m_FirstFreeBlockIdx == -1) {
            return -1;
        }

        S32 l_NewBlockIdx = m_FirstFreeBlockIdx;

        if (m_FirstUsedBlockIdx == -1) {
            S32 l_FreeSize = m_EndAddress - m_StartAddress;

            if (l_Size > l_FreeSize) {
                return -1;
            }

            Block* l_NewBlock = &m_Blocks[(S16)l_NewBlockIdx];

            m_FirstUsedBlockIdx = (S16)l_NewBlockIdx;
            m_FirstFreeBlockIdx = l_NewBlock->m_NextBlockIdx;

            l_NewBlock->m_StartAddress = m_StartAddress;
            l_NewBlock->m_Size = l_Size;
            l_NewBlock->m_Flags = Block::BLOCK_USED;
            l_NewBlock->m_OwnerKey = i_UserParam1;
            l_NewBlock->m_UserParam2 = i_UserParam2;
            l_NewBlock->m_Date = -1;
            l_NewBlock->m_NextBlockIdx = -1;
            l_NewBlock->m_PrevBlockIdx = -1;

            m_AllocatedSize += l_NewBlock->m_Size;

            return l_NewBlockIdx;
        }

        l_BlockIdx = m_FirstUsedBlockIdx;
        while (TRUE) {
            Block* l_Block = &m_Blocks[(S16)l_BlockIdx];

            if (l_Block->m_PrevBlockIdx == -1) {
                S32 l_FreeSize = l_Block->m_StartAddress - m_StartAddress;

                if (l_FreeSize >= l_Size) {
                    Block* l_NewBlock = &m_Blocks[(S16)l_NewBlockIdx];

                    m_FirstUsedBlockIdx = (S16)l_NewBlockIdx;
                    m_FirstFreeBlockIdx = l_NewBlock->m_NextBlockIdx;

                    l_NewBlock->m_StartAddress = m_StartAddress;
                    l_NewBlock->m_Size = l_Size;
                    l_NewBlock->m_Flags = Block::BLOCK_USED;
                    l_NewBlock->m_PrevBlockIdx = -1;
                    l_NewBlock->m_NextBlockIdx = l_BlockIdx;
                    l_NewBlock->m_OwnerKey = i_UserParam1;
                    l_NewBlock->m_UserParam2 = i_UserParam2;
                    l_NewBlock->m_Date = -1;

                    l_Block->m_PrevBlockIdx = l_NewBlockIdx;

                    m_AllocatedSize += l_NewBlock->m_Size;

                    return l_NewBlockIdx;
                }
            }
            else {
                S32 l_PrevBlockIdx = l_Block->m_PrevBlockIdx;
                Block* l_PrevBlock = &m_Blocks[(S16)l_PrevBlockIdx];

                S32 l_FreeStart = l_PrevBlock->m_StartAddress + l_PrevBlock->m_Size;
                S32 l_FreeSize = l_Block->m_StartAddress - l_FreeStart;

                if (l_FreeSize >= l_Size) {
                    Block* l_NewBlock = &m_Blocks[(S16)l_NewBlockIdx];

                    m_FirstFreeBlockIdx = l_NewBlock->m_NextBlockIdx;

                    l_NewBlock->m_StartAddress = l_PrevBlock->m_StartAddress + l_PrevBlock->m_Size;
                    l_NewBlock->m_Size = l_Size;
                    l_NewBlock->m_Flags = Block::BLOCK_USED;
                    l_NewBlock->m_PrevBlockIdx = l_Block->m_PrevBlockIdx;
                    l_NewBlock->m_NextBlockIdx = l_BlockIdx;
                    l_NewBlock->m_OwnerKey = i_UserParam1;
                    l_NewBlock->m_UserParam2 = i_UserParam2;
                    l_NewBlock->m_Date = -1;

                    l_PrevBlock->m_NextBlockIdx = l_NewBlockIdx;
                    l_Block->m_PrevBlockIdx = l_NewBlockIdx;

                    m_AllocatedSize += l_NewBlock->m_Size;

                    return l_NewBlockIdx;
                }
            }

            S16 l_NextBlockIdx = l_Block->m_NextBlockIdx;
            if (l_NextBlockIdx != -1) {
                l_BlockIdx = l_NextBlockIdx;
                continue;
            }
            else {
                S32 l_FreeStart = l_Block->m_StartAddress + l_Block->m_Size;
                S32 l_FreeSize = m_EndAddress - l_FreeStart;

                if (l_FreeSize >= l_Size) {
                    Block* l_NewBlock = &m_Blocks[(S16)l_NewBlockIdx];

                    m_FirstFreeBlockIdx = l_NewBlock->m_NextBlockIdx;

                    l_NewBlock->m_StartAddress = l_Block->m_StartAddress + l_Block->m_Size;
                    l_NewBlock->m_Size = l_Size;
                    l_NewBlock->m_Flags = Block::BLOCK_USED;
                    l_NewBlock->m_PrevBlockIdx = l_BlockIdx;
                    l_NewBlock->m_NextBlockIdx = -1;
                    l_NewBlock->m_OwnerKey = i_UserParam1;
                    l_NewBlock->m_UserParam2 = i_UserParam2;
                    l_NewBlock->m_Date = -1;

                    l_Block->m_NextBlockIdx = l_NewBlockIdx;

                    m_AllocatedSize += l_NewBlock->m_Size;

                    return l_NewBlockIdx;
                }

                return -1;
            }
        }
    }

    void FreeBlock(S16 i_BlockIdx) {
        S32 l_BlockIdx = i_BlockIdx;

        if (l_BlockIdx == -1 || m_FirstUsedBlockIdx == -1) {
            return;
        }

        ASSERTLEF_Z(l_BlockIdx < BlockCount && (m_Blocks[l_BlockIdx].m_Flags & Block::BLOCK_USED), "", 204, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");

        Block* l_Block = &m_Blocks[l_BlockIdx];

        m_AllocatedSize -= l_Block->m_Size;

        if (l_Block->m_PrevBlockIdx >= 0) {
            m_Blocks[l_Block->m_PrevBlockIdx].m_NextBlockIdx = l_Block->m_NextBlockIdx;
        }
        else {
            m_FirstUsedBlockIdx = l_Block->m_NextBlockIdx;
        }

        S16 l_NextBlockIdx = l_Block->m_NextBlockIdx;
        if (l_NextBlockIdx >= 0) {
            m_Blocks[l_NextBlockIdx].m_PrevBlockIdx = l_Block->m_PrevBlockIdx;
        }

        l_Block->m_Flags = Block::BLOCK_NONE;
        l_Block->m_NextBlockIdx = (S16)m_FirstFreeBlockIdx;
        m_FirstFreeBlockIdx = l_BlockIdx;

        if (l_BlockIdx <= m_Unk_0x1c10) {
            m_Unk_0x1c10 = l_BlockIdx;
        }
    }

    S16 GetNextBlock(S16 i_BlockIdx) const {
        if (i_BlockIdx == -1) {
            return (S16)m_FirstUsedBlockIdx;
        }
        return m_Blocks[i_BlockIdx].m_NextBlockIdx;
    }

    S16 GetBlockId(void* i_Ptr) {
        S16 l_BlockIdx = (S16)m_FirstUsedBlockIdx;
        while (l_BlockIdx > -1) {
            if ((void*)m_Blocks[l_BlockIdx].m_StartAddress == i_Ptr) {
                return l_BlockIdx;
            }
            l_BlockIdx = m_Blocks[l_BlockIdx].m_NextBlockIdx;
        }

        ASSERTLEF_Z(FALSE, "ARAM Block Not Found : Moved or Released.", 262, "ARamXAllocator_Z.h", "FALSE");
        return -1;
    }

    void LockBlock(S16 i_BlockIdx) {
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 279, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        m_Blocks[i_BlockIdx].m_Flags |= Block::BLOCK_LOCKED;
    }

    S32 GetBlockDate(S16 i_BlockIdx) const {
        if (i_BlockIdx < 0) {
            return -1;
        }
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 314, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        return m_Blocks[i_BlockIdx].m_Date;
    }

    S32 GetBlockOwnerKey(S16 i_BlockIdx) const {
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 321, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        return m_Blocks[i_BlockIdx].m_OwnerKey;
    }

    S32 GetBlockPtr(S16 i_BlockIdx) const {
        if (i_BlockIdx < 0) {
            return -1;
        }
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 245, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        return m_Blocks[i_BlockIdx].m_StartAddress;
    }

    S32 GetBlockSize(S16 i_BlockIdx) const {
        if (i_BlockIdx < 0) {
            return -1;
        }
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 270, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        return m_Blocks[i_BlockIdx].m_Size;
    }

    void SetBlockDate(S16 i_BlockIdx, S32 i_User) {
        if (i_BlockIdx < 0) {
            return;
        }
        ASSERTLEF_Z(i_BlockIdx < BlockCount && (m_Blocks[i_BlockIdx].m_Flags & Block::BLOCK_USED), "", 305, "ARamXAllocator_Z.h", "BlockHdl<NbAllocMax && _block[BlockHdl].flags&Block::BLOCK_USED");
        m_Blocks[i_BlockIdx].m_Date = i_User;
    }

public:
    Block m_Blocks[BlockCount];
    S32 m_FirstUsedBlockIdx;
    S32 m_FirstFreeBlockIdx;
    S32 m_StartAddress;
    S32 m_EndAddress;
    S32 m_Unk_0x1c10;
    S32 m_AllocatedSize;
    Bool m_IsInitialized;
};

#endif
