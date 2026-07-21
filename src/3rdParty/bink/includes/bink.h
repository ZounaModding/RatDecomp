#ifndef _BINK_H_
#define _BINK_H_

#include "bink_types.h"

typedef struct BINK* HBINK;

typedef struct BINKPLANE {
    S32 Allocate;
    void* Buffer;
    U32 BufferPitch;
} BINKPLANE;

typedef struct BINKFRAMEPLANESET {
    BINKPLANE YPlane;
    BINKPLANE cRPlane;
    BINKPLANE cBPlane;
    BINKPLANE APlane;
} BINKFRAMEPLANESET;

typedef struct BINKFRAMEBUFFERS {
    S32 TotalFrames;
    U32 YABufferWidth;
    U32 YABufferHeight;
    U32 cRcBBufferWidth;
    U32 cRcBBufferHeight;
    U32 FrameNum;
    BINKFRAMEPLANESET Frames[2];
} BINKFRAMEBUFFERS;

typedef struct BINK {
    // TODO: Add members
} BINK;

typedef void* (*BINKMEMALLOC)(U32 bytes);
typedef void (*BINKMEMFREE)(void* ptr);
typedef void* (*BINKSNDOPEN)(void* param);

#ifdef __cplusplus
extern "C" void BinkSetMemory(BINKMEMALLOC a, BINKMEMFREE f);
extern "C" S32 BinkSetSoundSystem(BINKSNDOPEN open, void* param);
#else
void BinkSetMemory(BINKMEMALLOC a, BINKMEMFREE f);
S32 BinkSetSoundSystem(BINKSNDOPEN open, void* param);
#endif

#endif
