#include "Math_Z.h"

void Vec4_Add(Vec4f& o_Result, const Vec4f& i_Left, const Vec4f& i_Right) {
    o_Result.x = i_Left.x + i_Right.x;
    o_Result.y = i_Left.y + i_Right.y;
    o_Result.z = i_Left.z + i_Right.z;
    o_Result.w = i_Left.w + i_Right.w;
}
