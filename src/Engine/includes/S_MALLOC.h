#ifndef _S_MALLOC_H_
#define _S_MALLOC_H_

#include "Types_Z.h"

//---- Constants and macros ----//

#define s_MEM_BUCKET_COUNT 10
#define s_MEM_BUCKET_INITIAL_BLOCKS 16

// Encoded block address/size fields are biased by this value.
// A stored value of 0x400 decodes to zero, which is used as a sentinel in some cases.
#define s_MEM_BLOCK_ENCODE_BIAS 0x400

// Allocation block headers are 8 bytes: two encoded 32-bit words.
#define s_MEM_BLOCK_HEADER_SIZE 8

struct s_MEM_BLOCK;
struct s_FREE_MEM_BUCKETS;

//---- Function declarations ----//

void s_Init(void* i_StartOfMemory, U32 i_TotalMemory);
void s_MarkMem(U32);
U32 s_ShowUnMarkedMem();
U32 s_GetAllocatedMem();
void s_ShowMostNbMalloc();
U32 s_getnbfindalloc(void*, void*);
void s_getfindalloc(S32, Char*, void*, void*);
void s_findalloc(void* i_Start, void* i_End);
Bool s_VerifyMem();
U32 s_GetFreeMem();
U32 s_GetNbFreeBlocks();
U32 s_GetLargestFree();
S32 s_Dicho_RecEqual(s_MEM_BLOCK**, U32, S32, S32);
U32 s_Dicho_RecAfter(s_MEM_BLOCK**, U32, S32, S32);
S32 s_Dicho_RecBigger(s_MEM_BLOCK**, U32, S32, S32);
Bool s_QuickTestIntegrity(s_MEM_BLOCK*);
Bool s_TestIntegrity(s_MEM_BLOCK*);
void s_SplitToAllocAndFreeBlock(s_MEM_BLOCK*, U32);
void s_free(void* i_Ptr);
void* s_malloc(U32 i_Size, U32 i_Alignment);
void* s_malloc_end(U32 i_Size, U32 i_Alignment);
void* s_fullrealloc(void* i_Ptr, U32 i_NewSize);
void* s_mergewithnextrealloc(void* i_Ptr, U32 i_NewSize);
void* s_realloc(void* i_Ptr, U32 i_NewSize);

//---- Global variables ----//

// Allocator diagnostics.
U32 MAXnbFreeBlocks;
U32 RealAllocatedMem;

// Arena bounds.
void* StartOfMemory;
U32 EndOfMemory;

// Allocator state.
Bool IsReallocating;
// Set when a bucket is close to running out of tracking slots.
Bool NeedToExtend;
Bool HasAllocatedFreeBlock;

// Address inside the arena used as a threshold for allocation strategy.
// It is initialized around 4/5 into the arena, aligned down to 8 bytes.
U32 BestFitLimit;

// Active free-list manager and backing table for bucket entries.
s_FREE_MEM_BUCKETS* FreeBuckets;

// Unreferenced variable, present in the original object between FreeBuckets
// and TMPmem. Its real name and type are unknown (only size is).
static U32 s_UnusedGlobal;

s_MEM_BLOCK* TMPmem[s_MEM_BUCKET_COUNT * s_MEM_BUCKET_INITIAL_BLOCKS];

//---- Structure definitions ----//

struct s_MEM_BLOCK {
    union {
        U32 m_BlockFlagsRaw;

        struct {
            // Top bit of the first byte. Set when the block is in a free bucket.
            U32 m_IsFree : 1;

            // Is it marked
            U32 m_IsMarked : 1;

            // Encoded block metadata value. It uses the same 0x400 bias scheme.
            U32 m_BlockValue : 30;
        } m_BlockFlags;
    };

    union {
        U32 m_EncodedSizeRaw;

        struct {
            // Small encoded size-class/index field.
            U32 m_SizeBits : 3;

            // Encoded maximum-address field.
            // Decoded as: (m_SizeValue - 0x400) << 2.
            U32 m_SizeValue : 29;
        } m_EncodedSize;
    };

