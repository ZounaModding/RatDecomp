#ifndef _HFIELD_ZHDL_H_
#define _HFIELD_ZHDL_H_
#include "Object_ZHdl.h"

class Object_Z;
class HField_Z;
HANDLE_Z(HField_Z, Object_Z);

typedef DynArray_Z<HField_ZHdl, 32> HField_ZHdlDA;

#endif
