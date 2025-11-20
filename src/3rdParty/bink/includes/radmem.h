#ifndef _RADMEM_H_
#define _RADMEM_H_
#include "bink_types.h"
#include "OS/OSAlloc.h"

typedef void *(*RADMEMALLOC)(U32 size);
typedef void (*RADMEMFREE)(void* mem);

void RADSetMemory(register RADMEMALLOC malloc_fn, register RADMEMFREE free_fn);
void* radmalloc(U32 size);
void radfree(void* ptr);

extern volatile OSHeapHandle __OSCurrHeap;

RADMEMALLOC usermalloc = 0;
RADMEMFREE userfree = 0;

#endif
