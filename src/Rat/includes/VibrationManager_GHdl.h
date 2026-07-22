#ifndef _VIBRATIONMANAGER_CHDL_H_
#define _VIBRATIONMANAGER_CHDL_H_
#include "Manipulator_ZHdl.h"

class VibrationManager_C;

HANDLE_Z(VibrationManager_C, Manipulator_Z);

typedef DynArray_Z<VibrationManager_CHdl> VibrationManager_CHdlDA;
#endif // _VIBRATIONMANAGER_CHDL_H_
