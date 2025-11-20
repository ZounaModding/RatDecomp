#include "radmem.h"
#include "os/OSAlloc.h"

void RADSetMemory(RADMEMALLOC malloc_fn, RADMEMFREE free_fn) {
    usermalloc = malloc_fn;
    userfree = free_fn;
}

void* radmalloc(U32 size)
{
    U32 request;
    void *rawBlock;
    U8 fromUser;
    U32 addr;
    U32 offset;
    U8 *aligned;
    if (size == 0 || size == 0xFFFFFFFF)
        return 0;
    request   = size + 0x40;
    if (usermalloc != 0 && (rawBlock = usermalloc(request))) {
        if (rawBlock != 0 && rawBlock != (void *)-1) {
            fromUser = 3;
        } else {
            return 0;
        }
    } else {
        rawBlock = OSAllocFromHeap(__OSCurrHeap, request);
        if (rawBlock == 0) {
            return 0;
        }
        fromUser= 0;
    }
    addr    = (U32)rawBlock;
    offset  = (U32)(0x40 - (addr & 0x1F)) & 0xFF;
    aligned  = (U8 *)rawBlock + offset;
    aligned[-1] = (U8)offset;   
    aligned[-2] = fromUser;        
    if (fromUser == 3)      
        *(void **)(aligned - 8) = (void *)userfree;
    return aligned;
}

void radfree(void* ptr) {
    U8* ptrU8 = (U8*) ptr;
    U32* ptrU32 = (U32*) ptr;
    void (*customFree)(void*);
    if (ptr) {
        if ((ptrU8[-2]) == 3) {
            customFree = (RADMEMFREE)(ptrU32[-2]);
            customFree(ptrU8 - ptrU8[-1]);
        }
        else {
            OSFreeToHeap(__OSCurrHeap, ptrU8 - ptrU8[-1]);
        }
    }
}
