#ifndef _HFOG_ZHDL_H_
#define _HFOG_ZHDL_H_
#include "Object_ZHdl.h"

class Object_Z;
class HFog_Z;
HANDLE_Z(HFog_Z, Object_Z);

typedef DynArray_Z<HFog_ZHdl, 32> HFog_ZHdlDA;

class HFogData_Z;
class ObjectDatas_Z;
HANDLE_Z(HFogData_Z, ObjectDatas_Z);

typedef DynArray_Z<HFogData_ZHdl, 32> HFogData_ZHdlDA;

#endif
