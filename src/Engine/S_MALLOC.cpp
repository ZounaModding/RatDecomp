#include "S_MALLOC.h"
#include "Assert_Z.h"
#include "Sys_Z.h"
#include <stdio.h>

void s_FREE_MEM_BUCKETS::Extend() {
    if (IsReallocating) {
        return;
    }

    if (!NeedToExtend) {
        return;
    }

    IsReallocating = TRUE;

    S32 l_NewBucketTotalBlocks = (m_BucketTotalBlocks * 3) >> 1;

    s_MEM_BLOCK** l_NewBucketBlocks = (s_MEM_BLOCK**)s_malloc(l_NewBucketTotalBlocks * (s_MEM_BUCKET_COUNT * sizeof(s_MEM_BLOCK*)), 4);
    s_MEM_BLOCK** l_OldBucketBlocks = m_BucketBlocks;

    S32 l_BucketIdx = 0;
    S32 l_NewBucketOffset = 0;

    do {
        S32 l_BucketCount = m_BucketFreeBlocks[l_BucketIdx];

        if (l_BucketCount) {
            Sys_Z::MemCpyFrom(
                l_NewBucketBlocks + l_NewBucketTotalBlocks * l_BucketIdx,
                l_OldBucketBlocks + (l_BucketIdx * m_BucketTotalBlocks),
                l_BucketCount * sizeof(s_MEM_BLOCK*)
            );
        }

        l_BucketIdx++;
    } while (l_BucketIdx < s_MEM_BUCKET_COUNT);

    m_BucketBlocks = l_NewBucketBlocks;
    m_BucketTotalBlocks = l_NewBucketTotalBlocks;

    if (HasAllocatedFreeBlock) {
        s_free(l_OldBucketBlocks);
    }

    HasAllocatedFreeBlock = TRUE;
    IsReallocating = FALSE;
    NeedToExtend = FALSE;
}

S32 s_FREE_MEM_BUCKETS::s_GetInBucket(S32 i_BucketIdx, s_MEM_BLOCK* i_Block) {
    s_MEM_BLOCK** l_BucketTrack;
    S32 l_Count = m_BucketFreeBlocks[i_BucketIdx];

    if (l_Count) {
        l_BucketTrack = m_BucketBlocks + (m_BucketTotalBlocks * i_BucketIdx);
        U32 l_BlockSpan = i_Block->GetSize();

        S32 l_Idx = s_Dicho_RecEqual(l_BucketTrack, l_BlockSpan, 0, l_Count - 1);

        if (l_Idx < l_Count) {
            s_MEM_BLOCK** l_Backward = l_BucketTrack + l_Idx;
            s_MEM_BLOCK** l_Forward = l_BucketTrack + (l_Idx + 1);
            s_MEM_BLOCK** l_End = l_BucketTrack + l_Count;

            while (TRUE) {
                if (l_Backward < l_BucketTrack) {
                    break;
                }

                if (l_Forward >= l_End) {
                    goto lastCheck;
                }

                s_MEM_BLOCK* l_BackwardBlock = *l_Backward;
                s_MEM_BLOCK* l_ForwardBlock = *l_Forward;

                if (l_BackwardBlock == i_Block) {
                    return l_Backward - l_BucketTrack;
                }

                if (l_ForwardBlock == i_Block) {
                    return l_Forward - l_BucketTrack;
                }

                l_Backward--;
                l_Forward++;
            }

            while (TRUE) {
                if (l_Forward >= l_End) {
                    goto returnMinusOne;
                }

                if (*l_Forward == i_Block) {
                    return l_Forward - l_BucketTrack;
                }

                l_Forward++;
            }

        lastCheck:
            while (TRUE) {
                if (l_Backward < l_BucketTrack) {
                    break;
                }

                if (*l_Backward == i_Block) {
                    return l_Backward - l_BucketTrack;
                }

                l_Backward--;
            }
        }
    }
returnMinusOne:
    return -1;
}

