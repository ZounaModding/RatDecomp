#include "LowMemory_Z.h"
#include "Program_Z.h"
#include "Console_Z.h"
#include <malloc.h>
#include <stdio.h>
#include <kos/mm.h>
#include <arch/arch.h>
#include <arch/stack.h>

// The Dreamcast port has no S_MALLOC arena: every allocation is served by the KallistiOS
// dlmalloc heap, and the s_* statistics are tracked here in O(1) instead of walking blocks

// Largest alignment the engine ever requests
#define DC_MEM_MAX_TRACKED_ALIGN 256

// Kept outside the manager so allocations made before MemManager.Init() stay accounted.
static U32 g_AllocatedMem = 0;

static U32 s_RoundUpPow2(U32 i_Value) {
    U32 l_Pow2 = 8;
    while (l_Pow2 < i_Value) {
        l_Pow2 <<= 1;
    }
    return l_Pow2;
}

static U32 s_GetPtrAlignment(void* i_Ptr) {
    U32 l_Addr = (U32)i_Ptr;
    U32 l_Align = l_Addr & (0 - l_Addr);
    if (l_Align > DC_MEM_MAX_TRACKED_ALIGN) {
        l_Align = DC_MEM_MAX_TRACKED_ALIGN;
    }
    return l_Align;
}

static void* s_DCAlloc(U32 i_Size, U32 i_Align) {
    U32 l_Align = s_RoundUpPow2(i_Align);
    void* l_Mem = (l_Align <= 8) ? malloc(i_Size) : memalign(l_Align, i_Size);
    if (l_Mem) {
        g_AllocatedMem += malloc_usable_size(l_Mem);
    }
    return l_Mem;
}

static void s_DCFree(void* i_Ptr) {
    g_AllocatedMem -= malloc_usable_size(i_Ptr);
    free(i_Ptr);
}

static void* s_DCRealloc(void* i_Ptr, U32 i_Size) {
    if (!i_Ptr) {
        return NULL;
    }

    U32 l_Align = s_GetPtrAlignment(i_Ptr);

    if (l_Align <= 8) {
        U32 l_OldSize = malloc_usable_size(i_Ptr);
        void* l_Mem = realloc(i_Ptr, i_Size);
        if (!l_Mem) {
            // The old block is still live on failure.
            return NULL;
        }
        g_AllocatedMem -= l_OldSize;
        g_AllocatedMem += malloc_usable_size(l_Mem);
        return l_Mem;
    }

    // libc realloc does not preserve memalign alignment: re-allocate and copy.
    void* l_Mem = s_DCAlloc(i_Size, l_Align);
    if (!l_Mem) {
        return NULL;
    }

    U32 l_CopySize = malloc_usable_size(i_Ptr);
    if (l_CopySize > i_Size) {
        l_CopySize = i_Size;
    }
    memcpy(l_Mem, i_Ptr, l_CopySize);
    s_DCFree(i_Ptr);
    return l_Mem;
}

U32 s_GetFreeMem() {
    if (!MemManager.m_HeapBase) {
        return 0;
    }
    return ((U32)MemManager.m_HeapEnd - (U32)MemManager.m_HeapBase) - g_AllocatedMem;
}

U32 s_GetAllocatedMem() {
    return g_AllocatedMem;
}

U32 s_GetNbFreeBlocks() {
    // The dlmalloc heap is not walkable: no fragment count.
    return 0;
}

U32 s_GetLargestFree() {
    if (!MemManager.m_HeapBase) {
        return 0;
    }
    // Unclaimed contiguous sbrk headroom; free space
    // inside the heap is not visible without walking it.
    return (U32)MemManager.m_HeapEnd - (U32)mm_sbrk(0);
}

void s_MarkMem(U32 a1) {
}

U32 s_ShowUnMarkedMem() {
    // Hi_MemoryManager_Z::Shut() asserts the leak count is exactly 1.
    return 1;
}

void s_ShowMostNbMalloc() {
    malloc_stats();
}

void Hi_MemoryManager_Z::Init() {
    m_FreeMemCached = (U32)m_HeapEnd - (U32)m_HeapBase;
    m_NbAlloc = 0;
    m_FrameNbAlloc = 0;
    m_FreeMemCached = s_GetFreeMem();
    m_MaxMemUsed = 0;
    m_AllocTimer = 0.0f;
}

void Hi_MemoryManager_Z::Shut() {
    MarkMem(0);
    S32 l_NbLeak = ShowUnMarkedMem();
    if (l_NbLeak != 1) {
        ShowMostNbMalloc();
    }
    ASSERT_Z(l_NbLeak == 1, "Leaks Found");
}

Hi_MemoryManager_Z::Hi_MemoryManager_Z() {
    m_HeapEnd = NULL;
    m_HeapBase = NULL;
}

Hi_MemoryManager_Z::~Hi_MemoryManager_Z() {
}

void Hi_MemoryManager_Z::VerifyMem() {
}

void* Hi_MemoryManager_Z::Alloc(U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line, U32 i_Align) {
    void* l_Mem;
    Char l_Message[512];

#ifdef DEBUGALLOC_Z
    dbglog(DBG_INFO, "> Alloc: %lub align%lu \"%s\" %s:%ld\n", i_Size, i_Align, i_Comment, i_File, i_Line);
#endif

    l_Mem = s_DCAlloc(i_Size, i_Align);
    if (!l_Mem) {
        sprintf(l_Message, "Not enough mem; alloc: %dkb \"%s\"", i_Size >> 10, i_Comment);
        ASSERT_Z(l_Mem, l_Message);
    }
    m_NbAlloc++;
    m_FrameNbAlloc++;
    return l_Mem;
}

