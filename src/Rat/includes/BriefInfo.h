#ifndef _BRIEFINFO_H_
#define _BRIEFINFO_H_
#include "DynArray_Z.h"
#include "Types_Z.h"

struct BriefInfo {
    U8 m_Data[0x14];
};

typedef DynArray_Z<BriefInfo, 1> BriefInfoDA;

struct BriefData {
    BriefInfoDA m_BriefInfos;
};
#endif // _BRIEFINFO_H_
