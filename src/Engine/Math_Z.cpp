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

Mat4x4& Mat4x4::operator*=(const Mat4x4& _m) {
    Mat4x4 Temp;

    for (int i = 0; i < 4; i++) {
        Temp.m[0][i] = m[0][i] * _m.m[0][0] + m[1][i] * _m.m[0][1] + m[2][i] * _m.m[0][2] + m[3][i] * _m.m[0][3];

        Temp.m[1][i] = m[0][i] * _m.m[1][0] + m[1][i] * _m.m[1][1] + m[2][i] * _m.m[1][2] + m[3][i] * _m.m[1][3];

        Temp.m[2][i] = m[0][i] * _m.m[2][0] + m[1][i] * _m.m[2][1] + m[2][i] * _m.m[2][2] + m[3][i] * _m.m[2][3];

        Temp.m[3][i] = m[0][i] * _m.m[3][0] + m[1][i] * _m.m[3][1] + m[2][i] * _m.m[3][2] + m[3][i] * _m.m[3][3];
    }

    *this = Temp;
    return *this;
}

Mat4x4 Mat4x4::operator*(const Mat4x4& _m) const {
    Mat4x4 Temp;
    for (int i = 0; i < 4; i++) {
        Temp.m[0][i] = m[0][i] * _m.m[0][0] + m[1][i] * _m.m[0][1] + m[2][i] * _m.m[0][2] + m[3][i] * _m.m[0][3];

        Temp.m[1][i] = m[0][i] * _m.m[1][0] + m[1][i] * _m.m[1][1] + m[2][i] * _m.m[1][2] + m[3][i] * _m.m[1][3];

        Temp.m[2][i] = m[0][i] * _m.m[2][0] + m[1][i] * _m.m[2][1] + m[2][i] * _m.m[2][2] + m[3][i] * _m.m[2][3];

        Temp.m[3][i] = m[0][i] * _m.m[3][0] + m[1][i] * _m.m[3][1] + m[2][i] * _m.m[3][2] + m[3][i] * _m.m[3][3];
    }
    return Temp;
}

void Mat4x4::SetTRS(const Vec3f& Trans, const Quat& Rot, const Vec3f& Scale) {
    Mat4x4 ScaleMat, RotMat, TransMat;

    ScaleMat.m[0][0] = Scale.x;
    ScaleMat.m[1][0] = 0.f;
    ScaleMat.m[2][0] = 0.f;
    ScaleMat.m[3][0] = 0;
    ScaleMat.m[0][1] = 0.f;
    ScaleMat.m[1][1] = Scale.y;
    ScaleMat.m[2][1] = 0.f;
    ScaleMat.m[3][1] = 0;
    ScaleMat.m[0][2] = 0.f;
    ScaleMat.m[1][2] = 0.f;
    ScaleMat.m[2][2] = Scale.z;
    ScaleMat.m[3][2] = 0;
    ScaleMat.m[0][3] = 0.f;
    ScaleMat.m[1][3] = 0.f;
    ScaleMat.m[2][3] = 0.f;
    ScaleMat.m[3][3] = 1.f;

    TransMat.m[0][0] = 1.f;
    TransMat.m[1][0] = 0.f;
    TransMat.m[2][0] = 0.f;
    TransMat.m[3][0] = Trans.x;
    TransMat.m[0][1] = 0.f;
    TransMat.m[1][1] = 1.f;
    TransMat.m[2][1] = 0.f;
    TransMat.m[3][1] = Trans.y;
    TransMat.m[0][2] = 0.f;
    TransMat.m[1][2] = 0.f;
    TransMat.m[2][2] = 1.f;
    TransMat.m[3][2] = Trans.z;
    TransMat.m[0][3] = 0.f;
    TransMat.m[1][3] = 0.f;
    TransMat.m[2][3] = 0.f;
    TransMat.m[3][3] = 1.f;

    Rot.GetMatrix(RotMat);

    // We calculate the LocalMatrix
    (*this) = TransMat;
    (*this) *= RotMat;
    (*this) *= ScaleMat;
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
    Float rhn = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + w * w); // $VIOLET: I think its using the wrong sqrtf func. doesn't compile right
    v.x *= rhn;
    v.y *= rhn;
    v.z *= rhn;
    w *= rhn;
}
