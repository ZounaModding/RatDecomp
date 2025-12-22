#ifndef _PLAYANIM_ZHDL_H_
#define _PLAYANIM_ZHDL_H_

#include "Manipulator_ZHdl.h"
class PlayAnim_Z;
class Manipulator_Z;

HANDLE_Z(PlayAnim_Z, Manipulator_Z);

typedef DynArray_Z<PlayAnim_ZHdl, 32> PlayAnim_ZHdlDA;

#endif
