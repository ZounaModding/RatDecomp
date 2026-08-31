#ifndef _WARP_ZHDL_H_
#define _WARP_ZHDL_H_
#include "ResourceObject_ZHdl.h"

class Warp_Z;
HANDLE_Z(Warp_Z, ResourceObject_Z);

typedef DynArray_Z<Warp_ZHdl, 32> Warp_ZHdlDA;

#endif
