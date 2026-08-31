#include "Collision_Z.h"

void Vec3f_S16_Z::Set(const Vec3f& i_Vector) {
    x = (S16)(1024.0f * i_Vector.x + 0.5f);
    y = (S16)(1024.0f * i_Vector.y + 0.5f);
    z = (S16)(1024.0f * i_Vector.z + 0.5f);
}
