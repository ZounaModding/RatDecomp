#include "Keyframer_Z.h"

// $SABE TODO: Finish matching
void KeyVec3fComp_Z::Set(Vec3f const& i_Value, Vec3f const& i_TgtIn, Vec3f const& i_TgtOut) {
    m_ValueX = i_Value.x * KEY_VEC_COMP_FACTOR;
    m_ValueY = i_Value.y * KEY_VEC_COMP_FACTOR;
    m_ValueZ = i_Value.z * KEY_VEC_COMP_FACTOR;
    m_TgtInX = i_TgtIn.x * KEY_VEC_COMP_FACTOR;
    m_TgtInY = i_TgtIn.y * KEY_VEC_COMP_FACTOR;
    m_TgtInZ = i_TgtIn.z * KEY_VEC_COMP_FACTOR;
    m_TgtOutX = i_TgtOut.x * KEY_VEC_COMP_FACTOR;
    m_TgtOutY = i_TgtOut.y * KEY_VEC_COMP_FACTOR;
    m_TgtOutZ = i_TgtOut.z * KEY_VEC_COMP_FACTOR;
}
