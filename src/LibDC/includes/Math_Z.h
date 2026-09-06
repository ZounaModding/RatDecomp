#ifndef _MATH_Z_H_
#define _MATH_Z_H_
#include "MathTools_Z.h"
#include "Types_Z.h"
#include "Color_Z.h"
#include "DynArray_Z.h"
#include <math.h>
#include <cmath>
#include <string.h>
#include <dc/fmath.h>

namespace std {
using ::floorf;
using ::powf;
}

const Float Float_Eps = 1.e-6f;
const Float Float_Eps_2 = 1.e-12f;
#define Pi 3.14159265358979323846f
#define ROL_Z(val, shift) ((val << shift) | (val >> ((sizeof(val) * 8) - shift)))
#define ROR_Z(val, shift) ((val >> shift) | (val << ((sizeof(val) * 8) - shift)))
struct Sphere_Z;
struct Vec2f;
struct Vec3f;
struct Vec4f;
struct Quat;
struct QuatComp_Z;
struct Mat3x3;
struct Mat4x4;

inline Float Sqrt(Float x) {
    return fsqrt(x);
}

inline Float InvSqrt(Float x, Float y = 1.0f) {
    if (x) {
        return frsqrt(x) * y;
    }
    return 0.0f;
}

void Inverse2(const Mat4x4&, Mat4x4&);

inline Float DegToRad(Float i_Deg) {
    return (i_Deg * Pi / 180.f);
}

union UDummy { // $SABE: U dummy!
    S16 i16[2];
    U16 u16[2];
    Float f32;
    S32 i32;
    U32 u32;
};

struct Vec2f {
    Float x, y;

    Vec2f() { };
    Vec2f(Float i_x, Float i_y)
        : x(i_x)
        , y(i_y) { };

    Vec2f& Set(Float i_x, Float i_y) {
        x = i_x;
        y = i_y;
        return *this;
    }

    Vec2f& Set(const Vec2f& i_v) {
        x = i_v.x;
        y = i_v.y;
        return *this;
    }

    Vec2f& operator=(const Vec2f& i_v) {
        x = i_v.x;
        y = i_v.y;
        return *this;
    }

    Vec2f operator+(const Vec2f& i_v) const { return Vec2f(x + i_v.x, y + i_v.y); }

    Vec2f& operator+=(const Vec2f& i_v) {
        x += i_v.x;
        y += i_v.y;
        return *this;
    }

    Vec2f operator+() const { return *this; }

    Vec2f operator-(const Vec2f& i_v) const { return Vec2f(x - i_v.x, y - i_v.y); }

    Vec2f& operator-=(const Vec2f& i_v) {
        x -= i_v.x;
        y -= i_v.y;
        return *this;
    }

    Vec2f operator-() const { return Vec2f(-x, -y); }

    Vec2f operator*(Float i_f) const { return Vec2f(x * i_f, y * i_f); }

    Vec2f& operator*=(Float i_f) {
        x *= i_f;
        y *= i_f;
        return *this;
    }

    Float operator*(const Vec2f& i_v) const { return x * i_v.x + y * i_v.y; }

    Vec2f operator/(Float i_f) const {
        float l_Inv = 1.f / i_f;
        return Vec2f(x * l_Inv, y * l_Inv);
    }

    Vec2f& operator/=(Float i_f) {
        float l_Inv = 1.f / i_f;
        x *= l_Inv;
        y *= l_Inv;
        return *this;
    }

    Float operator^(const Vec2f& i_v) const { return x * i_v.y - y * i_v.x; }

    Vec2f operator&(const Vec2f& i_v) const { return Vec2f(x * i_v.x, y * i_v.y); }

    Float& operator[](int i_Index) {
        return (&x)[i_Index];
    }

    const Float& operator[](int i_Index) const {
        return (&x)[i_Index];
    }

    Bool operator==(const Vec2f& i_v) const {
        Vec2f l_Diff = *this - i_v;
        return (fabsf(l_Diff.x) < Float_Eps) && (fabsf(l_Diff.y) < Float_Eps);
    }

    Bool operator!=(const Vec2f& i_v) const { return !operator==(i_v); };

    Float GetNorm2() const { return (*this) * (*this); }

    Float GetNorm() const { return Sqrt(GetNorm2()); }

    Vec2f& Normalize() { return (*this) /= GetNorm(); }
};

Extern_Z const Vec2f VEC2F_NULL;
Extern_Z const Vec2f VEC2F_ONE;

struct Vec3f {
    Float x, y, z;

    Vec3f() { };
    Vec3f(const Color& i_Color);
    Vec3f(const Quat& i_Quat);
    inline Vec3f(const Vec4f& i_Vec);

    Vec3f(Float i_x, Float i_y, Float i_z) {
        x = i_x;
        y = i_y;
        z = i_z;
    }

