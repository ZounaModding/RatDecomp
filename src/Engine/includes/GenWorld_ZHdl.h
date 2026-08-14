#ifndef _GENWORLD_ZHDL_H_
#define _GENWORLD_ZHDL_H_
#include "Object_ZHdl.h"

class Object_Z;
class GenWorld_Z;
HANDLE_Z(GenWorld_Z, Object_Z);

typedef DynArray_Z<GenWorld_ZHdl, 32> GenWorld_ZHdlDA;

#endif
