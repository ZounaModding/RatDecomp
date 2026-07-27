#include "DCInput_Z.h"
#include "DCMain_Z.h"
#include "Console_Z.h"
#include "InputAction_Z.h"

Bool AddInputDevice() {
    gData.InputMgr->AddDevice();
    return TRUE;
}

DCInput_Z::DCInput_Z() {
    REGISTERCOMMANDC("AddInputDevice", AddInputDevice, "Adds a gamepad.");
}

DCInput_Z::~DCInput_Z() {
}

Bool DCInput_Z::Init() {
    InputPlatForm_Z::Init();
    // TODO: DC init
    AddDevice();
    return TRUE;
}

void DCInput_Z::Shut() {
}

void DCInput_Z::AddDevice() {
    S32 l_DeviceIdx = m_Devices.GetSize();
    // TODO: Check enough controllers
    m_Devices.Add();
    m_Devices[l_DeviceIdx].m_Status = 0;
    m_Devices[l_DeviceIdx].Reset();
}

void DCInput_Z::RemoveDevice(S32 a1) {
}

void DCInput_Z::UpdateInput(Float i_DeltaTime) {
    InputPlatForm_Z::UpdateInput(i_DeltaTime);
    // TODO: DC read input

    for (S16 i = 0; i < m_Devices.GetSize(); i++) {
        m_Devices[i].Reset();
        UpdatePaddle(i);
    }
}

void DCInput_Z::ResetPad(S16 i_PadIdx) {
    // TODO: DC reset input
}

void InputPlatForm_Z::Minimize() {
    m_Devices.Minimize();
    m_ActionButtonMappings.Minimize();
    m_RegisteredInputActionContexts.Minimize();
    m_ActiveInputActionContexts.Minimize();
}

Float DCInput_Z::GetControl(InputDevice_Z* i_Device, S32 i_ControlId, void* i_ControllerData, Bool i_Unknown) {
    Float l_Result = 0.0f;

    // TODO: Get result from input

    return l_Result / 255.0f;
}

Bool DCInput_Z::UpdatePaddle(S16 i_PadIdx) {
    // TODO: Update input bools
    return FALSE;
}

void DCInput_Z::Vibration(S32 a1, U8 a2, U8 a3) {
}

void DCInput_Z::IsButtonPressed(U8 a1) {
}

S32 DCInput_Z::GetDeviceStatus(S32 i_DeviceIdx, S32 i_Unused) {
    if (i_DeviceIdx >= 0 && i_DeviceIdx < 4) {
        return m_Devices[i_DeviceIdx].m_Status;
    }

    return 1;
}

void DCInput_Z::ResetPads() {
}