    Vec3f& Set(Float i_x, Float i_y, Float i_z) {
        x = i_x;
        y = i_y;
        z = i_z;
        return *this;
    }

    Vec3f& Set(const Vec3f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        return *this;
    }

    Vec3f& operator=(const Vec3f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        return *this;
    }

    Vec3f operator=(const Vec4f& i_Vec);

    Vec3f operator+(const Vec3f& i_Vec) const { return Vec3f(x + i_Vec.x, y + i_Vec.y, z + i_Vec.z); }

    Vec3f& operator+=(const Vec3f& i_Vec) {
        x += i_Vec.x;
        y += i_Vec.y;
        z += i_Vec.z;
        return *this;
    }

    Vec3f operator+() const { return *this; }

    Vec3f operator-(const Vec3f& i_Vec) const { return Vec3f(x - i_Vec.x, y - i_Vec.y, z - i_Vec.z); }

    Vec3f& operator-=(const Vec3f& i_Vec) {
        x -= i_Vec.x;
        y -= i_Vec.y;
        z -= i_Vec.z;
        return *this;
    }

    Vec3f operator-() const { return Vec3f(-x, -y, -z); }

    Vec3f operator*(Float i_Factor) const { return Vec3f(x * i_Factor, y * i_Factor, z * i_Factor); }

    Vec3f& operator*=(Float i_Factor) {
        x = x * i_Factor;
        y = y * i_Factor;
        z = z * i_Factor;
        return *this;
    }

    Float operator*(const Vec3f& i_Vec) const { return x * i_Vec.x + y * i_Vec.y + z * i_Vec.z; }

    Vec3f operator/(Float i_Factor) const {
        Float l_Inv = 1.f / i_Factor;
        return Vec3f(x * l_Inv, y * l_Inv, z * l_Inv);
    }

    Vec3f& operator/=(Float i_Factor) {
        Float l_Inv = 1.f / i_Factor;
        x *= l_Inv;
        y *= l_Inv;
        z *= l_Inv;
        return *this;
    }

    Vec3f operator/(const Vec3f& i_Vec) { return Vec3f(x / i_Vec.x, y / i_Vec.y, z / i_Vec.z); }

    Vec3f operator^(const Vec3f& i_Vec) const { return Vec3f(y * i_Vec.z - z * i_Vec.y, z * i_Vec.x - x * i_Vec.z, x * i_Vec.y - y * i_Vec.x); }

    Vec3f operator&(const Vec3f& i_Vec) const { return Vec3f(x * i_Vec.x, y * i_Vec.y, z * i_Vec.z); }

    Float& operator[](int i_Index) { return (&x)[i_Index]; }

    const Float& operator[](int i_Index) const { return (&x)[i_Index]; }

    Bool operator==(const Vec3f& i_Vec) const {
        Vec3f l_Diff = *this - i_Vec;
        return (fabsf(l_Diff.x) < Float_Eps) && (fabsf(l_Diff.y) < Float_Eps) && (fabsf(l_Diff.z) < Float_Eps);
    }

    Bool operator!=(const Vec3f& i_Vec) const { return !operator==(i_Vec); };

    operator Float*() { return &x; }

    Float GetNorm2() const { return (*this) * (*this); }

    Float GetNorm() const { return Sqrt(GetNorm2()); }

    inline Vec3f& Normalize() {
        return (*this) *= InvSqrt(GetNorm2());
    }

    Float HGetNorm2() const { return x * x + z * z; }

    Float HGetNorm() const { return fsqrt(HGetNorm2()); }

    Vec3f& HNormalize() {
        *this = (Vec3f(x, 0.f, z) / HGetNorm());
        return *this;
    }

    Bool CNormalize() {
        const Float n((*this) * (*this));

        // $SABE - From MixedRealityToolikt: Using Float_Eps generates precision problems
#ifndef BUGFIXES_Z
        if (n > Float_Eps)
#else
        if (n > Float_Eps_2)
#endif
        {
            (*this) *= InvSqrt(n);
            return TRUE;
        }
        return FALSE;
    }
};

Extern_Z const Vec3f VEC3F_NULL;
Extern_Z const Vec3f VEC3F_ONE;
Extern_Z const Vec3f VEC3F_HALF;
Extern_Z const Vec3f VEC3F_UP;
Extern_Z const Vec3f VEC3F_DOWN;
Extern_Z const Vec3f VEC3F_LEFT;
Extern_Z const Vec3f VEC3F_RIGHT;
Extern_Z const Vec3f VEC3F_FRONT;
Extern_Z const Vec3f VEC3F_BACK;
Extern_Z const Vec3f VEC3F_FRONTLEFT;
Extern_Z const Vec3f VEC3F_FRONTRIGHT;
Extern_Z const Vec3f VEC3F_BACKLEFT;
Extern_Z const Vec3f VEC3F_BACKRIGHT;

