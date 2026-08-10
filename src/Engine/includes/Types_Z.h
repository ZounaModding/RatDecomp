#ifndef _TYPES_Z_H_
#define _TYPES_Z_H_
#include "Macros_Z.h"

typedef bool Bool;
typedef char Char;
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef short S16;
typedef unsigned long U32;
typedef long S32;
typedef unsigned long long U64;
typedef long long S64;
typedef float Float;
typedef double Double;

typedef volatile bool VBool;
typedef volatile char VChar;
typedef volatile unsigned char VU8;
typedef volatile char VS8;
typedef volatile unsigned short VU16;
typedef volatile short VS16;
typedef volatile unsigned long VU32;
typedef volatile long VS32;
typedef volatile unsigned long long VU64;
typedef volatile long long VS64;
typedef volatile float VFloat;
typedef volatile double VDouble;

#undef S32_MIN
#define S32_MIN -2147483648

#undef S32_MAX
#define S32_MAX 2147483647

#undef U32_MIN
#define U32_MIN 0

#undef U32_MAX
#define U32_MAX 0xFFFFFFFF

#undef U32_MINUS_ONE
#define U32_MINUS_ONE ((U32) - 1)

#undef FALSE
#define FALSE (Bool)(0 == 1)

#undef TRUE
#define TRUE (Bool)(0 == 0)

#undef NULL
#define NULL 0

#undef _ALLOCDEFAULTALIGN
#define _ALLOCDEFAULTALIGN 4

#define AlignUp_Z(A, B) (((U32)(A) + (B) - 1) & ~((B) - 1))
#define AlignDown_Z(A, B) ((U32)(A) & ~((B) - 1))
#define IsAligned_Z(A, B) (((U32)(A) & ((B) - 1)) == 0)

#undef Weak_Z
#ifdef __MWERKS__
#define Weak_Z __declspec(weak)
#else
#define Weak_Z __attribute__((weak))
#endif

#undef Extern_Z
#define Extern_Z extern

#undef ExternC_Z
#define ExternC_Z extern "C"

#undef Volatile_Z
#define Volatile_Z volatile

#define Aligned_Z(x) __attribute__((aligned(x)))
#define Packed_Z(x)        \
    _Pragma("push")        \
        _Pragma("pack(1)") \
            x              \
                _Pragma("pop")

#define ARRAY_CHAR_MAX 256

#undef UNDEFINED_VALUE
#define UNDEFINED_VALUE -1612
#undef UNDEFINED_FVALUE
#define UNDEFINED_FVALUE -1612.0f

#endif