s_MEM_BLOCK* s_FREE_MEM_BUCKETS::FindBlock(S32 i_BucketIdx, S32 i_MinSize) {
    S32 l_Count = m_BucketFreeBlocks[i_BucketIdx];

    if (!l_Count) {
        return NULL;
    }

    s_MEM_BLOCK** l_BucketTrack = m_BucketBlocks + (m_BucketTotalBlocks * i_BucketIdx);

    S32 l_Idx = s_Dicho_RecBigger(l_BucketTrack, i_MinSize, 0, l_Count - 1);

    if (l_Idx < 0) {
        return NULL;
    }

    S32 l_Size = l_BucketTrack[l_Idx]->GetSize();
    s_MEM_BLOCK** l_Backward = l_BucketTrack + (l_Idx - 1);
    s_MEM_BLOCK** l_Forward = l_BucketTrack + (l_Idx + 1);
    s_MEM_BLOCK** l_End = l_BucketTrack + l_Count;
    s_MEM_BLOCK* l_BestBlock = l_BucketTrack[l_Idx];

    while (TRUE) {
        if (l_Backward < l_BucketTrack) {
            break;
        }

        if (l_Forward >= l_End) {
            goto scanBackward;
        }

        s_MEM_BLOCK* l_BackwardBlock = *l_Backward;
        s_MEM_BLOCK* l_ForwardBlock = *l_Forward;

        if (l_BackwardBlock->GetSize() != l_Size) {
            break;
        }

        if (l_ForwardBlock->GetSize() != l_Size) {
            goto scanBackward;
        }

        if (l_BackwardBlock < l_BestBlock) {
            l_BestBlock = l_BackwardBlock;
        }

        if (l_ForwardBlock < l_BestBlock) {
            l_BestBlock = l_ForwardBlock;
        }

        l_Backward--;
        l_Forward++;
    }

    while (TRUE) {
        if (l_Forward >= l_End) {
            goto returnBestBlock;
        }

        s_MEM_BLOCK* l_Block = *l_Forward;

        if (l_Block->GetSize() != l_Size) {
            goto returnBestBlock;
        }

        if (l_Block < l_BestBlock) {
            l_BestBlock = l_Block;
        }

        l_Forward++;
    }

scanBackward:
    while (TRUE) {
        if (l_Backward < l_BucketTrack) {
            break;
        }

        s_MEM_BLOCK* l_Block = *l_Backward;

        if (l_Block->GetSize() != l_Size) {
            break;
        }

        if (l_Block < l_BestBlock) {
            l_BestBlock = l_Block;
        }

        l_Backward--;
    }
returnBestBlock:
    return l_BestBlock;
}

void s_FREE_MEM_BUCKETS::AddBlock(s_MEM_BLOCK* i_Block) {
    S32 l_Bucket = FindBucket(i_Block->GetSize());
    AddBlockToBucket(l_Bucket, i_Block);
}

void s_FREE_MEM_BUCKETS::RemoveBlock(s_MEM_BLOCK* i_Block) {
    RemoveBlockInline(i_Block);
}

s_MEM_BLOCK* s_FREE_MEM_BUCKETS::FindBlock(S32 i_MinSize) {
    s_MEM_BLOCK* l_Block = FindBestBlock(i_MinSize);

    if ((U32)l_Block >= BestFitLimit) {
        return FindFirstBlock(i_MinSize);
    }

    return l_Block;
}

void s_Init(void* i_StartOfMemory, U32 i_TotalMemory) {
    // Reset global allocator state.
    IsReallocating = FALSE;
    HasAllocatedFreeBlock = FALSE;
    RealAllocatedMem = 0;

    // Store arena bounds.
    StartOfMemory = i_StartOfMemory;
    EndOfMemory = (U32)i_StartOfMemory + i_TotalMemory;

    // At initialization, the allocator has one free block: the root block.
    MAXnbFreeBlocks = 1;

    // Place the allocation-strategy threshold around 4/5 into the arena.
    // The result is aligned down to 8 bytes.
    BestFitLimit = (U32)i_StartOfMemory + AlignDown_Z((i_TotalMemory / 5) * 4, 8);

    // The free-bucket manager is stored at the beginning of the arena itself.
    FreeBuckets = (s_FREE_MEM_BUCKETS*)i_StartOfMemory;

    // The root block is embedded inside the bucket manager and represents the
    // initial free memory range.
    s_MEM_BLOCK* l_RootBlock = &FreeBuckets->m_RootBlock;

    // Initialize bucket tracking.
    FreeBuckets->Init();

    // Initialize the root block as the single initial free block.
    l_RootBlock->Init();

    // Insert the root block into the correct free bucket.
    FreeBuckets->AddBlock(l_RootBlock);
}

void s_MarkMem(U32 i_Mark) {
    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);

    if (!l_Block) {
        return;
    }

    if (!l_Block->GetFree()) {
        l_Block->SetMarked(i_Mark);
    }

    s_MEM_BLOCK* l_PrevBlock = l_Block->GetPrev();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            l_PrevBlock->SetMarked(i_Mark);
        }

        l_PrevBlock = l_PrevBlock->GetPrev();
    }

    l_PrevBlock = l_Block->GetNext();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            l_PrevBlock->SetMarked(i_Mark);
        }

        l_PrevBlock = l_PrevBlock->GetNext();
    }
}

