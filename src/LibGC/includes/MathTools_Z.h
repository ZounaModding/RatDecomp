#ifndef _MATHTOOLS_Z_H_
#define _MATHTOOLS_Z_H_
#include "Types_Z.h"
#include <math.h>

struct Vec2f;

Float O_Atan2(Float x, Float y);
Float O_Cos(Float x);
Float O_Sin(Float x);
void O_SinCos(Vec2f& o_Result, Float i_Angle);
Float O_Tan(Float x);
Float O_ACos(Float x);
Float O_ASin(Float x);
Float O_ATan(Float x);

#endif
