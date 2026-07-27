#ifndef _PLATFORM_Z_H_
#define _PLATFORM_Z_H_

#include "GCMain_Z.h"
#include "GCMovieManipulator_Z.h"

#define PlatformGlobals_Z Extern_Z GCGlobals

#define BIGFILE_EXTENSION ".DGC"
#define BIGFILE_NAME_EXTENSION ".NGC"
#define BIGFILE_PLATFORM_VERSION ".01"

#define PlatformMovieManipulator_Z                     \
    class GCMovieManipulator_Z;                        \
    HANDLE_Z(GCMovieManipulator_Z, ManipulatorDraw_Z); \
    typedef GCMovieManipulator_ZHdl MovieManipHdl

#endif