U32 s_ShowUnMarkedMem() {
    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);

    if (!l_Block) {
        return 0;
    }

    S32 l_Count = 0;
    Char l_Buffer[512];

    if (!l_Block->GetFree() && !l_Block->GetMarked()) {
        U32 l_Size = l_Block->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_Block->GetAlignDecal();
        sprintf(l_Buffer, "| Size : %d/%d |\n", l_Size, l_Block->GetSize());
        l_Count = 1;
    }

    s_MEM_BLOCK* l_PrevBlock = l_Block->GetPrev();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree() && !l_PrevBlock->GetMarked()) {
            U32 l_Size = l_PrevBlock->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_PrevBlock->GetAlignDecal();
            sprintf(l_Buffer, "| Size : %d/%d |\n", l_Size, l_PrevBlock->GetSize());
            l_Count++;
        }

        l_PrevBlock = l_PrevBlock->GetPrev();
    }

    l_PrevBlock = l_Block->GetNext();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree() && !l_PrevBlock->GetMarked()) {
            U32 l_Size = l_PrevBlock->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_PrevBlock->GetAlignDecal();
            sprintf(l_Buffer, "| Size : %d/%d |\n", l_Size, l_PrevBlock->GetSize());
            l_Count++;
        }

        l_PrevBlock = l_PrevBlock->GetNext();
    }

    return l_Count;
}

U32 s_GetAllocatedMem() {
    return RealAllocatedMem;
}

static inline Bool s_SwapMallocStat(
    U32& io_CurrentSize, U32& io_NextSize,
    U32& io_CurrentCount, U32& io_NextCount
) {
    U32 l_NextSize = io_NextSize;
    U32 l_NextCount = io_NextCount;
    U32 l_CurrentCount = io_CurrentCount;
    io_NextSize = io_CurrentSize;
    io_NextCount = l_CurrentCount;
    io_CurrentCount = l_NextCount;
    io_CurrentSize = l_NextSize;
    return FALSE;
}

#pragma warn_uninitializedvar off

void s_ShowMostNbMalloc() {
    U32 l_Counts[4096];
    U32 l_Sizes[4096];
    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);
    s_MEM_BLOCK* l_CurrentBlock;
    U32 l_NumSizes = 0;
    U32 l_Idx = 0;
    Bool l_Sorted;

    for (l_Idx = 0; l_Idx < l_NumSizes; l_Idx++) {
        if (l_Sizes[l_Idx] == l_Block->GetSize()) {
            break;
        }
    }

    if (l_Idx >= l_NumSizes) {
        l_Sizes[l_Idx] = l_Block->GetSize();
        l_Counts[l_Idx] = 0;
        l_NumSizes++;
    }

    l_Counts[l_Idx]++;

    l_CurrentBlock = l_Block->GetPrev();

    while (l_CurrentBlock) {
        if (l_CurrentBlock->GetFree()) {
            for (l_Idx = 0; l_Idx < l_NumSizes; l_Idx++) {
                if (l_Sizes[l_Idx] == l_CurrentBlock->GetSize()) {
                    break;
                }
            }

            if (l_Idx >= l_NumSizes) {
                l_Sizes[l_Idx] = l_CurrentBlock->GetSize();
                l_Counts[l_Idx] = 0;
                l_NumSizes++;
            }

            l_Counts[l_Idx]++;
        }

        l_CurrentBlock = l_CurrentBlock->GetPrev();
    }

    l_CurrentBlock = l_Block->GetNext();

    while (l_CurrentBlock) {
        if (l_CurrentBlock->GetFree()) {
            for (l_Idx = 0; l_Idx < l_NumSizes; l_Idx++) {
                if (l_Sizes[l_Idx] == l_CurrentBlock->GetSize()) {
                    break;
                }
            }

            if (l_Idx >= l_NumSizes) {
                l_Sizes[l_Idx] = l_CurrentBlock->GetSize();
                l_Counts[l_Idx] = 0;
                l_NumSizes++;
            }

            l_Counts[l_Idx]++;
        }

        l_CurrentBlock = l_CurrentBlock->GetNext();
    }

    l_Sorted = FALSE;

    while (!l_Sorted) {
        l_Sorted = TRUE;

        for (U32 i = 0; i < l_NumSizes - 1; i++) {
            if (l_Sizes[i] < l_Sizes[i + 1]) {
                l_Sorted = s_SwapMallocStat(
                    l_Sizes[i], l_Sizes[i + 1], l_Counts[i], l_Counts[i + 1]
                );
            }
        }
    }

    for (U32 i = 0; i < l_NumSizes && i < 50; i++) {
    }
}

#pragma warn_uninitializedvar on

