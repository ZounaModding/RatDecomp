#include "Math_Z.h"
#include "MathTools_Z.h"

const Vec3i VEC3I_NULL = Vec3i(0, 0, 0);
const Vec4i VEC4I_NULL = Vec4i(0, 0, 0, 0);

// Vec2f constants
const Vec2f VEC2F_NULL = Vec2f(0.f, 0.f);
const Vec2f VEC2F_ONE = Vec2f(1.f, 1.f);

// Vec3f constants
const Vec3f VEC3F_NULL = Vec3f(0.f, 0.f, 0.f);
const Vec3f VEC3F_ONE = Vec3f(1.f, 1.f, 1.f);
const Vec3f VEC3F_HALF = Vec3f(0.5f, 0.5f, 0.5f);
const Vec3f VEC3F_UP = Vec3f(0.f, 1.f, 0.f);
const Vec3f VEC3F_DOWN = Vec3f(0.f, -1.f, 0.f);
const Vec3f VEC3F_LEFT = Vec3f(1.f, 0.f, 0.f);
const Vec3f VEC3F_RIGHT = Vec3f(-1.f, 0.f, 0.f);
const Vec3f VEC3F_FRONT = Vec3f(0.f, 0.f, 1.f);
const Vec3f VEC3F_BACK = Vec3f(0.f, 0.f, -1.f);

// Vec4f constants
const Vec4f VEC4F_NULL = Vec4f(0.f, 0.f, 0.f, 0.f);
const Vec4f VEC4F_UP = Vec4f(0.f, 1.f, 0.f, 0.f);
const Vec4f VEC4F_DOWN = Vec4f(0.f, -1.f, 0.f, 0.f);
const Vec4f VEC4F_LEFT = Vec4f(1.f, 0.f, 0.f, 0.f);
const Vec4f VEC4F_RIGHT = Vec4f(-1.f, 0.f, 0.f, 0.f);
const Vec4f VEC4F_FRONT = Vec4f(0.f, 0.f, 1.f, 0.f);
const Vec4f VEC4F_BACK = Vec4f(0.f, 0.f, -1.f, 0.f);

// Color constants
const Color COLOR_WHITE = Color(1.f, 1.f, 1.f, 1.f);
const Color COLOR_BLACK = Color(0.f, 0.f, 0.f, 1.f);
const Color COLOR_GREY = Color(0.5f, 0.5f, 0.5f, 1.f);
const Color COLOR_LIGHTGREY = Color(0.66f, 0.66f, 0.66f, 1.f);
const Color COLOR_DARKGREY = Color(0.33f, 0.33f, 0.33f, 1.f);
const Color COLOR_RED = Color(1.f, 0.f, 0.f, 1.f);
const Color COLOR_GREEN = Color(0.f, 1.f, 0.f, 1.f);
const Color COLOR_BLUE = Color(0.f, 0.f, 1.f, 1.f);
const Color COLOR_YELLOW = Color(1.f, 1.f, 0.f, 1.f);
const Color COLOR_CYAN = Color(0.f, 1.f, 1.f, 1.f);
const Color COLOR_MAGENTA = Color(1.f, 0.f, 1.f, 1.f);
const Color COLOR_ORANGE = Color(1.f, 0.5f, 0.f, 1.f);
const Color COLOR_BROWN = Color(145.f / 255.f, 89.0f / 255.f, 60.f / 255.f, 1.f);
const Color COLOR_LIGHTBLUE = Color(0.5f, 0.5f, 1.f, 1.f);
const Color COLOR_LIGHTGREEN = Color(0.5f, 1.f, 0.5f, 1.f);
const Color COLOR_LIGHTRED = Color(1.f, 0.5f, 0.5f, 1.f);

const Quat QUAT_NULL = Quat(1.f, 0.f, 0.f, 0.f);

Vec3f::Vec3f(const Color& i_Color)
    : x(i_Color.r)
    , y(i_Color.g)
    , z(i_Color.b) { };

Vec3f::Vec3f(const Quat& Q) {
    Float w = Q.w;

    if (w < -1.f) w = -1.f;
    if (w > 1.f) w = 1.f;
    Float halfang = O_ACos(w);

    Float s = sqrt(1.f - w * w);

    if (s > Float_Eps)
        *this = Q.v * (1.f / s);
    else {
        *this = VEC3F_NULL;
        return;
    }
    w = halfang * 2.f;
    if (w < 0.f) {
        *this = -*this;
        w = -w;
    }
    if (w > 3.141592f) {
        *this = -*this;
        w = 3.141592f * 2.f - w;
    }
    *this *= w;
}

