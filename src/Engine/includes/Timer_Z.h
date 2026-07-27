#ifndef _TIMER_Z_H_
#define _TIMER_Z_H_
#include "Types_Z.h"

ExternC_Z void CalibrateTimer();
Float GetAbsoluteTime();

#define FPS_PAL 50.0f
#define FPS_NTSC 60.0f
#define FRAME_TIME_PAL (1.0f / FPS_PAL)
#define FRAME_TIME_NTSC (1.0f / FPS_NTSC)

#endif
