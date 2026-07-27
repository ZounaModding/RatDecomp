#include "Timer_Z.h"
#include <kos/timer.h>

void CalibrateTimer() { }

Float GetAbsoluteTime() {
    uint32_t l_Seconds;
    uint32_t l_Microseconds;

    timer_us_gettime(&l_Seconds, &l_Microseconds);

    return (Float)l_Seconds + ((Float)l_Microseconds / 1000000.0f);
}
