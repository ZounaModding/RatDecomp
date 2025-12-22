#ifndef _OBJECTMOVE_ZHDL_H_
#define _OBJECTMOVE_ZHDL_H_

#include "Manipulator_ZHdl.h"
class ObjectMove_Z;
class Manipulator_Z;

HANDLE_Z(ObjectMove_Z, Manipulator_Z);

typedef DynArray_Z<ObjectMove_ZHdl, 32> ObjectMove_ZHdlDA;

#endif
