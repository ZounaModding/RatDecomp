#ifndef _FLARE_ZHDL_H_
#define _FLARE_ZHDL_H_
#include "Object_ZHdl.h"

class Object_Z;
class Flare_Z;
HANDLE_Z(Flare_Z, Object_Z);

typedef DynArray_Z<Flare_ZHdl, 32> Flare_ZHdlDA;

#endif