inline Vec3f operator*(Float i_Factor, const Vec3f& i_Vec) {
    return i_Vec * i_Factor;
}

struct Vec3i {
    S32 x, y, z;

    Vec3i() { };

    Vec3i(S32 i_x, S32 i_y, S32 i_z) {
        x = i_x;
        y = i_y;
        z = i_z;
    };

    Vec3i& Set(S32 i_x, S32 i_y, S32 i_z) {
        x = i_x;
        y = i_y;
        z = i_z;
        return *this;
    }

    Vec3i& Set(const Vec3i& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        return *this;
    }

    Vec3i operator=(const Vec3i& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        return *this;
    }

    Vec3i operator+(const Vec3i& i_Vec) const { return Vec3i(x + i_Vec.x, y + i_Vec.y, z + i_Vec.z); }

    Vec3i& operator+=(const Vec3i& i_Vec) {
        x += i_Vec.x;
        y += i_Vec.y;
        z += i_Vec.z;
        return *this;
    }

    Vec3i operator+() const { return *this; }

    Vec3i operator-(const Vec3i& i_Vec) const { return Vec3i(x - i_Vec.x, y - i_Vec.y, z - i_Vec.z); }

    Vec3i& operator-=(const Vec3i& i_Vec) {
        x -= i_Vec.x;
        y -= i_Vec.y;
        z -= i_Vec.z;
        return *this;
    }

    Vec3i operator-() const { return Vec3i(-x, -y, -z); }

    Vec3i operator*(S32 i_Factor) const { return Vec3i(x * i_Factor, y * i_Factor, z * i_Factor); }

    Vec3i& operator*=(S32 i_Factor) {
        x *= i_Factor;
        y *= i_Factor;
        z *= i_Factor;
        return *this;
    }

    Bool operator==(const Vec3i& i_Vec) const { return x == i_Vec.x && y == i_Vec.y && z == i_Vec.z; }

    S32& operator[](const int i_Index) { return (&x)[i_Index]; }

    const S32& operator[](const int i_Index) const { return (&x)[i_Index]; }
};

Extern_Z const Vec3i VEC3I_NULL;

struct Vec4i {
    S32 x, y, z, w;

    Vec4i() { };
    Vec4i(S32 i_x, S32 i_y, S32 i_z, S32 i_w)
        : x(i_x)
        , y(i_y)
        , z(i_z)
        , w(i_w) { };
    Vec4i(const Vec3i& i_v)
        : x(i_v.x)
        , y(i_v.y)
        , z(i_v.z)
        , w(1) { };

    Vec4i operator+(const Vec4i& i_v) const { return Vec4i(x + i_v.x, y + i_v.y, z + i_v.z, w + i_v.w); }

    Vec4i operator-(const Vec4i& i_v) const { return Vec4i(x - i_v.x, y - i_v.y, z - i_v.z, w - i_v.w); }

    Vec4i& operator-=(const Vec4i& i_v) {
        x -= i_v.x;
        y -= i_v.y;
        z -= i_v.z;
        w -= i_v.w;
        return *this;
    }
};

Extern_Z const Vec4i VEC4I_NULL;

struct Vec4f {
    Float x, y, z, w;

    Vec4f() { }

    Vec4f(const Color& i_Color);

    Vec4f(const Vec3f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        w = 1.f;
    }

    Vec4f(Float i_x, Float i_y, Float i_z, Float _w) {
        x = i_x;
        y = i_y;
        z = i_z;
        w = _w;
    }

    Vec4f& Set(Float i_x, Float i_y, Float i_z, Float _w) {
        x = i_x;
        y = i_y;
        z = i_z;
        w = _w;
        return *this;
    }

    Vec4f& Set(const Vec4f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        w = i_Vec.w;
        return *this;
    }

    Vec4f operator=(const Vec4f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        w = i_Vec.w;
        return *this;
    }

    Vec4f operator=(const Vec3f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        w = 1.f;
        return *this;
    }

    Vec4f operator+(const Vec4f& i_Vec) const { return Vec4f(x + i_Vec.x, y + i_Vec.y, z + i_Vec.z, 1.f); }

    Vec4f& operator+=(const Vec4f& i_Vec) {
        x += i_Vec.x;
        y += i_Vec.y;
        z += i_Vec.z;
        w += i_Vec.w;
        return *this;
    }

    Vec4f operator+() const { return *this; }

    Vec4f operator-(const Vec4f& i_Vec) const { return Vec4f(x - i_Vec.x, y - i_Vec.y, z - i_Vec.z, 1.f); }

