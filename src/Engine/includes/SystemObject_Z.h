#ifndef _SYSTEMOBJECT_Z_H_
#define _SYSTEMOBJECT_Z_H_
#include "Math_Z.h"

class BaseObject_Z;

struct Box_Z;
struct Sphere_Z;
struct Cylindre_Z;
struct Capsule_Z;
struct Segment_Z;

struct Box_Z {
    Mat3x3 Mat; // $SABE: Mat3x3 is actually Float[3][4]. 3x3 is pure rotation, center is [0][3],[1][3],[2][3]
    Vec3f Scale;
    Float Radius;

    // $SABE TODO: Not getting generated
    Box_Z()
        : Mat()
        , Scale() { };

    Box_Z(const Quat& i_Rot, const Vec3f& i_Trans, const Vec3f& i_Size);
    void Set(const Quat& i_Rot, const Vec3f& i_Trans, const Vec3f& i_Size);
    void Set(const Vec4f& i_Right, const Vec4f& i_Up, const Vec4f& i_Front, const Vec3f& i_Trans, const Vec3f& i_Size);
    void Set(const Vec3f& i_Right, const Vec3f& i_Up, const Vec3f& i_Front, const Vec3f& i_Trans, const Vec3f& i_Size);
    void Set(const Sphere_Z& i_Sph);
    Box_Z& operator=(const Box_Z& i_Box);
    Box_Z& operator+=(const Box_Z& i_Box);
    Box_Z& operator+=(const Sphere_Z& i_Sph);
    Box_Z operator*(const Mat4x4& i_Mat) const;
    void MatMulHasScale(const Box_Z& i_Box, const Mat4x4& i_Mat, Float i_Scale, Float i_InvScale);
    void Build(const Vec3f& i_Center, const Vec3f& i_YAxis, const Vec3f* i_PointList, U32 i_NumPoint, Bool i_bAllowTinyBoxes = FALSE);
    void GetVtx(Vec3f* o_VtxArr) const;

    inline void GetTranslation(Vec3f& o_Trans) const { o_Trans.x = Mat.m.m[0][3], o_Trans.y = Mat.m.m[1][3], o_Trans.z = Mat.m.m[2][3]; }

    inline void SetTranslation(const Vec3f& i_Trans) { Mat.m.m[0][3] = i_Trans.x, Mat.m.m[1][3] = i_Trans.y, Mat.m.m[2][3] = i_Trans.z; }

    inline void GetTranslation(Vec4f& o_Trans) const { o_Trans.x = Mat.m.m[0][3], o_Trans.y = Mat.m.m[1][3], o_Trans.z = Mat.m.m[2][3], o_Trans.w = 0.f; }

    inline void SetTranslation(const Vec4f& i_Trans) { Mat.m.m[0][3] = i_Trans.x, Mat.m.m[1][3] = i_Trans.y, Mat.m.m[2][3] = i_Trans.z; }
};

typedef DynArray_Z<Box_Z, 8, FALSE, FALSE> Box_ZDA;

struct Sphere_Z {
    Vec3f Center;
    Float Radius;

    // TODO: Implement
    Sphere_Z operator+(const Sphere_Z& i_Sphere) const {
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        Sphere_Z l_Result;
        return l_Result;
    }

    Sphere_Z& operator=(const Sphere_Z& i_Sph) {
        Center.x = i_Sph.Center.x;
        Center.y = i_Sph.Center.y;
        Center.z = i_Sph.Center.z;
        Radius = i_Sph.Radius;
        return *this;
    }

    void Set(const Vec3f* i_VecPtr, S32 i_NbPoint);

    inline void Set(const Vec3f& i_Center, Float i_Radius) {
        Center.x = i_Center.x;
        Center.y = i_Center.y;
        Center.z = i_Center.z;
        Radius = i_Radius;
    }
} Aligned_Z(16);

struct ClipSphere_Z {
    Sphere_Z Sph;
    U32 Flag;
};

struct FastBox_Z {
    Mat4x4 Mat;
    Mat4x4 RotMat;
    Vec4f Trans;
    Vec4f Scale;
    Float Radius;
} Aligned_Z(16);

struct BoxFlag_Z {
    FastBox_Z Box;
    U32 Flag;
    BaseObject_Z* Object; // $SABE: Not sure if it's really here
};

// $SABE: Reversed by chatgpt, could be totally wrong
struct CollisionFastTriangle_Z {
    Vec4f m_PlaneNormal;     // 0x00
    Float m_PlaneD;          // 0x10
    Float m_CentroidRadius;  // 0x14
    Float m_GramDetAbs;      // 0x18
    S32 m_IsDegenerate;      // 0x1C
    U8 m_Unk_0x20[0x10];     // 0x20
    Vec4f m_Centroid;        // 0x30
    Vec4f m_V0;              // 0x40
    Vec4f m_V1;              // 0x50
    Vec4f m_V2;              // 0x60
    Vec4f m_Edge01;          // 0x70
    Vec4f m_Edge02;          // 0x80
    Vec4f m_Edge12;          // 0x90
    Vec4f m_RawNormal;       // 0xA0
    Vec4f m_ScaledPlane;     // 0xB0
    Vec4f m_EdgePlane01;     // 0xC0
    Vec4f m_EdgePlane12;     // 0xD0
    Vec4f m_EdgePlane20;     // 0xE0
    Float m_Edge01LenSq;     // 0xF0
    Float m_Edge01DotEdge02; // 0xF4
    Float m_Edge02LenSq;     // 0xF8
    Float m_NormalLength;    // 0xFC
    Float m_InvNormalLength; // 0x100
} Aligned_Z(16);

struct TriangleFlag_Z {
    CollisionFastTriangle_Z Triangle;
    U32 Flag;
    BaseObject_Z* Object; // $SABE: Not sure if it's really here
};

struct SphereFlag_Z {
    Sphere_Z Sph;
    U32 Flag;
    BaseObject_Z* Object; // $SABE: Not sure if it's really here
};

struct Rect_Z {
    S16 x1;
    S16 y1;
    S16 x2;
    S16 y2;

    Rect_Z() { }

    Rect_Z(S16 _x1, S16 _y1, S16 _x2, S16 _y2) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    }

    void Set(S16 _x1, S16 _y1, S16 _x2, S16 _y2) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    }
};

struct Vec2fRect_Z {
    Vec2f m_BMin;
    Vec2f m_BMax;
};

#define DEFAULT_NOCULLED 10001.f

class CullCone_Z {
public:
    Vec3f m_Origin;
    Float m_Length;
    Vec3f m_Direction;
    Float m_AngleCos;

    Bool IsCulled(const Vec4f& i_Position);

    void NoCulling() { m_Length = DEFAULT_NOCULLED; }
};

struct Capsule_Z {
    Vec3f Origin;
    Float Length;
    Vec3f Direction;
    Float Radius;
};

struct Segment_Z {
    Vec3f Origin;
    Float Length;
    Vec3f Direction;
    Float Pad;
};

struct Cylindre_Z {
    Cylindre_Z() {
        Radius = 1.0f;
    }

    Segment_Z Seg;
    Float Radius;
};

#endif
