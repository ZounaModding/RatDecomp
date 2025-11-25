#ifndef _SYS_Z_H_
#define _SYS_Z_H_
#include "Types_Z.h"

class Sys_Z {
public:
    static void* MemCpyFrom(void* o_Dest, void* i_Src, const U32 i_Size);
    static void GetAbsoluteFromPath(const Char* i_Path, Char* o_AbsPath, Char* i_Unk);
};

// $VIOLET: NOLINT is needed to get clangd to shut up
#define MEMCPYFROMRANGE_Z(dst, src, start, end) src = Sys_Z::MemCpyFrom(dst, src, ((U8*)&end - (U8*)&start + sizeof(end))) // NOLINT(bugprone-sizeof-expression)
#define MEMCPYFROM_Z(dst, src, size) src = Sys_Z::MemCpyFrom(dst, src, size)                                               // NOLINT(bugprone-sizeof-expression)

// #define MEMCPYFROMRANGEC_Z(dst, src, start, end) Sys_Z::MemCpyFrom(dst, src, ((U8*)&end - (U8*)&start + sizeof(end))) // NOLINT(bugprone-sizeof-expression)
// #define MEMCPYFROMC_Z(dst, src, size) Sys_Z::MemCpyFrom(dst, src, size)                                               // NOLINT(bugprone-sizeof-expression)

#endif