    Vec4f& operator-=(const Vec4f& i_Vec) {
        x -= i_Vec.x;
        y -= i_Vec.y;
        z -= i_Vec.z;
        w -= i_Vec.w;
        return *this;
    }

    Vec4f operator-() const { return Vec4f(-x, -y, -z, -w); }

    Vec4f operator*(Float i_Factor) const { return Vec4f(x * i_Factor, y * i_Factor, z * i_Factor, w * i_Factor); }

    Vec4f& operator*=(Float i_Factor);

    Float operator*(const Vec4f& i_Vec) const { return x * i_Vec.x + y * i_Vec.y + z * i_Vec.z; }

    Vec4f operator/(Float i_Factor) const {
        Float l_Inv = 1.f / i_Factor;
        return Vec4f(x * l_Inv, y * l_Inv, z * l_Inv, w * l_Inv);
    }

    Vec4f& operator/=(Float i_Factor) {
        Float l_Inv = 1.f / i_Factor;
        x *= l_Inv;
        y *= l_Inv;
        z *= l_Inv;
        w *= l_Inv;
        return *this;
    }

    Float& operator[](int i_Index) {
        return (&x)[i_Index];
    }

    const Float& operator[](int i_Index) const {
        return (&x)[i_Index];
    }

    Bool operator==(const Vec4f& i_Vec) {
        Vec4f l_Diff = *this - i_Vec;
        return (fabsf(l_Diff.x) < Float_Eps) && (fabsf(l_Diff.y) < Float_Eps) && (fabsf(l_Diff.y) < Float_Eps);
    }

    operator Float*() { return &x; }

    Vec4f operator^(const Vec4f& i_Vec) const { return Vec4f(y * i_Vec.z - z * i_Vec.y, z * i_Vec.x - x * i_Vec.z, x * i_Vec.y - y * i_Vec.x, 1.f); }

    Float GetNorm2() const { return x * x + y * y + z * z; }

    Float GetNorm() const { return fsqrt(x * x + y * y + z * z); }

    Vec4f& Normalize() { return (*this) /= fsqrt(x * x + y * y + z * z); }

    Vec3f& xyz() {
        return *(Vec3f*)&x;
    }
} Aligned_Z(16);

Extern_Z const Vec4f VEC4F_NULL;
Extern_Z const Vec4f VEC4F_UP;
Extern_Z const Vec4f VEC4F_DOWN;
Extern_Z const Vec4f VEC4F_LEFT;
Extern_Z const Vec4f VEC4F_RIGHT;
Extern_Z const Vec4f VEC4F_FRONT;
Extern_Z const Vec4f VEC4F_BACK;

inline Vec3f::Vec3f(const Vec4f& i_Vec) {
    x = i_Vec.x;
    y = i_Vec.y;
    z = i_Vec.z;
}

inline Float Vec4_HDist2(const Vec4f& i_Left, const Vec4f& i_Right) {
    Vec4f l_Delta = Vec4f(i_Left.x - i_Right.x, i_Left.y - i_Right.y, i_Left.z - i_Right.z, 1.f);
    return l_Delta.x * l_Delta.x + l_Delta.z * l_Delta.z;
}

inline Vec4f operator*(Float i_Factor, const Vec4f& i_Vec) {
    return i_Vec * i_Factor;
}

struct Mat3x3 {
public:
    union mat {
        Float m[3][4]; // 3x4 to store extra data and take advantage of alignment

        // Row2
        struct {
            Float f[2];
            UDummy dummy;
            Float v;
        } m02;

        struct {
            Float f[6];
            UDummy dummy;
            Float v;
        } m12;

        struct {
            Float f[10];
            UDummy dummy;
            Float v;
        } m22;

        // Row3
        struct {
            Float f[3];
            UDummy dummy;
        } m03;

        struct {
            Float f[7];
            UDummy dummy;
        } m13;

        struct {
            Float f[11];
            UDummy dummy;
        } m23;
    } m;

    Mat3x3() { }

    //Mat3x3(const Mat3x3& i_m);
    //Mat3x3(const Mat4x4& i_m);
    Mat3x3& SetNull();

    Mat3x3& SetIdentity() {
        m.m[0][0] = 1.f;
        m.m[0][1] = 0.f;
        m.m[0][2] = 0.f;
        m.m[0][3] = 0.f;
        m.m[1][0] = 0.f;
        m.m[1][1] = 1.f;
        m.m[1][2] = 0.f;
        m.m[1][3] = 0.f;
        m.m[2][0] = 0.f;
        m.m[2][1] = 0.f;
        m.m[2][2] = 1.f;
        m.m[2][3] = 0.f;
        return *this;
    }

    void Transp(Mat3x3& o_m) const;
    Mat3x3& operator*=(const Mat3x3& i_m);
    Mat3x3 operator*(const Mat3x3& i_m) const;

