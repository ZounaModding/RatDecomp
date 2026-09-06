#ifndef _MATHTOOLS_Z_H_
#define _MATHTOOLS_Z_H_
#include "Types_Z.h"
#include <math.h>

struct Vec2f;
struct Vec3f;
struct Mat4x4;
void MathInitTables();

Float O_Atan2(Float x, Float y);
Float O_Cos(Float x);
Float O_Sin(Float x);
void O_SinCos(Vec2f& o_Result, Float i_Angle);
void BuildLookAtMatrix(const Vec3f& i_LookAt, const Vec3f& i_Up, Mat4x4& o_Matrix);
Float O_Tan(Float x);
Float O_ACos(Float x);
Float O_ASin(Float x);
Float O_ATan(Float x);
#endif
