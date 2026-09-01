#ifndef _MATHGC_Z_H_
#define _MATHGC_Z_H_

#include "Math_Z.h"
#include <mtx.h>

#define ConvertMtx_Z(inM, outM)                          \
    for (U32 l_counti = 0; l_counti < 3; l_counti++)     \
        for (U32 l_countj = 0; l_countj < 4; l_countj++) \
            outM[l_counti][l_countj] = inM.m[l_countj][l_counti];

#define ConvertMtx33_Z(inM, outM)                                   \
    for (U32 l_counti = 0; l_counti < 3; l_counti++) {              \
        for (U32 l_countj = 0; l_countj < 3; l_countj++)            \
            outM[l_counti][l_countj] = inM.m.m[l_countj][l_counti]; \
        outM[l_counti][3] = 0.0f;                                   \
    }

#define ConvertMtx44_Z(inM, outM)                                 \
    for (U32 l_counti = 0; l_counti < 4; l_counti++) {            \
        for (U32 l_countj = 0; l_countj < 4; l_countj++)          \
            outM[l_counti][l_countj] = inM.m[l_countj][l_counti]; \
    }

#define Reverse_Z(mtx)                            \
    for (U32 l_count = 0; l_count < 4; l_count++) \
        mtx[2][l_count] = -mtx[2][l_count];

#define ReverseColumn_Z(mtx)                      \
    for (U32 l_count = 0; l_count < 4; l_count++) \
        mtx[l_count][2] = -mtx[l_count][2];

#endif