    Float Determinant() const;

    void Set(const Mat4x4& i_m);
    inline Vec2f operator*(const Vec2f& i_v) const;

    inline Vec3f operator*(const Vec3f& i_v) const {
        Vec3f l_v;

        l_v.x = m.m[0][0] * i_v.x + m.m[1][0] * i_v.y + m.m[2][0] * i_v.z;
        l_v.y = m.m[0][1] * i_v.x + m.m[1][1] * i_v.y + m.m[2][1] * i_v.z;
        l_v.z = m.m[0][2] * i_v.x + m.m[1][2] * i_v.y + m.m[2][2] * i_v.z;

        return l_v;
    }

    Vec4f operator*(const Vec4f& i_v) const {
        Vec4f l_v;

        l_v.x = m.m[0][0] * i_v.x + m.m[1][0] * i_v.y + m.m[2][0] * i_v.z;
        l_v.y = m.m[0][1] * i_v.x + m.m[1][1] * i_v.y + m.m[2][1] * i_v.z;
        l_v.z = m.m[0][2] * i_v.x + m.m[1][2] * i_v.y + m.m[2][2] * i_v.z;
        l_v.w = 1.0f;

        return l_v;
    }

    Bool operator==(const Mat3x3& i_m) const;
    Bool operator!=(const Mat3x3& i_m) const;
    const Vec4f& GetRow(const int i_x) const;
    Vec4f& GetRow(const int i_x);
    void GetEular(Vec3f& o_Eular);
} Aligned_Z(16);

struct Mat4x4 {
    Float m[4][4];

    Mat4x4() { }

    Mat4x4(const Float _s);

    Mat4x4(const Vec3f& i_Trans, const Quat& i_Rot, const Vec3f& i_Scale) {
        SetTRS(i_Trans, i_Rot, i_Scale);
    }

    Mat4x4(const Vec3f& i_Trans, const Quat& i_Rot, const Float i_Scale) {
        SetTRS(i_Trans, i_Rot, i_Scale);
    }

    Mat4x4(const Mat3x3& _Mat);

    void Set(
        Float i_00, Float i_01, Float i_02, Float i_03,
        Float i_10, Float i_11, Float i_12, Float i_13,
        Float i_20, Float i_21, Float i_22, Float i_23,
        Float i_30, Float i_31, Float i_32, Float i_33
    ) {
        m[0][0] = i_00;
        m[0][1] = i_01;
        m[0][2] = i_02;
        m[0][3] = i_03;
        m[1][0] = i_10;
        m[1][1] = i_11;
        m[1][2] = i_12;
        m[1][3] = i_13;
        m[2][0] = i_20;
        m[2][1] = i_21;
        m[2][2] = i_22;
        m[2][3] = i_23;
        m[3][0] = i_30;
        m[3][1] = i_31;
        m[3][2] = i_32;
        m[3][3] = i_33;
    }

    const Mat3x3& m3() const {
        return *(Mat3x3*)m;
    }

    Mat3x3& m3();

    Mat4x4& SetNull() {
        memset(this, 0, sizeof((*this)));
        return *this;
    }

    Mat4x4& SetIdentity() {
        SetNull();
        for (int i = 0; i < 4; i++) {
            m[i][i] = 1.0f;
        }
        return *this;
    }

    Vec3f MulHomogenous(const Vec3f& i_v) const;

    Mat4x4& operator*=(const Mat4x4& i_m);
    Mat4x4 operator*(const Mat4x4& i_m) const;

    operator Float*();
    operator const Float*() const;

    Bool operator==(const Mat4x4& i_m) const;
    Bool operator!=(const Mat4x4& i_m) const;

    const Vec4f& GetRow(const int i_x) const;
    Vec4f& GetRow(const int i_x);

    void MulWithoutTrans(const Vec3f& i_v, Vec4f& o_v) const;

    inline void MulWithoutTrans(const Vec3f& i_v, Vec3f& o_v) const {
        o_v = m3() * i_v;
    }

    void MulWithoutTrans(const Vec4f& i_v, Vec4f& o_v) const;

    void Transp(Mat4x4& _Out) const;

    inline Float GetUniformScale() const {
        Vec3f l_Scale(m[0][0], m[0][1], m[0][2]);
        return l_Scale.GetNorm();
    }

    inline void GetScale(Vec3f& o_Scale) const {
        o_Scale.x = Vec3f(m[0][0], m[0][1], m[0][2]).GetNorm();
        o_Scale.y = Vec3f(m[1][0], m[1][1], m[1][2]).GetNorm();
        o_Scale.z = Vec3f(m[2][0], m[2][1], m[2][2]).GetNorm();
    }

    const Vec4f& GetMatrixTrans4() const;

