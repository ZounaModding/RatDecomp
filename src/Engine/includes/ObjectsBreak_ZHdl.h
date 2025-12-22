#ifndef _OBJECTSBREAK_ZHDL_H_
#define _OBJECTSBREAK_ZHDL_H_
#include "ObjectsBounce_ZHdl.h"

class ObjectsBreak_Z;
class ObjectsBounce_Z;

HANDLE_Z(ObjectsBreak_Z, ObjectsBounce_Z);

typedef DynArray_Z<ObjectsBreak_ZHdl, 32> ObjectsBreak_ZHdlDA;

#endif
