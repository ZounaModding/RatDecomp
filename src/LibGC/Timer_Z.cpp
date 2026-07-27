#include "Timer_Z.h"
#include <os.h>

#ifdef __MWERKS__
U32 CONSOLE_BUS_SPEED : 0x800000F8;
#else
U32 CONSOLE_BUS_SPEED = 0;
#endif
Extern_Z Float Random();

void CalibrateTimer() { }

Float GetAbsoluteTime() {
    return (((OSGetTick())) / (((CONSOLE_BUS_SPEED / 4)) / 1000.0f) / 1000.0f);
}