// TODO: Finish matching
U32 s_getnbfindalloc(void* i_Start, void* i_End) {
    U32 l_Count = 0;

    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);

    if (!l_Block) {
        return 0;
    }

    if (!l_Block->GetFree()) {
        if ((U32)i_End >= (U32)l_Block) {
            if ((U32)i_Start <= l_Block->GetBlockEnd()) {
                l_Count = 1;
            }
        }
    }

    s_MEM_BLOCK* l_PrevBlock = l_Block->GetPrev();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            if ((U32)i_End >= (U32)l_PrevBlock) {
                if ((U32)i_Start <= l_PrevBlock->GetBlockEnd()) {
                    l_Count++;
                }
            }
        }

        l_PrevBlock = l_PrevBlock->GetPrev();
    }

    l_PrevBlock = l_Block->GetNext();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            if ((U32)i_End >= (U32)l_PrevBlock) {
                if ((U32)i_Start <= l_PrevBlock->GetBlockEnd()) {
                    l_Count++;
                }
            }
        }

        l_PrevBlock = l_PrevBlock->GetNext();
    }

    return l_Count;
}

void s_getfindalloc(S32 i_Index, Char* i_Buffer, void* i_Start, void* i_End) {
    S32 l_CurrentIndex = 0;
    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);

    if (!l_Block) {
        return;
    }

    if (!l_Block->GetFree()) {
        if (i_End >= l_Block) {
            if (i_Start <= (void*)l_Block->GetBlockEnd()) {
                if (i_Index == l_CurrentIndex) {
                    U32 l_Size = l_Block->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_Block->GetAlignDecal();
                    sprintf(i_Buffer, "| Size : %d/%d |\n", l_Size, l_Block->GetSize());
                    return;
                }

                l_CurrentIndex++;
            }
        }
    }

    s_MEM_BLOCK* l_PrevBlock = l_Block->GetPrev();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            if (i_End >= l_PrevBlock) {
                if (i_Start <= (void*)l_PrevBlock->GetBlockEnd()) {
                    if (i_Index == l_CurrentIndex) {
                        U32 l_Size = l_PrevBlock->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_PrevBlock->GetAlignDecal();
                        sprintf(i_Buffer, "| Size : %d/%d |\n", l_Size, l_PrevBlock->GetSize());
                        return;
                    }

                    l_CurrentIndex++;
                }
            }
        }

        l_PrevBlock = l_PrevBlock->GetPrev();
    }

    l_PrevBlock = l_Block->GetNext();

    while (l_PrevBlock) {
        if (!l_PrevBlock->GetFree()) {
            if (i_End >= l_PrevBlock) {
                if (i_Start <= (void*)l_PrevBlock->GetBlockEnd()) {
                    if (i_Index == l_CurrentIndex) {
                        U32 l_Size = l_PrevBlock->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_PrevBlock->GetAlignDecal();
                        sprintf(i_Buffer, "| Size : %d/%d |\n", l_Size, l_PrevBlock->GetSize());
                        return;
                    }

                    l_CurrentIndex++;
                }
            }
        }

        l_PrevBlock = l_PrevBlock->GetNext();
    }
}

void s_findalloc(void* i_Start, void* i_End) {
    S32 l_Count = s_getnbfindalloc(i_Start, i_End);

    Char l_Buffer[520];

    for (S32 i = 0; i < l_Count; i++) {
        s_getfindalloc(i, l_Buffer, i_Start, i_End);
    }
}

Bool s_VerifyMem() {
    if (!FreeBuckets) {
        return TRUE;
    }

    s_MEM_BLOCK* l_Block = FreeBuckets->FindBlock(0);

    if (l_Block) {
        return s_TestIntegrity(l_Block);
    }

    return FALSE;
}

U32 s_GetFreeMem() {
    if (FreeBuckets) {
        return FreeBuckets->GetFreeMem();
    }

    return 0;
}

U32 s_GetNbFreeBlocks() {
    if (FreeBuckets) {
        return FreeBuckets->GetNbFreeBlocks();
    }

    return 0;
}

U32 s_GetLargestFree() {
    if (FreeBuckets) {
        return FreeBuckets->GetLargestFreeBlock();
    }

    return 0;
}

