#include "SystemObject_Z.h"

void Sphere_Z::Set(const Vec3f* i_VecPtr, S32 i_NbPoint) {
    Vec3f l_Min;
    Vec3f l_Max;
    l_Min = l_Max = i_VecPtr[0];
    for (S32 i = 1; i < i_NbPoint; i++) {
        l_Min.x = Min(l_Min.x, i_VecPtr[i].x);
        l_Min.y = Min(l_Min.y, i_VecPtr[i].y);
        l_Min.z = Min(l_Min.z, i_VecPtr[i].z);
        l_Max.x = Max(l_Max.x, i_VecPtr[i].x);
        l_Max.y = Max(l_Max.y, i_VecPtr[i].y);
        l_Max.z = Max(l_Max.z, i_VecPtr[i].z);
    }

    Center = (l_Max + l_Min) * 0.5f;
    Float l_Longest = (Center - i_VecPtr[0]).GetNorm2();
    for (S32 i = 1; i < i_NbPoint; i++) {
        l_Longest = Max(l_Longest, (Center - i_VecPtr[i]).GetNorm2());
    }
    Radius = Sqrt(l_Longest);
}

Vec3f CubePoint[8] = {
    Vec3f(1.0f, 1.0f, 1.0f),
    Vec3f(1.0f, 1.0f, -1.0f),
    Vec3f(-1.0f, 1.0f, -1.0f),
    Vec3f(-1.0f, 1.0f, 1.0f),
    Vec3f(1.0f, -1.0f, 1.0f),
    Vec3f(1.0f, -1.0f, -1.0f),
    Vec3f(-1.0f, -1.0f, -1.0f),
    Vec3f(-1.0f, -1.0f, 1.0f),
};

S32 CubeEdge[2 * 12] = {
    0,
    1,
    1,
    2,
    2,
    3,
    3,
    0,
    4,
    5,
    5,
    6,
    6,
    7,
    7,
    4,
    0,
    4,
    1,
    5,
    2,
    6,
    3,
    7,
};

Vec3f CubeNormal[3] = {
    Vec3f(0.0f, 1.0f, 0.0f),
    Vec3f(1.0f, 0.0f, 0.0f),
    Vec3f(0.0f, 0.0f, 1.0f),
};

S32 CubeIndex[6 * 4] = {
    0,
    1,
    2,
    3,
    0,
    4,
    5,
    1,
    0,
    3,
    7,
    4,
    6,
    5,
    4,
    7,
    6,
    7,
    3,
    2,
    6,
    2,
    1,
    5,
};

void Box_Z::MatMulHasScale(const Box_Z& i_Box, const Mat4x4& i_Mat, Float i_Scale, Float i_InvScale) {
    for (S32 i = 0; i < 3; i++) {
        Mat.m.m[0][i] = (i_Box.Mat.m.m[0][0] * i_Mat.m[0][i] + i_Mat.m[1][i] * i_Box.Mat.m.m[0][1] + i_Mat.m[2][i] * i_Box.Mat.m.m[0][2]) * i_InvScale;
        Mat.m.m[1][i] = (i_Box.Mat.m.m[1][0] * i_Mat.m[0][i] + i_Mat.m[1][i] * i_Box.Mat.m.m[1][1] + i_Mat.m[2][i] * i_Box.Mat.m.m[1][2]) * i_InvScale;
        Mat.m.m[2][i] = (i_Box.Mat.m.m[2][0] * i_Mat.m[0][i] + i_Mat.m[1][i] * i_Box.Mat.m.m[2][1] + i_Mat.m[2][i] * i_Box.Mat.m.m[2][2]) * i_InvScale;
        Mat.m.m[i][3] = i_Box.Mat.m.m[0][3] * i_Mat.m[0][i] + i_Mat.m[1][i] * i_Box.Mat.m.m[1][3] + i_Mat.m[2][i] * i_Box.Mat.m.m[2][3] + i_Mat.m[3][i];
    }

    Scale = i_Box.Scale * i_Scale;
    Radius = i_Box.Radius * i_Scale;
}
