#include "LowMemory_Z.h"
#include "Memory_Z.h"
Extern_Z "C" void memset(void* dest, S32 c, S32 n);
Extern_Z "C" void* OSGetArenaLo();
Extern_Z "C" void* OSGetArenaHi();

Extern_Z "C" void* __sys_alloc(S32 i_Size) {
    return AllocAlignL_Z(i_Size, 13, 32);
};

Extern_Z "C" void __sys_free(void* i_Ptr) {
    Z_Free(i_Ptr);
};

void MemoryManager_Z::Init() {
    m_HeapBase = OSGetArenaLo();
    m_HeapEnd = OSGetArenaHi();
    m_HeapEnd = (void*)((U8*)m_HeapEnd - 0x10000 + 0x600);
    memset(m_HeapBase, 14, (U8*)m_HeapEnd - (U8*)m_HeapBase);
    Hi_MemoryManager_Z::Init();
}

MemoryManager_Z MemManager;