S32 s_Dicho_RecEqual(s_MEM_BLOCK** i_Track, U32 i_Size, S32 i_Min, S32 i_Max) {
    if (i_Min == i_Max) {
        if (i_Track[i_Min]->GetSize() == i_Size) {
            return i_Min;
        }
        return -1;
    }

    if (i_Min == i_Max - 1) {
        if (i_Track[i_Max]->GetSize() == i_Size) {
            return i_Max;
        }
        if (i_Track[i_Min]->GetSize() == i_Size) {
            return i_Min;
        }
        return -1;
    }

    U32 l_EndOfMemory = EndOfMemory;

    while (TRUE) {
        S32 l_Mid = (i_Min + i_Max) >> 1;
        U32 l_MidSize = i_Track[l_Mid]->GetSize(l_EndOfMemory);

        if (i_Size == l_MidSize) {
            return l_Mid;
        }

        if (i_Size > l_MidSize) {
            i_Max = l_Mid;
            if (i_Min == l_Mid - 1) {
                if (i_Track[i_Min]->GetSize(l_EndOfMemory) == i_Size) {
                    return i_Min;
                }
                return -1;
            }
        }
        else {
            i_Min = l_Mid;
            if (l_Mid == i_Max - 1) {
                if (i_Track[i_Max]->GetSize(l_EndOfMemory) == i_Size) {
                    return i_Max;
                }
                return -1;
            }
        }
    }
}

U32 s_Dicho_RecAfter(s_MEM_BLOCK** i_Track, U32 i_Size, S32 i_Min, S32 i_Max) {
    if (i_Min == i_Max) {
        if (i_Size <= i_Track[i_Min]->GetSize()) {
            return i_Min + 1;
        }
        return i_Min;
    }

    if (i_Min == i_Max - 1) {
        if (i_Size <= i_Track[i_Max]->GetSize()) {
            return i_Max + 1;
        }
        if (i_Size <= i_Track[i_Min]->GetSize()) {
            return i_Min + 1;
        }
        return i_Min;
    }

    U32 l_EndOfMemory = EndOfMemory;

    while (TRUE) {
        S32 l_Mid = (i_Min + i_Max) >> 1;
        U32 l_MidSize = i_Track[l_Mid]->GetSize(l_EndOfMemory);

        if (i_Size == l_MidSize) {
            if (i_Size > i_Track[l_Mid + 1]->GetSize(l_EndOfMemory)) {
                return l_Mid + 1;
            }
            i_Min = l_Mid;
            if (l_Mid == i_Max - 1) {
                if (i_Size <= i_Track[i_Max]->GetSize(l_EndOfMemory)) {
                    return i_Max + 1;
                }
                return i_Max;
            }
        }
        else if (i_Size > l_MidSize) {
            i_Max = l_Mid;
            if (i_Min == l_Mid - 1) {
                if (i_Size <= i_Track[i_Min]->GetSize(l_EndOfMemory)) {
                    return i_Min + 1;
                }
                return i_Min;
            }
        }
        else {
            i_Min = l_Mid;
            if (l_Mid == i_Max - 1) {
                if (i_Size <= i_Track[i_Max]->GetSize(l_EndOfMemory)) {
                    return i_Max + 1;
                }
                return i_Max;
            }
        }
    }
}

S32 s_Dicho_RecBigger(s_MEM_BLOCK** i_Track, U32 i_Size, S32 i_Min, S32 i_Max) {
    if (i_Min == i_Max) {
        if (i_Size <= i_Track[i_Min]->GetSize()) {
            return i_Min;
        }
        return -1;
    }

    if (i_Min == i_Max - 1) {
        if (i_Size <= i_Track[i_Max]->GetSize()) {
            return i_Max;
        }
        if (i_Size <= i_Track[i_Min]->GetSize()) {
            return i_Min;
        }
        return -1;
    }

    U32 l_EndOfMemory = EndOfMemory;

    while (TRUE) {
        S32 l_Mid = (i_Min + i_Max) >> 1;
        U32 l_MidSize = i_Track[l_Mid]->GetSize(l_EndOfMemory);

        if (i_Size == l_MidSize) {
            if (i_Size > i_Track[l_Mid + 1]->GetSize(l_EndOfMemory)) {
                return l_Mid;
            }
            i_Min = l_Mid;
            if (l_Mid == i_Max - 1) {
                if (i_Size <= i_Track[i_Max]->GetSize(l_EndOfMemory)) {
                    return i_Max;
                }
                return l_Mid;
            }
        }
        else if (i_Size > l_MidSize) {
            i_Max = l_Mid;
            if (i_Min == l_Mid - 1) {
                if (i_Size <= i_Track[i_Min]->GetSize(l_EndOfMemory)) {
                    return i_Min;
                }
                return -1;
            }
        }
        else {
            i_Min = l_Mid;
            if (l_Mid == i_Max - 1) {
                if (i_Size <= i_Track[i_Max]->GetSize(l_EndOfMemory)) {
                    return i_Max;
                }
                return l_Mid;
            }
        }
    }
}

