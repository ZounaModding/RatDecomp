#ifndef _MATH_Z_H_
#define _MATH_Z_H_
#include "MathTools_Z.h"
#include "Types_Z.h"
#include "Bitmap_Z.h"

#define Float_Eps 1.e-6f
#define Pi 3.14159265358979323846f
#define ROL_Z(val, shift) ((val << shift) | (val >> ((sizeof(val) * 8) - shift)))
#define ROR_Z(val, shift) ((val >> shift) | (val << ((sizeof(val) * 8) - shift)))
struct Sphere_Z;
struct Vec2f;
struct Vec3f;
struct Vec4f;
struct Quat;
struct Mat3x3;
struct Mat4x4;

void Inverse2(const Mat4x4&, Mat4x4&);

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
};

struct Vec3f {
    Float x, y, z;

    Vec3f() { };
    Vec3f(const Color& i_Color);
    Vec3f(const Quat& i_Quat);

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

    Vec3f operator=(const Vec3f& i_Vec) {
        x = i_Vec.x;
        y = i_Vec.y;
        z = i_Vec.z;
        return *this;
    }

    inline Vec3f operator=(const Vec4f& i_Vec);

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
        x *= i_Factor;
        y *= i_Factor;
        z *= i_Factor;
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

    Float GetNorm() const { return sqrtf(GetNorm2()); }

    Vec3f& Normalize() { return (*this) /= GetNorm(); }

    Float HGetNorm2() const { return x * x + z * z; }

    Float HGetNorm() const { return sqrtf(HGetNorm2()); }

    Vec3f& HNormalize() {
        *this = (Vec3f(x, 0.f, z) / HGetNorm());
        return *this;
    }
};

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
};

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

    Vec4f operator+(const Vec4f& i_Vec) const { return Vec4f(x + i_Vec.x, y + i_Vec.y, z + i_Vec.z, w + i_Vec.w); }

    Vec4f& operator+=(const Vec4f& i_Vec) {
        x += i_Vec.x;
        y += i_Vec.y;
        z += i_Vec.z;
        w += i_Vec.w;
        return *this;
    }

    Vec4f operator+() const { return *this; }

    Vec4f operator-(const Vec4f& i_Vec) const { return Vec4f(x - i_Vec.x, y - i_Vec.y, z - i_Vec.z, w - i_Vec.w); }

    Vec4f& operator-=(const Vec4f& i_Vec) {
        x -= i_Vec.x;
        y -= i_Vec.y;
        z -= i_Vec.z;
        w -= i_Vec.w;
        return *this;
    }

    Vec4f operator-() const { return Vec4f(-x, -y, -z, -w); }

    Vec4f operator*(Float i_Factor) const { return Vec4f(x * i_Factor, y * i_Factor, z * i_Factor, w * i_Factor); }

    Vec4f& operator*=(Float i_Factor) {
        x *= i_Factor;
        y *= i_Factor;
        z *= i_Factor;
        w *= i_Factor;
        return *this;
    }

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

    Float GetNorm() const { return sqrtf(x * x + y * y + z * z); }

    Vec4f& Normalize() { return (*this) /= sqrtf(x * x + y * y + z * z); }
};

inline Vec4f operator*(Float i_Factor, const Vec4f& i_Vec) {
    return i_Vec * i_Factor;
}

struct Mat3x3 {
public:
    union mat {
        Float m[3][4];

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
    Mat3x3& SetIdentity();
    void Transp(Mat3x3& o_m) const;
    Mat3x3& operator=(const Mat3x3& i_m);
    Mat3x3& operator*=(const Mat3x3& i_m);
    Mat3x3 operator*(const Mat3x3& i_m) const;

    Float Determinant() const;

    void Set(const Mat4x4& i_m);
    inline Vec2f operator*(const Vec2f& i_v) const;

    inline Vec3f operator*(const Vec3f& i_v) const;
    inline Vec4f operator*(const Vec4f& i_v) const;
    Bool operator==(const Mat3x3& i_m) const;
    Bool operator!=(const Mat3x3& i_m) const;
    const Vec4f& GetRow(const int i_x) const;
    Vec4f& GetRow(const int i_x);
    void GetEular(Vec3f& o_Eular);
};

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
    Mat4x4(const Mat4x4& _Mat);

    const Mat3x3& m3() const;
    Mat3x3& m3();

    Mat4x4& SetNull();
    Mat4x4& SetIdentity();

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
    void MulWithoutTrans(const Vec3f& i_v, Vec3f& o_v) const;
    void MulWithoutTrans(const Vec4f& i_v, Vec4f& o_v) const;

    void Transp(Mat4x4& _Out) const;

    Float GetUniformScale() const;
    void GetScale(Vec3f& o_Scale) const;
    const Vec3f& GetMatrixTrans() const;
    const Vec4f& GetMatrixTrans4() const;

    void SetTRS(const Vec3f& i_Trans, const Quat& i_Rot, const Vec3f& i_Scale);
    void SetTRS(const Vec3f& i_Trans, const Quat& i_Rot, const Float i_Scale);

    U32 GetCRC() const;
    inline Vec3f operator*(const Vec3f& i_Vec) const;
    inline Vec4f operator*(const Vec4f& i_Vec) const;

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
        w = cosf(Angle / 2.f);
        v = sinf(Angle / 2.f) * Axis;
    }

    inline Quat(const Quat& i_Quat) {
        w = i_Quat.w;
        v = i_Quat.v;
    }

    Quat(const Vec3f& V1, const Vec3f& V2);
    Quat(const Vec3f& V1, const Vec3f& V2, const Vec3f& V3);
    Quat(const Mat3x3& Matrix);
    Quat(const Mat4x4& Matrix);

    void SetAngleAxis(Float Angle, Vec3f& Axis) {
        w = cosf(Angle / 2.f);
        v = sinf(Angle / 2.f) * Axis;
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

// $SABE: Tangent Binormal Vertex ? - From Monopoly MAP
struct TBVtx {
    Vec2f UnkVec2f_0x0;
    Vec2f UnkVec2f_0x8;
};

Sphere_Z operator*(const Mat4x4& Mat, const Sphere_Z& Sphere);

template <typename T>
T Clamp(T i_Value, T i_Min, T i_Max) {
    if (i_Value <= i_Min) return i_Min;
    if (i_Value >= i_Max) return i_Max;
    return i_Value;
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

void Inverse2(const Mat4x4& i_Mat, Mat4x4& o_Mat);
#endif
