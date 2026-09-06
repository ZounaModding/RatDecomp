#ifndef _CAMERA_ZHDL_H_
#define _CAMERA_ZHDL_H_
#include "Camera_Z.h"

class Camera_Z;
class Object_Z;
HANDLE_Z(Camera_Z, Object_Z);

typedef DynArray_Z<Camera_ZHdl> Camera_ZHdlDA;
#endif
