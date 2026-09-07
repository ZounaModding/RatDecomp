#include "Collision_Z.h"

void Vec3f_S16_Z::Set(const Vec3f& i_Vector) {
    x = (S16)(1024.0f * i_Vector.x + 0.5f);
    y = (S16)(1024.0f * i_Vector.y + 0.5f);
    z = (S16)(1024.0f * i_Vector.z + 0.5f);
}

void Vec3f_S16_Z::Get(Vec4f& o_Vector) const {
    o_Vector.x = x;
    o_Vector.y = y;
    o_Vector.z = z;
    o_Vector.x *= (1.0f / 1024.0f);
    o_Vector.y *= (1.0f / 1024.0f);
    o_Vector.z *= (1.0f / 1024.0f);
    o_Vector.w *= (1.0f / 1024.0f);
}

void Vec3f_S16_Z::Get(const Vec3f_S16_Z& i_V0, const Vec3f_S16_Z& i_V1, const Vec3f_S16_Z& i_V2, Vec4f& o_V0, Vec4f& o_V1, Vec4f& o_V2) {
    i_V0.Get(o_V0);
    i_V1.Get(o_V1);
    i_V2.Get(o_V2);
}
