#include "CameraUser_Z.h"

void CameraUser_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_camera);
    m_InputNum = 0;
    m_ViewportId = 0;
    m_UnkS32_0x28 = 0;
}

void CameraUser_Z::Wheel(Float i_DeltaTime) {
}