Bool s_QuickTestIntegrity(s_MEM_BLOCK* i_Block) {
    if (!i_Block) {
        return FALSE;
    }

    if (!i_Block->IsValid()) {
        return FALSE;
    }

    if (i_Block->GetPrev() && !i_Block->GetPrev()->IsValid()) {
        return FALSE;
    }

    if (i_Block->GetNext() && !i_Block->GetNext()->IsValid()) {
        return FALSE;
    }

    if (i_Block->GetPrev() && i_Block->GetPrev()->GetNext() != i_Block) {
        return FALSE;
    }

    if (i_Block->GetNext() && i_Block->GetNext()->GetPrev() != i_Block) {
        return FALSE;
    }

    return TRUE;
}

Bool s_TestIntegrity(s_MEM_BLOCK* i_Block) {
    if (!s_QuickTestIntegrity(i_Block)) {
        return FALSE;
    }

    if (!FreeBuckets->TestIntegrity()) {
        return FALSE;
    }

    if (i_Block->GetFree()) {
        if (FreeBuckets->s_GetInBucket(FreeBuckets->FindBucket(i_Block->GetSize()), i_Block) == -1) {
            return FALSE;
        }
    }
    else if (FreeBuckets->s_GetInBucket(FreeBuckets->FindBucket(i_Block->GetSize()), i_Block) != -1) {
        return FALSE;
    }

    s_MEM_BLOCK* l_CurrentBlock = i_Block->GetPrev();

    while (l_CurrentBlock) {
        if (!s_QuickTestIntegrity(l_CurrentBlock)) {
            return FALSE;
        }

        if (l_CurrentBlock->GetFree()) {
            s_FREE_MEM_BUCKETS* l_FreeBuckets = FreeBuckets;
            if (l_FreeBuckets->s_GetInBucket(l_FreeBuckets->FindBucket(l_CurrentBlock->GetSize()), l_CurrentBlock) == -1) {
                return FALSE;
            }
        }
        else {
            s_FREE_MEM_BUCKETS* l_FreeBuckets = FreeBuckets;
            if (l_FreeBuckets->s_GetInBucket(l_FreeBuckets->FindBucket(l_CurrentBlock->GetSize()), l_CurrentBlock) != -1) {
                return FALSE;
            }
        }

        s_MEM_BLOCK* l_PrevBlock = l_CurrentBlock->GetPrev();

        if (l_PrevBlock >= l_CurrentBlock) {
            return FALSE;
        }

        l_CurrentBlock = l_PrevBlock;
    }

    l_CurrentBlock = i_Block->GetNext();

    while (l_CurrentBlock) {
        if (!s_QuickTestIntegrity(l_CurrentBlock)) {
            return FALSE;
        }

        if (l_CurrentBlock->GetFree()) {
            s_FREE_MEM_BUCKETS* l_FreeBuckets = FreeBuckets;
            if (l_FreeBuckets->s_GetInBucket(l_FreeBuckets->FindBucket(l_CurrentBlock->GetSize()), l_CurrentBlock) == -1) {
                return FALSE;
            }
        }
        else {
            s_FREE_MEM_BUCKETS* l_FreeBuckets = FreeBuckets;
            if (l_FreeBuckets->s_GetInBucket(l_FreeBuckets->FindBucket(l_CurrentBlock->GetSize()), l_CurrentBlock) != -1) {
                return FALSE;
            }
        }

        s_MEM_BLOCK* l_NextBlock = l_CurrentBlock->GetNext();

        if (l_NextBlock && l_NextBlock <= l_CurrentBlock) {
            return FALSE;
        }

        l_CurrentBlock = l_NextBlock;
    }

    return TRUE;
}

void s_SplitToAllocAndFreeBlock(s_MEM_BLOCK* i_Block, U32 i_Size) {
    s_MEM_BLOCK* l_NewBlock = (s_MEM_BLOCK*)((U32)i_Block + i_Size);
    s_MEM_BLOCK* l_NextBlock = i_Block->GetNext();

    if (l_NextBlock && l_NextBlock->GetFree()) {
        if (i_Block->GetSize() <= i_Size) {
            return;
        }

        s_MEM_BLOCK l_NextBlockCopy = *l_NextBlock;
        FreeBuckets->RemoveBlock(l_NextBlock);

        i_Block->SetNext(l_NewBlock);
        l_NewBlock->SetPrev(i_Block);

        l_NewBlock->SetNext(l_NextBlockCopy.GetNext());

        if (l_NextBlockCopy.GetNext()) {
            l_NextBlockCopy.GetNext()->SetPrev(l_NewBlock);
        }

        l_NewBlock->SetAlign(0);
        FreeBuckets->AddBlock(l_NewBlock);
        return;
    }

    if (i_Block->GetSize() < i_Size + 0x20) {
        return;
    }

    i_Block->AddAfter(l_NewBlock);
    l_NewBlock->SetAlign(0);
    FreeBuckets->AddBlock(l_NewBlock);
}