    void Init() {
        // Initialize the root block with the neutral encoded value.
        // For m_EncodedSize, this makes GetMaxAddress() resolve to EndOfMemory.
        m_BlockFlags.m_BlockValue = s_MEM_BLOCK_ENCODE_BIAS;
        m_EncodedSize.m_SizeValue = s_MEM_BLOCK_ENCODE_BIAS;

        // Root block starts in the base size class.
        SetSizeBitsFromMask(1);

        // The root block initially represents all free arena memory.
        m_BlockFlags.m_IsFree = TRUE;
    } // Fully inlined

    U32 GetMaxAddress() {
        s_MEM_BLOCK* l_NextBlock = GetNext();
        if (!l_NextBlock) {
            l_NextBlock = (s_MEM_BLOCK*)EndOfMemory;
        }
        return (U32)l_NextBlock;
    } // Fully inlined

    Bool IsValid() {
        Bool l_IsValid;

        if ((U32)this < (U32)StartOfMemory) {
            l_IsValid = FALSE;
        }
        else if ((U32)this >= EndOfMemory) {
            l_IsValid = FALSE;
        }
        else {
            l_IsValid = TRUE;
        }

        return l_IsValid;
    } // Fully inlined

    void SetSizeBitsFromMask(U32 i_Mask) {
        m_EncodedSize.m_SizeBits = 0;

        while (m_EncodedSize.m_SizeBits < 0x20) {
            if ((1 << m_EncodedSize.m_SizeBits) & i_Mask) {
                break;
            }

            m_EncodedSize.m_SizeBits++;
        }
    } // Fully inlined

    s_MEM_BLOCK* GetNext() {
        s_MEM_BLOCK* l_NextBlock = (s_MEM_BLOCK*)((m_EncodedSize.m_SizeValue - s_MEM_BLOCK_ENCODE_BIAS) << 2);
        if (l_NextBlock) {
            l_NextBlock = (s_MEM_BLOCK*)((U32)l_NextBlock | 0x80000000);
        }
        return l_NextBlock;
    } // Matched 100%

    void MergeNext() {
        s_MEM_BLOCK* l_NextBlock = GetNext();

        if (!l_NextBlock) {
            return;
        }

        s_MEM_BLOCK* l_NextNextBlock = l_NextBlock->GetNext();
        SetNext(l_NextNextBlock);

        if (l_NextNextBlock) {
            l_NextNextBlock->SetPrev(this);
        }
    } // Matched 100%

    s_MEM_BLOCK* GetPrev() {
        return (s_MEM_BLOCK*)((m_BlockFlags.m_BlockValue - s_MEM_BLOCK_ENCODE_BIAS) << 2);
    } // Matched 100%

    U32 GetFree() {
        return m_BlockFlags.m_IsFree;
    } // Matched 100%

    U32 GetMarked() {
        return m_BlockFlags.m_IsMarked;
    } // Fully inlined

    U32 GetAlignDecal() {
        U32 l_Align = 1 << (m_EncodedSize.m_SizeBits + 2);
        U32 l_Address = (U32)this + s_MEM_BLOCK_HEADER_SIZE;
        U32 l_Decal = l_Address & (l_Align - 1);

        if (l_Decal) {
            return l_Align - l_Decal;
        }

        return 0;
    } // Matched 100%

    U32 GetAlignment() {
        return 1 << (m_EncodedSize.m_SizeBits + 2);
    } // Fully inlined

    void GetReallocBlockSizes(U32 i_Size, U32& o_BlockSize, U32& o_CurrentSize) {
        i_Size += s_MEM_BLOCK_HEADER_SIZE;
        o_BlockSize = i_Size + GetAlignDecal();
        o_CurrentSize = GetSize();
    } // Fully inlined

    U32 GetSize() {
        s_MEM_BLOCK* l_NextBlock = GetNext();

        if (!l_NextBlock) {
            l_NextBlock = (s_MEM_BLOCK*)EndOfMemory;
        }

        return (U32)l_NextBlock - (U32)this;
    } // Matched 100%

    U32 GetBlockEnd() {
        U32 l_BlockEnd = (U32)this;
        l_BlockEnd += GetSize();
        return l_BlockEnd;
    } // Fully inlined

    U32 GetMaxAddress(U32 i_EndOfMemory) {
        s_MEM_BLOCK* l_NextBlock = GetNext();
        if (!l_NextBlock) {
            l_NextBlock = (s_MEM_BLOCK*)i_EndOfMemory;
        }
        return (U32)l_NextBlock;
    } // Fully inlined

