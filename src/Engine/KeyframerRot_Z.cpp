#include "Keyframer_Z.h"
#include "Types_Z.h"

void KeyRot_Z::Set(const Quat& i_Value) {
    Float l_NewX = i_Value.v.x * KEY_QUAT_COMP_FACTOR + 0.5f;
    m_ValueX = l_NewX >= 0.0f ? (int)l_NewX : -((int)-l_NewX);
    Float l_NewY = i_Value.v.y * KEY_QUAT_COMP_FACTOR + 0.5f;
    m_ValueY = l_NewY >= 0.0f ? (int)l_NewY : -((int)-l_NewY);
    Float l_NewZ = i_Value.v.z * KEY_QUAT_COMP_FACTOR + 0.5f;
    m_ValueZ = l_NewZ >= 0.0f ? (int)l_NewZ : -((int)-l_NewZ);
    Float l_NewW = i_Value.w * KEY_QUAT_COMP_FACTOR + 0.5f;
    m_ValueW = l_NewW >= 0.0f ? (int)l_NewW : -((int)-l_NewW);
}
