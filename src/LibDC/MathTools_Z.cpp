#include "MathTools_Z.h"
#include "Math_Z.h"

#include <dc/fmath.h>
#include <math.h>

Float O_Atan2(Float x, Float y) {
    if (x == 0.0f && y == 0.0f) {
        return Pi * 0.5f;
    }

    return atan2f(x, y);
}

Float O_Cos(Float x) {
    return fcos(x);
}

Float O_Sin(Float x) {
    return fsin(x);
}

void O_SinCos(Vec2f& o_Result, Float i_Angle) {
    fsincosr(i_Angle, &o_Result.x, &o_Result.y);
}

Float O_Tan(Float x) {
    return ftan(x);
}

Float O_ACos(Float x) {
    return acosf(Clamp<Float>(x, -1.0f, 1.0f));
}

Float O_ASin(Float x) {
    return asinf(Clamp<Float>(x, -1.0f, 1.0f));
}

Float O_ATan(Float x) {
    return atanf(x);
}