void Mat4x4::Transp(Mat4x4& o_Matrix) const {
    for (S32 i = 0; i < 4; i++) {
        for (S32 j = 0; j < 4; j++) {
            o_Matrix.m[i][j] = m[j][i];
        }
    }
}

void Mat4x4::SetTRS(const Vec3f& i_Trans, const Quat& i_Rot, const Vec3f& i_Scale) {
    i_Rot.GetMatrix(*this);

    m[0][0] *= i_Scale.x;
    m[1][0] *= i_Scale.y;
    m[2][0] *= i_Scale.z;
    m[3][0] = i_Trans.x;
    m[0][1] *= i_Scale.x;
    m[1][1] *= i_Scale.y;
    m[2][1] *= i_Scale.z;
    m[3][1] = i_Trans.y;
    m[0][2] *= i_Scale.x;
    m[1][2] *= i_Scale.y;
    m[2][2] *= i_Scale.z;
    m[3][2] = i_Trans.z;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

void Inverse2(const Mat4x4& In, Mat4x4& Out) {

    Float Det = 0.f;
    Det += In.m[0][0] * In.m[1][1] * In.m[2][2];
    Det += In.m[1][0] * In.m[2][1] * In.m[0][2];
    Det += In.m[2][0] * In.m[0][1] * In.m[1][2];
    Det -= In.m[2][0] * In.m[1][1] * In.m[0][2];
    Det -= In.m[1][0] * In.m[0][1] * In.m[2][2];
    Det -= In.m[0][0] * In.m[2][1] * In.m[1][2];

    Float Det1 = 1.f / Det;

    Out.m[0][0] = (In.m[1][1] * In.m[2][2] - In.m[2][1] * In.m[1][2]) * Det1;
    Out.m[0][1] = -(In.m[0][1] * In.m[2][2] - In.m[2][1] * In.m[0][2]) * Det1;
    Out.m[0][2] = (In.m[0][1] * In.m[1][2] - In.m[1][1] * In.m[0][2]) * Det1;
    Out.m[1][0] = -(In.m[1][0] * In.m[2][2] - In.m[2][0] * In.m[1][2]) * Det1;
    Out.m[1][1] = (In.m[0][0] * In.m[2][2] - In.m[2][0] * In.m[0][2]) * Det1;
    Out.m[1][2] = -(In.m[0][0] * In.m[1][2] - In.m[1][0] * In.m[0][2]) * Det1;
    Out.m[2][0] = (In.m[1][0] * In.m[2][1] - In.m[2][0] * In.m[1][1]) * Det1;
    Out.m[2][1] = -(In.m[0][0] * In.m[2][1] - In.m[2][0] * In.m[0][1]) * Det1;
    Out.m[2][2] = (In.m[0][0] * In.m[1][1] - In.m[1][0] * In.m[0][1]) * Det1;

    Out.m[3][0] = -(Out.m[0][0] * In.m[3][0] + Out.m[1][0] * In.m[3][1] + Out.m[2][0] * In.m[3][2]);
    Out.m[3][1] = -(Out.m[0][1] * In.m[3][0] + Out.m[1][1] * In.m[3][1] + Out.m[2][1] * In.m[3][2]);
    Out.m[3][2] = -(Out.m[0][2] * In.m[3][0] + Out.m[1][2] * In.m[3][1] + Out.m[2][2] * In.m[3][2]);

    Out.m[0][3] = 0.f;
    Out.m[1][3] = 0.f;
    Out.m[2][3] = 0.f;
    Out.m[3][3] = 1.f;
}

Quat::Quat(const Mat3x3& Matrix) {
    SetMatrix(Matrix);
}

Quat::Quat(const Mat4x4& Matrix) {
    SetMatrix(Matrix);
}

void Quat::SetMatrix(const Mat4x4& Matrix) {
    Float trace = Matrix.m[0][0] + Matrix.m[1][1] + Matrix.m[2][2];
    Float root;

    if (trace > 0.0f) {
        root = sqrtf(trace + 1.f);
        w = 0.5f * root;
        root = 0.5f / root;
        v.x = (Matrix.m[1][2] - Matrix.m[2][1]) * root;
        v.y = (Matrix.m[2][0] - Matrix.m[0][2]) * root;
        v.z = (Matrix.m[0][1] - Matrix.m[1][0]) * root;
    }
    else {
        static int next[3] = { 1, 2, 0 };
        int i = 0;
        if (Matrix.m[1][1] > Matrix.m[0][0]) i = 1;
        if (Matrix.m[2][2] > Matrix.m[i][i]) i = 2;
        int j = next[i];
        int k = next[j];

        root = sqrtf(Matrix.m[i][i] - Matrix.m[j][j] - Matrix.m[k][k] + 1.f);
        v[i] = 0.5f * root;

        if (root != 0.0f)
            root = 0.5f / root;

        w = (Matrix.m[j][k] - Matrix.m[k][j]) * root;
        v[j] = (Matrix.m[i][j] + Matrix.m[j][i]) * root;
        v[k] = (Matrix.m[i][k] + Matrix.m[k][i]) * root;
    }
    Normalize();
}

void Quat::SetMatrix(const Mat3x3& Matrix) {
    Float trace = Matrix.m.m[0][0] + Matrix.m.m[1][1] + Matrix.m.m[2][2];
    Float root;

    if (trace > 0.0f) {
        root = sqrtf(trace + 1.f);
        w = 0.5f * root;
        root = 0.5f / root;
        v.x = (Matrix.m.m[1][2] - Matrix.m.m[2][1]) * root;
        v.y = (Matrix.m.m[2][0] - Matrix.m.m[0][2]) * root;
        v.z = (Matrix.m.m[0][1] - Matrix.m.m[1][0]) * root;
    }
    else {
        static int next[3] = { 1, 2, 0 };
        int i = 0;
        if (Matrix.m.m[1][1] > Matrix.m.m[0][0]) i = 1;
        if (Matrix.m.m[2][2] > Matrix.m.m[i][i]) i = 2;
        int j = next[i];
        int k = next[j];

        root = sqrtf(Matrix.m.m[i][i] - Matrix.m.m[j][j] - Matrix.m.m[k][k] + 1.f);

        v[i] = 0.5f * root;

        if (root != 0.0f)
            root = 0.5f / root;

        w = (Matrix.m.m[j][k] - Matrix.m.m[k][j]) * root;
        v[j] = (Matrix.m.m[i][j] + Matrix.m.m[j][i]) * root;
        v[k] = (Matrix.m.m[i][k] + Matrix.m.m[k][i]) * root;
    }
    Normalize();
}

void Quat::GetMatrix(Mat4x4& Mat) const {
    Float X = v.x, Y = v.y, Z = v.z, W = w;
    Float X2 = 2.f * X * X, Y2 = 2.f * Y * Y, Z2 = 2.f * Z * Z;
    Float XY = 2.f * X * Y, ZY = 2.f * Z * Y, XZ = 2.f * X * Z;
    Float WX = 2.f * W * X, WY = 2.f * W * Y, WZ = 2.f * W * Z;

    Mat.m[0][0] = (1.f - Y2 - Z2);
    Mat.m[1][0] = XY - WZ;
    Mat.m[2][0] = XZ + WY;

    Mat.m[3][0] = 0.f;
    Mat.m[0][1] = (XY + WZ);
    Mat.m[1][1] = 1.f - X2 - Z2;
    Mat.m[2][1] = ZY - WX;
    Mat.m[3][1] = 0.f;
    Mat.m[0][2] = (XZ - WY);
    Mat.m[1][2] = ZY + WX;
    Mat.m[2][2] = 1.f - X2 - Y2;
    Mat.m[3][2] = 0.f;
    Mat.m[0][3] = 0.f;
    Mat.m[1][3] = 0.f;
    Mat.m[2][3] = 0.f;
    Mat.m[3][3] = 1.f;
}

void Quat::GetMatrix(Mat3x3& Mat) const {
    Float X = v.x, Y = v.y, Z = v.z, W = w;
    Float X2 = 2 * X * X, Y2 = 2 * Y * Y, Z2 = 2 * Z * Z;
    Float XY = 2 * X * Y, ZY = 2 * Z * Y, XZ = 2 * X * Z;
    Float WX = 2 * W * X, WY = 2 * W * Y, WZ = 2 * W * Z;

    Mat.m.m[0][0] = (1 - Y2 - Z2);
    Mat.m.m[1][0] = XY - WZ;
    Mat.m.m[2][0] = XZ + WY;
    Mat.m.m[0][1] = (XY + WZ);
    Mat.m.m[1][1] = 1 - X2 - Z2;
    Mat.m.m[2][1] = ZY - WX;
    Mat.m.m[0][2] = (XZ - WY);
    Mat.m.m[1][2] = ZY + WX;
    Mat.m.m[2][2] = 1 - X2 - Y2;
}

Vec3f Quat::operator*(const Vec3f& p) const {
    Float lw, vx, vy, vz;

    lw = w;
    vx = v.x;
    vy = v.y;
    vz = v.z;

    Float pw, pvx, pvy, pvz;

    pvx = p.x;
    pvy = p.y;
    pvz = p.z;

    Vec3f result;

    pw = -vx * pvx - vy * pvy - vz * pvz;
    result.x = lw * pvx + vy * pvz - vz * pvy;
    result.y = lw * pvy + vz * pvx - vx * pvz;
    pvz = lw * pvz + vx * pvy - vy * pvx;

    pvx = result.x;
    pvy = result.y;

    result.x = -pw * vx + pvx * lw - pvy * vz + pvz * vy;
    result.y = -pw * vy + pvy * lw - pvz * vx + pvx * vz;
    result.z = -pw * vz + pvz * lw - pvx * vy + pvy * vx;

    return result;
}

void Quat::Normalize(void) {
    Float l_Norm = Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + w * w);
    if (l_Norm < Float_Eps) {
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        w = 1.0f;
        return;
    }

    Float rhn = 1.0f / l_Norm;
    v.x *= rhn;
    v.y *= rhn;
    v.z *= rhn;
    w *= rhn;
}

