#ifndef _OBJECTSBOUNCE_ZHDL_H_
#define _OBJECTSBOUNCE_ZHDL_H_
#include "ObjectsMove_ZHdl.h"

class ObjectsBounce_Z;
class ObjectsMove_Z;

HANDLE_Z(ObjectsBounce_Z, ObjectsMove_Z);

typedef DynArray_Z<ObjectsBounce_ZHdl, 32> ObjectsBounce_ZHdlDA;

#endif
