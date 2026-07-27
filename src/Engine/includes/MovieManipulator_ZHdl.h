#ifndef _MOVIEMANIPULATOR_ZHDL_H_
#define _MOVIEMANIPULATOR_ZHDL_H_
#include "ManipulatorDraw_ZHdl.h"
#include <MovieManipulator_Z.h>
#include "Platform_Z.h"

HANDLE_Z(MovieManipulator_Z, ManipulatorDraw_Z);

PlatformMovieManipulator_Z;

typedef DynArray_Z<MovieManipHdl, 8> MovieManipHdlDA;

#endif
