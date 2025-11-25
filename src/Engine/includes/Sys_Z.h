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
#define LOAD_Z(dst) MEMCPYFROM_Z(&dst, *i_Data, sizeof(dst))                                                               // NOLINT(bugprone-sizeof-expression)
#define LOADBYTES_Z(dst, size) MEMCPYFROM_Z(dst, *i_Data, size)                                                            // NOLINT(bugprone-sizeof-expression)
#define LOADRANGE_Z(start, end) MEMCPYFROMRANGE_Z(&start, *i_Data, start, end)                                             // NOLINT(bugprone-sizeof-expression)
#define LOADARRAY_Z(dst, count) MEMCPYFROM_Z(dst, *i_Data, sizeof(*dst) * count)                                           // NOLINT(bugprone-sizeof-expression)
#define LOADDYNARRAY_Z(da) \
    LOAD_Z(l_Nb);          \
    da.SetSize(l_Nb);      \
    LOADARRAY_Z(da.GetArrayPtr(), l_Nb)
#define LOADDYNARRAYNB_Z(da, nb) \
    LOAD_Z(nb);                  \
    da.SetSize(nb);              \
    LOADARRAY_Z(da.GetArrayPtr(), nb)
#define LOADDYNARRAYL_Z(da) \
    LOAD_Z(l_Nb);           \
    da.SetSize(l_Nb);       \
    for (S32 i = 0; i < l_Nb; i++) da[i].Load(i_Data)
#define LOADDYNARRAYNBL_Z(da, nb) \
    LOAD_Z(nb);                   \
    da.SetSize(nb);               \
    for (S32 i = 0; i < nb; i++) da[i].Load(i_Data);
#endif
