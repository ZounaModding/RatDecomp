#include "math_ppc.h"
#include "fdlibm.h"

__declspec(weak) double fabs(double f) {
    return __fabs(f);
}

__declspec(weak) float powf(float f, float f2) {
    return pow(f, f2);
}

/* 8036CA0C-8036CA30 36734C 0024+00 0/0 4/4 0/0 .text            cosf */
__declspec(weak) float cosf(float x) {
    return cos(x);
}

/* 8036C9E8-8036CA0C 367328 0024+00 0/0 4/4 0/0 .text            sinf */
__declspec(weak) float sinf(float x) {
    return sin(x);
}