    void SetTRS(const Vec3f& i_Trans, const Quat& i_Rot, const Vec3f& i_Scale);
    void SetTRS(const Vec3f& i_Trans, const Quat& i_Rot, const Float i_Scale);

    U32 GetCRC() const;
    inline Vec3f operator*(const Vec3f& i_Vec) const;
    Vec4f operator*(const Vec4f& i_Vec) const;

    inline const Vec3f& GetMatrixTrans() const {
        return *(Vec3f*)(&m[3][0]);
    }

    Mat4x4& ReverseAxis() {
        for (U32 i = 0; i < 4; i++) {
            m[i][2] = -m[i][2];
        }

        return *this;
    }
} Aligned_Z(16);

Vec3f Mat4x4::operator*(const Vec3f& i_Vec) const {
    Vec3f l_Vec;

    l_Vec.x = m[0][0] * i_Vec.x + m[1][0] * i_Vec.y + m[2][0] * i_Vec.z + m[3][0];
    l_Vec.y = m[0][1] * i_Vec.x + m[1][1] * i_Vec.y + m[2][1] * i_Vec.z + m[3][1];
    l_Vec.z = m[0][2] * i_Vec.x + m[1][2] * i_Vec.y + m[2][2] * i_Vec.z + m[3][2];

    return l_Vec;
}

struct Quat {
    Vec3f v;
    Float w;

    inline Quat() { }

    inline Quat(Float _w, Float i_x, Float i_y, Float i_z) {
        w = _w;
        v.x = i_x;
        v.y = i_y;
        v.z = i_z;
    }

    inline Quat(Float Angle, const Vec3f& Axis) {
        Vec2f l_SinCos;
        O_SinCos(l_SinCos, Angle * 0.5f);
        xyz() = Axis * l_SinCos.x;
        w = l_SinCos.y;
    }

    inline Quat(const Quat& i_Quat) {
        w = i_Quat.w;
        v = i_Quat.v;
    }

    inline Quat& SetIdentity() {
        w = 1.0f;
        v.x = v.y = v.z = 0.0f;
        return *this;
    }

    Quat(const Vec3f& V1, const Vec3f& V2);
    Quat(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3);
    Quat(const Mat3x3& Matrix);
    Quat(const Mat4x4& Matrix);

    Quat& SetAngleAxis(const Float Angle, const Vec3f& Axis) {
        Vec2f l_SinCos;
        O_SinCos(l_SinCos, Angle * 0.5f);
        xyz() = Axis;
        w = l_SinCos.y;
        xyz() *= l_SinCos.x;
        return *this;
    }

    Quat& SetInverse() {
        v = -v;
        return *this;
    }

    void Normalize();
    void GetMatrix(Mat4x4& Mat) const;
    void GetMatrix(Mat3x3& Mat) const;
    void SetMatrix(const Mat4x4& Mat);
    void SetMatrix(const Mat3x3& Mat);

    inline Vec3f& xyz() {
        return (Vec3f&)(*this);
    }

    Quat operator+(const Quat& i_Quat) const { return Quat(w + i_Quat.w, v.x + i_Quat.v.x, v.y + i_Quat.v.y, v.z + i_Quat.v.z); }

    Quat operator-(const Quat& i_Quat) const { return Quat(w - i_Quat.w, v.x - i_Quat.v.x, v.y - i_Quat.v.y, v.z - i_Quat.v.z); }

    Quat& operator=(const Quat& i_Quat) {
        w = i_Quat.w;
        v = i_Quat.v;
        return *this;
    }

    Quat operator*(const Quat& i_Quat) const;
    Quat operator*(const Float i_Factor) const;
    Quat& operator*=(const Quat& i_Quat);

    Vec3f operator*(const Vec3f& i_Vec) const;

    Bool operator==(const Quat& i_Quat) const { return ((Vec4f(w - i_Quat.w, v.x - i_Quat.v.x, v.y - i_Quat.v.y, v.z - i_Quat.v.z).GetNorm2()) < Float_Eps) ? TRUE : FALSE; }

    Bool operator!=(const Quat& i_Quat) const { return !operator==(i_Quat); }
} Aligned_Z(16);

Extern_Z const Quat QUAT_NULL;

struct QuatComp_Z {
    S16 x;
    S16 y;
    S16 z;
    S16 w;
};

// $SABE: Tangent Binormal Vertex ? - From Monopoly MAP
struct TBVtx {
    Vec2f UnkVec2f_0x0;
    Vec2f UnkVec2f_0x8;
};

class Vec3f_S16_Z {
public:
    S16 x;
    S16 y;
    S16 z;

    void Set(const Vec3f& i_Vector);
};

Sphere_Z operator*(const Mat4x4& i_Mat, const Sphere_Z& i_Sphere);

