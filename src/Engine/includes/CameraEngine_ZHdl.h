#ifndef _CAMERAENGINE_ZHDL_H_
#define _CAMERAENGINE_ZHDL_H_
#include "Manipulator_ZHdl.h"

class Manipulator_Z;
class CameraEngine_Z;
HANDLE_Z(CameraEngine_Z, Manipulator_Z);

typedef DynArray_Z<CameraEngine_ZHdl, 8> CameraEngine_ZHdlDA;

#endif
