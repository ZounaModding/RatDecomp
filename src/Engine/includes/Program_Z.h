#ifndef _PROGRAM_Z_H_
#define _PROGRAM_Z_H_

#include "BaseObject_Z.h"
#include "Main_Z.h"
#include "LowMemory_Z.h"
#include "Platform_Z.h"

#define BIGFILE_MAJOR_VERSION "1"
#define BIGFILE_MINOR_VERSION 6
#define BIGFILE_PATCH_VERSION ".63"
#define BIGFILE_VERSION_STRING "v" BIGFILE_MAJOR_VERSION ".%02d%s%s"
#define BIGFILE_VERSION_COPYRIGHT " - Asobo Studio - Internal Cross Technology"

PlatformGlobals_Z gData;

Extern_Z S32 User_NbMatrix;
Extern_Z Float User_MaxSqrDistActive;
Extern_Z Float User_DefaultAgentMaxSqrDistActive;
Extern_Z Float User_DefaultAgentMinSqrDistActive;
Extern_Z S32 User_BFVersion;

#endif
