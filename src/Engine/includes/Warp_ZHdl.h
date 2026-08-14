#ifndef _WARP_ZHDL_H_
#define _WARP_ZHDL_H_
#include "Object_ZHdl.h"

class Object_Z;
class Warp_Z;
HANDLE_Z(Warp_Z, Object_Z);

typedef DynArray_Z<Warp_ZHdl, 32> Warp_ZHdlDA;

#endif
