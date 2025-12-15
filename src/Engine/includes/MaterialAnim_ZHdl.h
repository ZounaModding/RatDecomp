#ifndef _MATERIALANIM_ZHDL_H_
#define _MATERIALANIM_ZHDL_H_
#include "ResourceObject_ZHdl.h"

class MaterialAnim_Z;

HANDLE_Z(MaterialAnim_Z, ResourceObject_Z);

typedef DynArray_Z<MaterialAnim_ZHdl, 32> MaterialAnim_ZHdlDA;

#endif // _MATERIALANIM_Z_H_
