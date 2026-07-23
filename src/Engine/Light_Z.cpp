#include "Light_Z.h"
#include "LightData_Z.h"

Light_Z::Light_Z() {
}

void Light_Z::Clean() {
}

void Light_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
}

LightData_Z::LightData_Z() {
    m_LightFlag = FL_IS_LIGHT_ACTIVE;
    m_Direction = VEC3F_NULL;
    m_Ambient = VEC3F_NULL;
    m_Color = VEC3F_ONE;
}

void LightData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
}
