#ifndef _OBJECTTHROW_ZHDL_H_
#define _OBJECTTHROW_ZHDL_H_

#include "ObjectMove_ZHdl.h"
class ObjectThrow_Z;
class ObjectMove_Z;

HANDLE_Z(ObjectThrow_Z, ObjectMove_Z);

typedef DynArray_Z<ObjectThrow_ZHdl, 32> ObjectThrow_ZHdlDA;

#endif