    U32 GetSize(U32 i_EndOfMemory) {
        return GetMaxAddress(i_EndOfMemory) - (U32)this;
    } // Fully inlined

    static void* GetRealPtr(void* i_Ptr) {
        U32 l_Decal = *((U32*)i_Ptr - 1);

        if (l_Decal >= s_MEM_BLOCK_ENCODE_BIAS) {
            return (void*)((U32)i_Ptr - s_MEM_BLOCK_HEADER_SIZE);
        }

        return (void*)((U32)i_Ptr - l_Decal - s_MEM_BLOCK_HEADER_SIZE);
    } // Matched 100%

    void SetMarked(U32 i_Mark) {
        m_BlockFlags.m_IsMarked = i_Mark;
    } // Matched 100%

    void AddAfter(s_MEM_BLOCK* i_Block) {
        if (!i_Block) {
            return;
        }

        i_Block->SetPrev(this);

        s_MEM_BLOCK* l_NextBlock = GetNext();

        i_Block->SetNext(l_NextBlock);

        if (l_NextBlock) {
            l_NextBlock->SetPrev(i_Block);
        }

        SetNext(i_Block);
    } // Matched 100%

    void SetAlign(U32 i_Align) {
        U32 l_AlignMask = (i_Align + 3) >> 2;

        if (!l_AlignMask) {
            l_AlignMask = 1;
        }

        m_EncodedSize.m_SizeBits = 0;

        while (m_EncodedSize.m_SizeBits < 0x20) {
            U32 l_Bits = m_EncodedSize.m_SizeBits;

            if (l_AlignMask & (1 << l_Bits)) {
                break;
            }

            m_EncodedSize.m_SizeBits = l_Bits + 1;
        }
    } // Matched 100%

    void SetPrev(s_MEM_BLOCK* i_Block) {
        m_BlockFlags.m_BlockValue = ((U32)i_Block >> 2) + s_MEM_BLOCK_ENCODE_BIAS;
    } // Matched 100%

    void SetNext(s_MEM_BLOCK* i_Block) {
        m_EncodedSize.m_SizeValue = ((U32)i_Block >> 2) + s_MEM_BLOCK_ENCODE_BIAS;
    } // Matched 100%
};

struct s_FREE_MEM_BUCKETS {
    // Flat table storing all bucket entries.
    // Bucket N starts at m_BucketBlocks + (m_BucketTotalBlocks * N).
    s_MEM_BLOCK** m_BucketBlocks;

    // Number of tracking slots available per bucket.
    S32 m_BucketTotalBlocks;

    // Number of blocks currently stored in each bucket.
    S32 m_BucketFreeBlocks[s_MEM_BUCKET_COUNT];

    // Initial free block. It is embedded in the bucket manager at the arena start.
    s_MEM_BLOCK m_RootBlock;

    void Init() {
        m_BucketTotalBlocks = s_MEM_BUCKET_INITIAL_BLOCKS;
        m_BucketBlocks = TMPmem;

        for (S32 i = 0; i < s_MEM_BUCKET_COUNT; i++) {
            m_BucketFreeBlocks[i] = 0;
        }
    } // Fully inlined

    S32 FindBucket(S32 i_MinSize) {
        i_MinSize -= s_MEM_BLOCK_HEADER_SIZE;

        S32 l_BucketIndex;

        // Large blocks start in a higher bucket range.
        if (i_MinSize & 0xFFFF0000) {
            l_BucketIndex = 12;

            if (l_BucketIndex >= s_MEM_BUCKET_COUNT - 1) {
                return s_MEM_BUCKET_COUNT - 1;
            }

            i_MinSize = (S32)i_MinSize >> 16;
        }
        else {
            l_BucketIndex = -4;
        }

        // Classify the size by finding its approximate highest set bit.
        if (i_MinSize & 0xFF00FF00) {
            i_MinSize = (S32)i_MinSize >> 8;
            l_BucketIndex += 8;
        }

        if (i_MinSize & 0xF0F0F0F0) {
            i_MinSize = (S32)i_MinSize >> 4;
            l_BucketIndex += 4;
        }

        if (i_MinSize & 0xCCCCCCCC) {
            i_MinSize = (S32)i_MinSize >> 2;
            l_BucketIndex += 2;
        }

        if (i_MinSize & 0xAAAAAAAA) {
            l_BucketIndex += 1;
        }

        if (l_BucketIndex < 0) {
            l_BucketIndex = 0;
        }

        if (l_BucketIndex >= s_MEM_BUCKET_COUNT) {
            return s_MEM_BUCKET_COUNT - 1;
        }

        return l_BucketIndex;
    } // Matched 100%

