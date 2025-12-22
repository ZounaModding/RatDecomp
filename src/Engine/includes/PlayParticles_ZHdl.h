#ifndef _PLAYPARTICLES_ZHDL_H_
#define _PLAYPARTICLES_ZHDL_H_

#include "Manipulator_ZHdl.h"
class PlayParticles_Z;
class Manipulator_Z;

HANDLE_Z(PlayParticles_Z, Manipulator_Z);

typedef DynArray_Z<PlayParticles_ZHdl, 32> PlayParticles_ZHdlDA;

#endif