void s_free(void* i_Ptr) {
    if (!i_Ptr) {
        return;
    }

    s_MEM_BLOCK* l_Block = (s_MEM_BLOCK*)s_MEM_BLOCK::GetRealPtr(i_Ptr);

    RealAllocatedMem -= l_Block->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_Block->GetAlignDecal();

    ASSERTLE_Z(s_QuickTestIntegrity(l_Block), "Memory error: Free of an invalid block !", 1412, "s_QuickTestIntegrity(block)");

    l_Block->GetFree();

    s_MEM_BLOCK* l_PrevBlock = l_Block->GetPrev();

    if (l_PrevBlock && l_PrevBlock->GetFree()) {
        l_Block = l_PrevBlock;
        FreeBuckets->RemoveBlock(l_Block);
        l_Block->MergeNext();
    }

    s_MEM_BLOCK* l_NextBlock = l_Block->GetNext();

    if (l_NextBlock && l_NextBlock->GetFree()) {
        FreeBuckets->RemoveBlock(l_NextBlock);
        l_Block->MergeNext();
    }

    FreeBuckets->AddBlock(l_Block);
    FreeBuckets->Extend();
}

void* s_malloc(U32 i_Size, U32 i_Alignment) {
    s_FREE_MEM_BUCKETS* l_FreeBuckets;
    const U32 l_AlignedSize = (i_Size + 3) & ~3;
    const U32 l_AlignedAlignment = (i_Alignment + 3) & ~3;
    const U32 l_BlockMinSize = l_AlignedSize + 4 + l_AlignedAlignment;

    l_FreeBuckets = FreeBuckets;

    s_MEM_BLOCK* l_Block = l_FreeBuckets->FindBlock(l_BlockMinSize);

    if (!l_Block) {
        return NULL;
    }

    ASSERTLE_Z(s_QuickTestIntegrity(l_Block), "Memory Integrity Error (Alloc) !", 1462, "s_QuickTestIntegrity(block)");

    l_Block->SetAlign(l_AlignedAlignment);

    void* l_Result = l_Block + 1;
    U32 l_AlignDecal = l_Block->GetAlignDecal();

    if (l_AlignDecal) {
        l_Result = (void*)((U32)l_Result + l_AlignDecal);
        *((U32*)l_Result - 1) = l_AlignDecal;
    }

    l_FreeBuckets = FreeBuckets;
    l_FreeBuckets->RemoveBlock(l_Block);
    s_SplitToAllocAndFreeBlock(
        l_Block, l_AlignedSize + l_AlignDecal + s_MEM_BLOCK_HEADER_SIZE
    );
    l_Block->SetMarked(FALSE);
    l_FreeBuckets = FreeBuckets;
    l_FreeBuckets->Extend();

    RealAllocatedMem += l_AlignedSize;
    return l_Result;
}

static inline U32 s_GetEndAllocMinSize(U32 i_Size, U32 i_Alignment) {
    i_Size += i_Alignment;
    i_Size += 4;
    return i_Size;
}

void* s_malloc_end(U32 i_Size, U32 i_Alignment) {
    s_FREE_MEM_BUCKETS* l_FreeBuckets = FreeBuckets;
    s_MEM_BLOCK* l_Block;
    s_MEM_BLOCK* l_AllocatedBlock;
    U32 l_Result;
    U32 l_AlignedSize = (i_Size + 3) & ~3;
    U32 l_AlignedAlignment = (i_Alignment + 3) & ~3;
    U32 l_BlockMinSize = l_AlignedSize + 4 + l_AlignedAlignment;

    l_Block = FreeBuckets->FindLastBlock();
    l_AllocatedBlock = l_Block;

    if (!l_Block) {
        return NULL;
    }

    U32 l_BlockSize = l_Block->GetSize();

    if (l_BlockSize < l_BlockMinSize) {
        return NULL;
    }

    ASSERTLE_Z(s_QuickTestIntegrity(l_Block), "Memory Integrity Error (Alloc End)!", 1537, "s_QuickTestIntegrity(block)");

    l_FreeBuckets = FreeBuckets;
    l_FreeBuckets->RemoveBlock(l_Block);
    if (l_BlockSize > l_BlockMinSize + 0x20) {
        l_Result = l_Block->GetBlockEnd() - l_AlignedSize;
        l_Result -= l_Result & (l_AlignedAlignment - 1);
        l_AllocatedBlock = (s_MEM_BLOCK*)(l_Result - s_MEM_BLOCK_HEADER_SIZE);

        l_AllocatedBlock->SetAlign(l_AlignedAlignment);
        l_AllocatedBlock->m_BlockFlags.m_IsFree = FALSE;
        l_Block->AddAfter(l_AllocatedBlock);
        l_FreeBuckets = FreeBuckets;
        l_FreeBuckets->AddBlock(l_Block);
    }
    else {
        l_Block->SetAlign(l_AlignedAlignment);
        l_Result = (U32)(l_Block + 1);

        U32 l_AlignDecal = l_Block->GetAlignDecal();
        if (l_AlignDecal) {
            l_Result += l_AlignDecal;
            *((U32*)l_Result - 1) = l_AlignDecal;
        }
    }

    l_AllocatedBlock->SetMarked(FALSE);
    l_FreeBuckets = FreeBuckets;
    l_FreeBuckets->Extend();
    RealAllocatedMem += l_AlignedSize;

    return (void*)l_Result;
}