void BuildLookAtRotation(const Vec3f& i_LookAt, const Vec3f& i_Up, Quat& o_Quat);

inline Float Vec4_Dist2(const Vec4f& i_V1, const Vec4f& i_V2) {
    Vec3f l_Delta = i_V1 - i_V2;
    return l_Delta.GetNorm2();
}

inline void Vec4_Scale(Vec4f& o_Vec, Float i_Scale, const Vec4f& i_Vec) {
    o_Vec.x = i_Vec.x * i_Scale;
    o_Vec.y = i_Vec.y * i_Scale;
    o_Vec.z = i_Vec.z * i_Scale;
    o_Vec.w = i_Vec.w * i_Scale;
}

inline void Vec4_Add(Vec4f& o_Result, const Vec4f& i_Left, const Vec4f& i_Right) {
    o_Result.x = i_Left.x + i_Right.x;
    o_Result.y = i_Left.y + i_Right.y;
    o_Result.z = i_Left.z + i_Right.z;
    o_Result.w = i_Left.w + i_Right.w;
}

inline void Vec4_Sub(Vec4f& o_Vec, const Vec4f& i_A, const Vec4f& i_B) {
    o_Vec.x = i_A.x - i_B.x;
    o_Vec.y = i_A.y - i_B.y;
    o_Vec.z = i_A.z - i_B.z;
    o_Vec.w = 1.0f;
}

inline void Vec4_Cross(Vec4f& o_Vec, const Vec4f& i_A, const Vec4f& i_B) {
    o_Vec.Set(i_A.y * i_B.z - i_A.z * i_B.y, i_A.z * i_B.x - i_A.x * i_B.z, i_A.x * i_B.y - i_A.y * i_B.x, 1.0f);
}

inline Float Vec4_Dot(const Vec4f& i_A, const Vec4f& i_B) {
    return i_A * i_B;
}

inline void Vec4_Add_Scale(Vec4f& o_Vec, const Vec4f& i_A, Float i_Scale, const Vec4f& i_B) {
    o_Vec = i_A + (i_B * i_Scale);
}

inline void Vec4_Rotate(Vec4f& o_Vec, const Mat4x4& i_Mat, const Vec4f& i_Vec) {
    Vec4f l_Vec = i_Vec;

    o_Vec.x = i_Mat.m[0][0] * l_Vec.x + i_Mat.m[1][0] * l_Vec.y + i_Mat.m[2][0] * l_Vec.z;
    o_Vec.y = i_Mat.m[0][1] * l_Vec.x + i_Mat.m[1][1] * l_Vec.y + i_Mat.m[2][1] * l_Vec.z;
    o_Vec.z = i_Mat.m[0][2] * l_Vec.x + i_Mat.m[1][2] * l_Vec.y + i_Mat.m[2][2] * l_Vec.z;
    o_Vec.w = 1.0f;
}

inline void Vec4_Rotate(Vec4f& o_Vec, const Mat3x3& i_Mat, const Vec4f& i_Vec) {
    o_Vec.Set(i_Mat.m.m[0][0] * i_Vec.x + i_Mat.m.m[1][0] * i_Vec.y + i_Mat.m.m[2][0] * i_Vec.z, i_Mat.m.m[0][1] * i_Vec.x + i_Mat.m.m[1][1] * i_Vec.y + i_Mat.m.m[2][1] * i_Vec.z, i_Mat.m.m[0][2] * i_Vec.x + i_Mat.m.m[1][2] * i_Vec.y + i_Mat.m.m[2][2] * i_Vec.z, 1.0f);
}

template <typename T>
T Clamp(T i_Value, T i_Min, T i_Max) {
    if (i_Value <= i_Min) return i_Min;
    if (i_Value >= i_Max) return i_Max;
    return i_Value;
}

inline Double Abs(Float i_Value) {
    return fabsf(i_Value);
}

template <typename T>
inline void Smooth(
    const T& i_Current,
    const T& i_Target,
    Float i_Rate,
    Float i_DeltaTime,
    T& o_Result
) {
    Float l_Delta = i_Target - i_Current;
    l_Delta = l_Delta >= 0.0f ? l_Delta : -l_Delta;

    if (l_Delta < Float_Eps) {
        o_Result = i_Target;
    }
    else {
        Float l_Pow = (Float)pow(i_Rate, i_DeltaTime / 0.033f);
        o_Result = (i_Target - i_Current) * (1.0f - 1.0f / l_Pow) + i_Current;
    }
}