void BuildLookAtMatrix(const Vec3f& i_LookAt, const Vec3f& i_Up, Mat4x4& o_Matrix) {
    Vec3f l_Norm2 = i_LookAt ^ i_Up;
    if (l_Norm2 == VEC3F_NULL) {
        l_Norm2 = Vec3f(1.0f, 0.0f, 0.0f);
    }
    else {
        l_Norm2.Normalize();
    }

    Vec3f l_Norm3 = l_Norm2 ^ i_LookAt;

    o_Matrix.SetIdentity();
    o_Matrix.m[0][0] = l_Norm2.x;
    o_Matrix.m[0][1] = l_Norm2.y;
    o_Matrix.m[0][2] = l_Norm2.z;
    o_Matrix.m[1][0] = l_Norm3.x;
    o_Matrix.m[1][1] = l_Norm3.y;
    o_Matrix.m[1][2] = l_Norm3.z;
    o_Matrix.m[2][0] = i_LookAt.x;
    o_Matrix.m[2][1] = i_LookAt.y;
    o_Matrix.m[2][2] = i_LookAt.z;
}

// TODO: Finish matching
Float ComputeMathPrecision() {
    const Vec4f l_Vec1(1.79f, 1.09f, 1.3f, 1.34f);
    const Vec4f l_Vec2(0.55f, 0.84f, 0.17f, 0.61f);
    const Vec4f l_Vec3(1.13f, 0.78f, 0.35f, 0.87f);
    const Vec4f l_Vec4(0.66f, 0.31f, 0.95f, 0.47f);

    Vec4f l_Result(0.0f, 0.0f, 0.0f, 0.0f);
    l_Result = l_Vec1;
    l_Result += l_Vec2;
    l_Result += l_Result;
    l_Result += l_Vec3;
    l_Result.xyz() += Vec3f(l_Vec2) ^ Vec3f(-0.47f, -0.47f, 0.6f);

    Vec4f l_AbsResult;
    MaxVec(-l_Result, l_Result, l_AbsResult);
    const Vec3f l_Delta(
        l_AbsResult.x - (l_Vec1.x + l_Vec2.x + l_Vec1.x + l_Vec2.x + l_Vec3.x),
        l_AbsResult.y - (l_Vec1.y + l_Vec2.y + l_Vec1.y + l_Vec2.y + l_Vec3.y),
        l_AbsResult.z - (l_Vec1.z + l_Vec2.z + l_Vec1.z + l_Vec2.z + l_Vec3.z)
    );

    Vec4f l_Value;
    l_Value.x = l_Delta.x - l_Vec2.x;
    l_Value.y = (l_Result.xyz() - Vec3f(l_Vec4)).GetNorm2() - l_Vec2.y;
    l_Value.z = l_Delta.z - l_Vec2.z;
    l_Value.w = 0.0f;
    Float l_ValueNorm2 = l_Value.GetNorm2();
    Vec4f l_Temp(l_Delta - Vec3f(l_Vec3));
    l_Value.x = l_Temp.x * l_Temp.x + l_Temp.z * l_Temp.z;
    l_Value.x = Vec3f(
                    l_Result.x - l_Vec4.x,
                    l_Result.y - l_Vec4.y,
                    l_ValueNorm2 - l_Vec4.z
    )
                    .GetNorm();

    Vec4f l_MaxLimits;
    MaxVec(l_Result, l_Vec2, l_MaxLimits);
    Vec4f l_MinLimits;
    MinVec(l_Result, -l_Vec2, l_MinLimits);
    const Vec4f l_Limits = l_MaxLimits + l_MinLimits;
    Vec4f l_AbsLimits;
    MaxVec(-l_Limits, l_Limits, l_AbsLimits);
    l_Value.z = l_AbsLimits * Vec4f(l_Delta.x, l_Limits.GetNorm2(), l_Delta.z, 0.0f);

    Mat4x4 l_Left;
    l_Left.Set(
        l_Vec4.x, l_Vec4.y, l_Vec4.z, l_Vec4.w, l_Vec2.x, l_Vec2.y, l_Vec2.z, l_Vec2.w, l_Vec3.x, l_Vec3.y, l_Vec3.z, l_Vec3.w, l_Vec4.x, l_Vec4.y, l_Vec4.z, l_Vec4.w
    );
    Mat4x4 l_Right;
    l_Right.Set(
        l_Vec3.x, l_Vec3.y, l_Vec3.z, l_Vec3.w, l_Vec4.x, l_Vec4.y, l_Vec4.z, l_Vec4.w, l_Vec2.x, l_Vec2.y, l_Vec2.z, l_Vec2.w, l_Vec3.x, l_Vec3.y, l_Vec3.z, l_Vec3.w
    );

    Mat4x4 l_Matrix;
    for (S32 i = 0; i < 4; i++) {
        l_Matrix.m[0][i] = l_Left.m[0][i] * l_Right.m[0][0] + l_Left.m[1][i] * l_Right.m[0][1] + l_Left.m[2][i] * l_Right.m[0][2] + l_Left.m[3][i] * l_Right.m[0][3];
        l_Matrix.m[1][i] = l_Left.m[0][i] * l_Right.m[1][0] + l_Left.m[1][i] * l_Right.m[1][1] + l_Left.m[2][i] * l_Right.m[1][2] + l_Left.m[3][i] * l_Right.m[1][3];
        l_Matrix.m[2][i] = l_Left.m[0][i] * l_Right.m[2][0] + l_Left.m[1][i] * l_Right.m[2][1] + l_Left.m[2][i] * l_Right.m[2][2] + l_Left.m[3][i] * l_Right.m[2][3];
        l_Matrix.m[3][i] = l_Left.m[0][i] * l_Right.m[3][0] + l_Left.m[1][i] * l_Right.m[3][1] + l_Left.m[2][i] * l_Right.m[3][2] + l_Left.m[3][i] * l_Right.m[3][3];
    }

    Vec4f l_Final(l_Matrix * (l_Value.xyz() + l_Value.xyz()));
    l_Final.x /= 66710628.0f;
    l_Final.y /= 79069208.0f;
    l_Final.z /= 72642080.0f;
    l_Final.w /= 33569748.0f;

    return 25.0f * ((l_Final.x >= 0.0f ? l_Final.x : -l_Final.x) + (l_Final.y >= 0.0f ? l_Final.y : -l_Final.y) + (l_Final.z >= 0.0f ? l_Final.z : -l_Final.z) + (l_Final.w >= 0.0f ? l_Final.w : -l_Final.w)) - 100.0f >= 0.0f
               ? 25.0f * ((l_Final.x >= 0.0f ? l_Final.x : -l_Final.x) + (l_Final.y >= 0.0f ? l_Final.y : -l_Final.y) + (l_Final.z >= 0.0f ? l_Final.z : -l_Final.z) + (l_Final.w >= 0.0f ? l_Final.w : -l_Final.w)) - 100.0f
               : -(25.0f * ((l_Final.x >= 0.0f ? l_Final.x : -l_Final.x) + (l_Final.y >= 0.0f ? l_Final.y : -l_Final.y) + (l_Final.z >= 0.0f ? l_Final.z : -l_Final.z) + (l_Final.w >= 0.0f ? l_Final.w : -l_Final.w)) - 100.0f);
}