void* s_fullrealloc(void* i_Ptr, U32 i_NewSize) {
    if (!i_Ptr) {
        return NULL;
    }

    s_MEM_BLOCK* l_Block = (s_MEM_BLOCK*)s_MEM_BLOCK::GetRealPtr(i_Ptr);
    void* l_NewPtr;
    U32 l_AlignedSize = (i_NewSize + 3) & ~3;
    l_NewPtr = s_malloc(l_AlignedSize, l_Block->GetAlignment());

    if (l_NewPtr) {
        U32 l_CopySize = l_Block->GetSize() - s_MEM_BLOCK_HEADER_SIZE - l_Block->GetAlignDecal();

        if (l_CopySize > l_AlignedSize) {
            l_CopySize = l_AlignedSize;
        }

        Sys_Z::MemCpyFrom(l_NewPtr, i_Ptr, l_CopySize);
        s_free(i_Ptr);
    }

    return l_NewPtr;
}

static inline s_MEM_BLOCK* s_GetReallocBlock(void* i_Ptr) {
    s_MEM_BLOCK* l_Block = (s_MEM_BLOCK*)s_MEM_BLOCK::GetRealPtr(i_Ptr);
    return l_Block;
}

void* s_mergewithnextrealloc(void* i_Ptr, U32 i_NewSize) {
    s_MEM_BLOCK* l_Block;
    s_MEM_BLOCK* l_NextBlock;
    l_Block = s_GetReallocBlock(i_Ptr);
    l_NextBlock = l_Block->GetNext();

    if (!l_NextBlock) {
        return NULL;
    }

    if (!l_NextBlock->GetFree()) {
        return NULL;
    }

    if (l_Block->GetSize() + l_NextBlock->GetSize() >= i_NewSize) {
        FreeBuckets->RemoveBlock(l_NextBlock);
        l_Block->MergeNext();
        s_SplitToAllocAndFreeBlock(l_Block, i_NewSize);
        FreeBuckets->Extend();

        return i_Ptr;
    }

    return NULL;
}

void* s_realloc(void* i_Ptr, U32 i_NewSize) {
    s_MEM_BLOCK* l_Block;
    void* l_Result;
    U32 l_AlignedSize;
    U32 l_CurrentBlockSize;
    U32 l_BlockSize;

    if (!i_Ptr) {
        return NULL;
    }

    l_Block = (s_MEM_BLOCK*)s_MEM_BLOCK::GetRealPtr(i_Ptr);

    ASSERTLE_Z(s_QuickTestIntegrity(l_Block), "Memory Integrity Error (Realloc) !", 1659, "s_QuickTestIntegrity(block)");

    l_AlignedSize = (i_NewSize + 3) & ~3;
    l_Result = s_fullrealloc(i_Ptr, l_AlignedSize);

    if (l_Result) {
        return l_Result;
    }

    l_Block->GetReallocBlockSizes(l_AlignedSize, l_BlockSize, l_CurrentBlockSize);

    if (l_BlockSize > l_CurrentBlockSize) {
        l_Result = s_mergewithnextrealloc(i_Ptr, l_BlockSize);

        if (l_Result) {
            return l_Result;
        }

        return s_fullrealloc(i_Ptr, l_AlignedSize);
    }

    RealAllocatedMem -= l_CurrentBlockSize;
    RealAllocatedMem += l_AlignedSize;
    s_SplitToAllocAndFreeBlock(l_Block, l_BlockSize);
    l_Block->SetMarked(FALSE);
    FreeBuckets->Extend();

    return i_Ptr;
}