    void AddBlockToBucket(S32 i_BucketIdx, s_MEM_BLOCK* i_Block) {
        S32 l_Count = m_BucketFreeBlocks[i_BucketIdx];
        s_MEM_BLOCK** l_BucketTrack;

        // Ask for bucket expansion if this bucket is nearly full.
        if (l_Count >= (m_BucketTotalBlocks - 6)) {
            NeedToExtend = TRUE;
        }

        // Locate this bucket's slice in the flat block table.
        l_BucketTrack = m_BucketBlocks + (m_BucketTotalBlocks * i_BucketIdx);

        S32 l_InsertIdx;

        // Keep each bucket sorted by block span.
        if (l_Count) {
            l_InsertIdx = s_Dicho_RecAfter(l_BucketTrack, i_Block->GetSize(), 0, l_Count - 1);
        }
        else {
            l_InsertIdx = 0;
        }

        // Make room for the new block if it belongs before existing entries.
        if (l_InsertIdx < l_Count) {
            s_MEM_BLOCK** l_Current = l_BucketTrack + (l_Count - 1);
            s_MEM_BLOCK** l_Target = l_BucketTrack + l_InsertIdx;

            while (l_Current >= l_Target) {
                l_Current[1] = *l_Current;
                l_Current--;
            }
        }

        l_BucketTrack[l_InsertIdx] = i_Block;

        // A block in a free bucket is marked free.
        i_Block->m_BlockFlags.m_IsFree = TRUE;

        m_BucketFreeBlocks[i_BucketIdx]++;
    } // Fully inlined

    S32 FindFreeBucket(S32 i_MinSize) {
        S32 l_Idx = FindBucket(i_MinSize);

        for (; l_Idx < s_MEM_BUCKET_COUNT; l_Idx++) {
            if (m_BucketFreeBlocks[l_Idx]) {
                return l_Idx;
            }
        }
        return -1;
    } // Fully inlined

    s_MEM_BLOCK* FindBestBlock(S32 i_MinSize) {
        S32 l_BucketIdx = FindFreeBucket(i_MinSize);

        if (l_BucketIdx >= 0) {
            for (; l_BucketIdx < s_MEM_BUCKET_COUNT; l_BucketIdx++) {
                s_MEM_BLOCK* l_Block = FindBlock(l_BucketIdx, i_MinSize);

                if (l_Block) {
                    return l_Block;
                }
            }
        }

        return NULL;
    } // Fully inlined

    s_MEM_BLOCK* FindFirstBlock(S32 i_MinSize) {
        s_MEM_BLOCK* l_Block = NULL;

        S32 l_BucketIdx = FindFreeBucket(i_MinSize);

        if (l_BucketIdx >= 0) {
            while (l_BucketIdx < s_MEM_BUCKET_COUNT) {
                S32 l_Count = m_BucketFreeBlocks[l_BucketIdx];

                s_MEM_BLOCK** l_BucketTrack = m_BucketBlocks + (m_BucketTotalBlocks * l_BucketIdx);
                s_MEM_BLOCK** l_Cur = l_BucketTrack;
                s_MEM_BLOCK** l_End = l_BucketTrack + l_Count;

                while (l_Cur < l_End) {
                    s_MEM_BLOCK* l_CurBlock = *l_Cur;

                    if (l_CurBlock->GetSize() >= (U32)i_MinSize) {
                        if (!l_Block || l_CurBlock < l_Block) {
                            l_Block = l_CurBlock;
                        }
                    }

                    l_Cur++;
                }

                l_BucketIdx++;
            }
        }

        return l_Block;
    } // Fully inlined

