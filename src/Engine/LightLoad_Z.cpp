#include "LightData_Z.h"
#include "Light_Z.h"
#include "Sys_Z.h"

void Light_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);
    LOADRANGE_Z(m_Rotation, m_Translation);
}

void LightData_Z::Load(void** i_Data) {
    ObjectDatas_Z::Load(i_Data);
    LOADRANGE_Z(m_Direction, m_Ambient);
}