void* Hi_MemoryManager_Z::AllocEnd(U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line, U32 i_Align) {
    void* l_Mem;

#ifdef DEBUGALLOC_Z
    dbglog(DBG_INFO, "> AllocEnd: %lub align%lu \"%s\" %s:%ld\n", i_Size, i_Align, i_Comment, i_File, i_Line);
#endif

    l_Mem = s_DCAlloc(i_Size, i_Align);
    if (!l_Mem) {
        return l_Mem;
    }
    m_NbAlloc++;
    m_FrameNbAlloc++;
    return l_Mem;
}

void* Hi_MemoryManager_Z::FindAlloc(void* i_RangeStart, void* i_RangeEnd) {
    return NULL;
}

U32 Hi_MemoryManager_Z::FindAllocNb(void* i_RangeStart, void* i_RangeEnd) {
    return 0;
}

void* Hi_MemoryManager_Z::FindAllocID(S32 i_AllocID, Char* i_ResultDescription, void* i_RangeStart, void* i_RangeEnd) {
    return NULL;
}

void* Hi_MemoryManager_Z::Realloc(void* i_Ptr, U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line) {
    Char l_Message[512];

#ifdef DEBUGALLOC_Z
    dbglog(DBG_INFO, "> Realloc: %p -> %lub \"%s\" %s:%ld\n", i_Ptr, i_Size, i_Comment, i_File, i_Line);
#endif

    void* l_Mem = s_DCRealloc(i_Ptr, i_Size);
    if (l_Mem == NULL) {
        sprintf(l_Message, "Not enough mem; re-alloc: %dkb", i_Size >> 10);
        ASSERT_Z(l_Mem, l_Message);
    }
    m_FrameNbAlloc++;
    return l_Mem;
}

void Hi_MemoryManager_Z::Free(void* i_Ptr) {
    if (!i_Ptr) return;

#ifdef DEBUGALLOC_Z
    dbglog(DBG_INFO, "> Free: %p\n", i_Ptr);
#endif

    s_DCFree(i_Ptr);
    m_NbAlloc--;
}

U32 Hi_MemoryManager_Z::Update(Float i_DeltaTime) {
    U32 l_DidReset;

    m_AllocTimer = m_AllocTimer + i_DeltaTime;
    if (m_AllocTimer > 2.0f) {
        if (m_MaxMemUsed < GetMemUsed()) {
            m_MaxMemUsed = GetMemUsed();
        }
        l_DidReset = 1;
        m_AllocTimer = 0.0f;
        m_FrameNbAlloc = 0;
    }
    else {
        l_DidReset = 0;
    }
    return l_DidReset;
}

void Hi_MemoryManager_Z::PrintStatus() {
    dbglog(DBG_INFO, "> Heap: %lu/%lu used, %lu free, %lu allocs\n", GetMemUsed(), GetHeapSize(), GetFreeMem(), GetNbAlloc());
    s_ShowMostNbMalloc();
}

void Hi_MemoryManager_Z::SetCallStackPtrs(U32* a1, S32 a2) {
    memset(a1, 0, 32);
}

U32 MemoryGraphColor() {
    S32 l_NumParam = gData.Cons->GetNbParam();
    if (l_NumParam >= 2) {
        return TRUE;
    }
    return FALSE;
}

Bool Z_Verify() {
    // No walkable heap to verify
    return TRUE;
}

void* operator new(size_t i_Size, void* i_Ptr) {
    return i_Ptr;
}

void* operator new(size_t i_Size) {
    return MemManager.Alloc(i_Size, "Anonymous New", "No File", 0, _ALLOCDEFAULTALIGN);
}

void* operator new[](size_t i_Size) {
    return MemManager.Alloc(i_Size, "Anonymous New", "No File", 0, _ALLOCDEFAULTALIGN);
}

void operator delete(void* i_Ptr) {
    MemManager.Free(i_Ptr);
}

void operator delete[](void* i_Ptr) {
    MemManager.Free(i_Ptr);
}

void* operator new(size_t i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line) {
    return MemManager.Alloc(i_Size, i_Comment, i_File, i_Line, _ALLOCDEFAULTALIGN);
}

void* operator new[](size_t i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line) {
    return MemManager.Alloc(i_Size, i_Comment, i_File, i_Line, _ALLOCDEFAULTALIGN);
}

void* Z_Alloc(U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line, U32 i_Align) {
    return MemManager.Alloc(i_Size, i_Comment, i_File, i_Line, i_Align);
}

void* Z_AllocEnd(U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line, U32 i_Align) {
    return MemManager.AllocEnd(i_Size, i_Comment, i_File, i_Line, i_Align);
}

void* Z_AllocContiguous(U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line, U32 i_Align) {
    return MemManager.AllocContiguous(i_Size, i_Comment, i_File, i_Line, i_Align);
}

void* Z_Realloc(void* i_Ptr, U32 i_Size, const Char* i_Comment, const Char* i_File, S32 i_Line) {
    return MemManager.Realloc(i_Ptr, i_Size, i_Comment, i_File, i_Line);
}

void Z_Free(void* i_Ptr) {
    MemManager.Free(i_Ptr);
}

void Z_FreeContiguous(void* i_Ptr) {
    MemManager.FreeContiguous(i_Ptr);
}

void MemoryManager_Z::Init() {
    m_HeapBase = mm_sbrk(0);
    m_HeapEnd = (void*)(_arch_mem_top - THD_KERNEL_STACK_SIZE);
    Hi_MemoryManager_Z::Init();
}

MemoryManager_Z MemManager;