    s_MEM_BLOCK* FindLastBlock() {
        for (S32 i = s_MEM_BUCKET_COUNT - 1; i >= 0; i--) {
            if (m_BucketFreeBlocks[i]) {
                return m_BucketBlocks[m_BucketTotalBlocks * i];
            }
        }

        return NULL;
    } // Fully inlined

    S32 GetNbFreeBlocks() {
        S32 l_Total = 0;

        for (S32 i = 0; i < s_MEM_BUCKET_COUNT; i++) {
            l_Total += m_BucketFreeBlocks[i];
        }

        return l_Total;
    } // Fully inlined

    S32 GetFreeMem() {
        S32 l_Total = 0;
        s_MEM_BLOCK** l_Track;

        for (S32 i = 0; i < s_MEM_BUCKET_COUNT; i++) {
            S32 l_Count = m_BucketFreeBlocks[i];
            l_Track = m_BucketBlocks + m_BucketTotalBlocks * i;

            for (S32 j = 0; j < l_Count; j++) {
                l_Total += (*l_Track)->GetSize();
                l_Track++;
            }
        }

        return l_Total;
    } // Fully inlined

    U32 GetLargestFreeBlock() {
        U32 l_LargestSize = 0;
        s_MEM_BLOCK** l_Track;
        s_MEM_BLOCK* l_Block;

        for (S32 i = s_MEM_BUCKET_COUNT - 1; i >= 0; i--) {
            S32 l_Count = m_BucketFreeBlocks[i];
            l_Track = m_BucketBlocks + m_BucketTotalBlocks * i;

            for (S32 j = 0; j < l_Count; j++) {
                l_Block = *l_Track;
                U32 l_Size = l_Block->GetSize();

                if (l_Size > l_LargestSize) {
                    l_LargestSize = l_Size;
                }

                l_Track++;
            }

            if (l_Count) {
                break;
            }
        }

        return l_LargestSize;
    } // Fully inlined

    Bool TestIntegrity() {
        for (S32 i = 0; i < s_MEM_BUCKET_COUNT; i++) {
            S32 l_Count = m_BucketFreeBlocks[i];

            for (S32 j = 0; j < l_Count; j++) {
                s_MEM_BLOCK* l_Block = m_BucketBlocks[m_BucketTotalBlocks * i + j];

                if (!s_QuickTestIntegrity(l_Block)) {
                    return FALSE;
                }
            }
        }

        return TRUE;
    } // Fully inlined

    Bool TestBlock(s_MEM_BLOCK* i_Block) {
        if (i_Block->GetFree()) {
            if (s_GetInBucket(FindBucket(i_Block->GetSize()), i_Block) == -1) {
                return FALSE;
            }
        }
        else if (s_GetInBucket(FindBucket(i_Block->GetSize()), i_Block) != -1) {
            return FALSE;
        }

        return TRUE;
    } // Fully inlined

    void RemoveBlockInline(s_MEM_BLOCK* i_Block) {
        S32 l_BucketIdx = FindBucket(i_Block->GetSize());

        i_Block->m_BlockFlags.m_IsFree = FALSE;

        S32 l_BlockIdx = s_GetInBucket(l_BucketIdx, i_Block);
        m_BucketFreeBlocks[l_BucketIdx]--;
        S32 l_Count = m_BucketFreeBlocks[l_BucketIdx];
        s_MEM_BLOCK** l_BucketTrack = m_BucketBlocks + m_BucketTotalBlocks * l_BucketIdx;

        if (l_BlockIdx < l_Count) {
            s_MEM_BLOCK** l_Current = l_BucketTrack + l_BlockIdx;
            s_MEM_BLOCK** l_End = l_BucketTrack + l_Count;

            while (l_Current < l_End) {
                *l_Current = l_Current[1];
                l_Current++;
            }
        }
    } // Fully inlined

    void Extend();
    S32 s_GetInBucket(S32 i_BucketIdx, s_MEM_BLOCK* i_Block);
    s_MEM_BLOCK* FindBlock(S32 i_BucketIdx, S32 i_MinSize);
    void AddBlock(s_MEM_BLOCK* i_Block);
    void RemoveBlock(s_MEM_BLOCK* i_Block);
    s_MEM_BLOCK* FindBlock(S32 i_MinSize);
};

#endif