inline void Smooth(const Vec3f& i_Current, const Vec3f& i_Target, Float i_Rate, Float i_DeltaTime, Vec3f& o_Result) {
    if ((i_Target - i_Current).GetNorm2() < Float_Eps) {
        o_Result = i_Target;
    }
    else {
        Float l_Pow = (Float)pow(i_Rate, i_DeltaTime / 0.033f);
        Float l_Ratio = 1.0f - 1.0f / l_Pow;
        Vec3f l_Delta = i_Target - i_Current;
        o_Result = l_Delta * l_Ratio + i_Current;
    }
}

template <typename T>
void FastSmooth(
    const T& i_Current,
    const T& i_Target,
    Float i_Rate,
    Float i_DeltaTime,
    T& o_Result
) {
    Float l_Weight = 1.0f;
    Float l_Rate = i_Rate / 0.033f;
    Float l_Inv = l_Weight - l_Weight / i_Rate;

    Float l_Factor = 0.5f * (i_DeltaTime * (l_Rate * (l_Weight * l_Inv)));

    if (l_Factor > 1.0f) {
        l_Weight = 1.0f;
    }
    else {
        l_Weight = i_DeltaTime * (l_Rate * ((1.0f - l_Factor) * l_Inv));

        if (l_Weight > 1.0f) {
            l_Weight = 1.0f;
        }
    }

    o_Result = (i_Target - i_Current) * l_Weight + i_Current;
}

template <typename T>
T Min(T i_V1, T i_V2) {
    if (i_V1 < i_V2)
        return i_V1;
    else
        return i_V2;
}

template <typename T>
T Max(T i_V1, T i_V2) {
    if (i_V1 > i_V2)
        return i_V1;
    else
        return i_V2;
}

inline void Vec4_Max(Vec4f& o_Vec, const Vec4f& i_V1, const Vec4f& i_V2) {
    o_Vec.x = Max(i_V1.x, i_V2.x);
    o_Vec.y = Max(i_V1.y, i_V2.y);
    o_Vec.z = Max(i_V1.z, i_V2.z);
    o_Vec.w = Max(i_V1.w, i_V2.w);
}

inline void Vec4_Min(Vec4f& o_Vec, const Vec4f& i_V1, const Vec4f& i_V2) {
    o_Vec.x = Min(i_V1.x, i_V2.x);
    o_Vec.y = Min(i_V1.y, i_V2.y);
    o_Vec.z = Min(i_V1.z, i_V2.z);
    o_Vec.w = Min(i_V1.w, i_V2.w);
}

inline Vec4f MinVec(const Vec4f& i_Left, const Vec4f& i_Right) {
    return Vec4f(
        Min(i_Left.x, i_Right.x),
        Min(i_Left.y, i_Right.y),
        Min(i_Left.z, i_Right.z),
        Min(i_Left.w, i_Right.w)
    );
}

inline void MinVec(const Vec4f& i_Left, const Vec4f& i_Right, Vec4f& o_Result) {
    o_Result.x = Min(i_Left.x, i_Right.x);
    o_Result.y = Min(i_Left.y, i_Right.y);
    o_Result.z = Min(i_Left.z, i_Right.z);
    o_Result.w = Min(i_Left.w, i_Right.w);
}

inline Vec4f MaxVec(const Vec4f& i_Left, const Vec4f& i_Right) {
    return Vec4f(
        Max(i_Left.x, i_Right.x),
        Max(i_Left.y, i_Right.y),
        Max(i_Left.z, i_Right.z),
        Max(i_Left.w, i_Right.w)
    );
}

inline void MaxVec(const Vec4f& i_Left, const Vec4f& i_Right, Vec4f& o_Result) {
    o_Result.x = Max(i_Left.x, i_Right.x);
    o_Result.y = Max(i_Left.y, i_Right.y);
    o_Result.z = Max(i_Left.z, i_Right.z);
    o_Result.w = Max(i_Left.w, i_Right.w);
}

void Inverse2(const Mat4x4& i_Mat, Mat4x4& o_Mat);
Float ComputeMathPrecision();
void ConcatMat4x4(const Mat4x4&, const Mat4x4&, Mat4x4&);
void ConcatMat4x4AndConvert(const Mat4x4& a1, const Mat4x4& a2, Mat3x3& a3);
void BuildLookAtLH(const Vec3f& i_Position, const Vec3f& i_Target, const Vec3f& i_Up, Mat4x4& o_Matrix);

typedef DynArray_Z<Vec3f_S16_Z, 32, FALSE, FALSE> Vec3f_S16_ZDA;
typedef DynArray_Z<Vec3f, 32, FALSE, FALSE, 32> Vec3fDA;
typedef DynArray_Z<Vec2f, 32, FALSE, FALSE, 32> Vec2fDA;
typedef DynArray_Z<Vec4f, 32, FALSE, FALSE, 32> Vec4fDA;
typedef DynArray_Z<TBVtx, 32, FALSE, FALSE> TBVtxDA;

#endif
