#ifndef _BINKNGC_H_
#define _BINKNGC_H_
#include "bink_types.h"

typedef void* (*RADARAMALLOC)(U32 num_bytes);
typedef void  (*RADARAMFREE)(void* ptr);

struct RADARAMCALLBACKS {
    RADARAMALLOC aram_malloc;
    RADARAMFREE  aram_free;
};

#ifdef __cplusplus
extern "C" void* BinkOpenAX(void* i_Callbacks);
#else
void* BinkOpenAX(void* i_Callbacks);
#endif

U32 mult64anddiv(U32 m1, U32 m2, U32 d);
 
void radmemset16(void* dest,U16 value,U32 size);
#endif
