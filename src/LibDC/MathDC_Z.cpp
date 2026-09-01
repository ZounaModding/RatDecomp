#include "Math_Z.h"

Quat Quat::operator*(const Quat& i_Quat) const {
    Quat l_Quat;
    l_Quat.w = w * i_Quat.w - v * i_Quat.v;
    l_Quat.v = w * i_Quat.v + i_Quat.w * v + (v ^ i_Quat.v);
    return l_Quat;
}

Quat& Quat::operator*=(const Quat& i_Quat) {
    *this = *this * i_Quat;
    return *this;
}

Vec4f Mat4x4::operator*(const Vec4f& i_Vec) const {
    Vec4f l_Vec;
    Float l_Z = i_Vec.z;
    Float l_X = i_Vec.x;
    l_Vec.x = m[0][0] * l_X + m[1][0] * i_Vec.y + m[2][0] * l_Z + m[3][0];
    l_Vec.y = m[0][1] * l_X + m[1][1] * i_Vec.y + m[2][1] * l_Z + m[3][1];
    l_Vec.z = m[0][2] * l_X + m[1][2] * i_Vec.y + m[2][2] * l_Z + m[3][2];
    l_Vec.w = 1.0f;
    return l_Vec;
}

Mat4x4& Mat4x4::operator*=(const Mat4x4& i_Matrix) {
    ConcatMat4x4(*this, i_Matrix, *this);
    return *this;
}

Mat4x4 Mat4x4::operator*(const Mat4x4& i_Matrix) const {
    Mat4x4 l_Mat;
    ConcatMat4x4(*this, i_Matrix, l_Mat);
    return l_Mat;
}

void BuildLookAtLH(const Vec3f& i_Position, const Vec3f& i_Target, const Vec3f& i_Up, Mat4x4& o_Matrix) {
}

void ConcatMat4x4(const Mat4x4& a1, const Mat4x4& a2, Mat4x4& a3) {
    Mat4x4 l_Result;
    for (S32 i = 0; i < 4; i++) {
        for (S32 j = 0; j < 4; j++) {
            l_Result.m[i][j] = a1.m[i][0] * a2.m[0][j] + a1.m[i][1] * a2.m[1][j] + a1.m[i][2] * a2.m[2][j] + a1.m[i][3] * a2.m[3][j];
        }
    }
    a3 = l_Result;
}

void ConcatMat4x4AndConvert(const Mat4x4& a1, const Mat4x4& a2, Mat3x3& a3) {
    Mat3x3 l_Result;
    for (S32 i = 0; i < 3; i++) {
        for (S32 j = 0; j < 3; j++) {
            l_Result.m.m[i][j] = a1.m[i][0] * a2.m[0][j] + a1.m[i][1] * a2.m[1][j] + a1.m[i][2] * a2.m[2][j] + a1.m[i][3] * a2.m[3][j];
        }
    }
    a3 = l_Result;
}
