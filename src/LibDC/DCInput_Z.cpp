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
    AddDevice();
    return TRUE;
}

void DCInput_Z::Shut() {
}

void DCInput_Z::AddDevice() {
    S32 l_DeviceIdx = m_Devices.GetSize();
    if (l_DeviceIdx >= DC_MAX_CONTROLLERS) {
        return;
    }
    m_Devices.Add();
    m_Devices[l_DeviceIdx].m_Status = PAD_CONNECTED;
    m_Devices[l_DeviceIdx].Reset();
}

void DCInput_Z::RemoveDevice(S32 a1) {
}

void DCInput_Z::UpdateInput(Float i_DeltaTime) {
    InputPlatForm_Z::UpdateInput(i_DeltaTime);
    for (S16 i = 0; i < m_Devices.GetSize(); i++) {
        m_Controllers[i] = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
        m_Devices[i].Reset();
        UpdatePaddle(i);
    }
}

Float DCInput_Z::GetControl(InputDevice_Z* i_Device, S32 i_ControlId, void* i_ControllerData, Bool i_Unknown) {
    Float l_Result = 0.0f;
    cont_state_t* l_State = (cont_state_t*)i_ControllerData;

    switch (i_ControlId) {
        case BUTTON_A:
            l_Result = l_State->a ? 255 : 0;
            break;
        case BUTTON_B:
            l_Result = l_State->b ? 255 : 0;
            break;
        case BUTTON_X:
            l_Result = l_State->x ? 255 : 0;
            break;
        case BUTTON_Y:
            l_Result = l_State->y ? 255 : 0;
            break;
        case BUTTON_L:
            l_Result = l_State->ltrig;
            break;
        case BUTTON_R:
            l_Result = l_State->rtrig;
            break;
        case BUTTON_START:
            l_Result = l_State->start ? 255 : 0;
            break;
        case BUTTON_LEFT:
            l_Result = l_State->dpad_left ? 255 : 0;
            break;
        case BUTTON_RIGHT:
            l_Result = l_State->dpad_right ? 255 : 0;
            break;
        case BUTTON_UP:
            l_Result = l_State->dpad_up ? 255 : 0;
            break;
        case BUTTON_DOWN:
            l_Result = l_State->dpad_down ? 255 : 0;
            break;
        case BUTTON_LANALOG_LEFT:
            l_Result = -Clamp(5.5f * l_State->joyx, -255.0f, 0.0f);
            break;
        case BUTTON_LANALOG_RIGHT:
            l_Result = Clamp(5.5f * l_State->joyx, 0.0f, 255.0f);
            break;
        case BUTTON_LANALOG_UP:
            l_Result = Clamp(5.5f * l_State->joyy, 0.0f, 255.0f);
            break;
        case BUTTON_LANALOG_DOWN:
            l_Result = -Clamp(5.5f * l_State->joyy, -255.0f, 0.0f);
            break;
        case BUTTON_RANALOG_LEFT:
            l_Result = -Clamp(6.5f * l_State->joy2x, -255.0f, 0.0f);
            break;
        case BUTTON_RANALOG_RIGHT:
            l_Result = Clamp(6.5f * l_State->joy2x, 0.0f, 255.0f);
            break;
        case BUTTON_RANALOG_UP:
            l_Result = Clamp(6.5f * l_State->joy2y, 0.0f, 255.0f);
            break;
        case BUTTON_RANALOG_DOWN:
            l_Result = -Clamp(6.5f * l_State->joy2y, -255.0f, 0.0f);
            break;
    }

    return l_Result / 255.0f;
}

Bool DCInput_Z::UpdatePaddle(S16 i_PadIdx) {
    S32 l_PadIdx = i_PadIdx;
    InputDevice_Z& l_Device = m_Devices[l_PadIdx];
    maple_device_t* l_Controller = m_Controllers[l_PadIdx];

    if (!l_Controller || !cont_has_capabilities(l_Controller, CONT_TYPE_STANDARD_CONTROLLER)) {
        m_Devices[l_PadIdx].m_Status = PAD_DISCONNECTED;
        return FALSE;
    }

    m_Devices[l_PadIdx].m_Status = PAD_CONNECTED;
    cont_state_t* l_State = (cont_state_t*)maple_dev_status(l_Controller);
    Bool l_IsDualAnalog = cont_has_capabilities(l_Controller, CONT_CAPABILITIES_DUAL_ANALOG);

    if (l_State->dpad_up) {
        l_Device.m_DPadUpPressedPassed = 255;
    }
    else if (l_State->dpad_down) {
        l_Device.m_DPadDownPressedPassed = 255;
    }
    if (l_State->dpad_left) {
        l_Device.m_DPadLeftPressedPassed = 255;
    }
    else if (l_State->dpad_right) {
        l_Device.m_DPadRightPressedPassed = 255;
    }

    if (l_State->y) l_Device.m_YPressedPassed = 255;
    if (l_State->x) l_Device.m_XPressedPassed = 255;
    if (l_State->a) l_Device.m_APressedPassed = 255;
    if (l_State->b) l_Device.m_BPressedPassed = 255;
    if (l_State->ltrig) l_Device.m_TriggerL1PressedPassed = 255;
    if (l_State->rtrig) l_Device.m_TriggerR1PressedPassed = 255;
    if (l_State->start) l_Device.m_StartPressedPassed = 255;
    if (l_State->z) l_Device.m_TriggerZPressedPassed = 255;

    l_Device.m_StickXValue = (S16)(5.5f * l_State->joyx);
    l_Device.m_StickYValue = (S16)(5.5f * l_State->joyy);
    l_Device.m_SubStickXValue = (S16)(6.5f * l_State->joy2x);
    l_Device.m_SubStickYValue = (S16)(6.5f * l_State->joy2y);

    if (l_Device.m_DPadRightPressedPassed) l_Device.m_StickXValue = 255;
    if (l_Device.m_DPadLeftPressedPassed) l_Device.m_StickXValue = -255;
    if (l_Device.m_DPadUpPressedPassed) l_Device.m_StickYValue = 255;
    if (l_Device.m_DPadDownPressedPassed) l_Device.m_StickYValue = -255;

    l_Device.m_StickZValue = (U16)l_Device.m_TriggerL1PressedPassed - (U16)l_Device.m_TriggerR1PressedPassed;

    if ((gData.m_EngineFlag & FL_ENABLE_L2R2) && l_Device.m_TriggerL1PressedPassed && l_Device.m_TriggerR1PressedPassed) {
        gData.Cons->InterpCommand("Source DCUser.tsc");
    }

    GetControls(&l_Device, l_State, FALSE);

    l_Device.m_APressed = l_Device.m_APressedPassed;
    l_Device.m_XPressed = l_Device.m_XPressedPassed;
    l_Device.m_YPressed = l_Device.m_YPressedPassed;
    l_Device.m_BPressed = l_Device.m_BPressedPassed;
    l_Device.m_TriggerZPressed = l_Device.m_TriggerZPressedPassed;
    l_Device.m_StartPressed = l_Device.m_StartPressedPassed;
    l_Device.m_DPadUpPressed = l_Device.m_DPadUpPressedPassed;
    l_Device.m_DPadDownPressed = l_Device.m_DPadDownPressedPassed;
    l_Device.m_DPadLeftPressed = l_Device.m_DPadLeftPressedPassed;
    l_Device.m_DPadRightPressed = l_Device.m_DPadRightPressedPassed;
    l_Device.m_TriggerLBPressed = l_Device.m_TriggerL1PressedPassed;
    l_Device.m_TriggerRBPressed = l_Device.m_TriggerR1PressedPassed;
    l_Device.m_TriggerLTPressed = l_Device.m_TriggerL1PressedPassed;
    l_Device.m_TriggerRTPressed = l_Device.m_TriggerR1PressedPassed;
    l_Device.UpdateAllButtons();
    return FALSE;
}

void DCInput_Z::Vibration(S32 a1, U8 a2, U8 a3) {
}

void DCInput_Z::IsButtonPressed(U8 a1) {
}

S32 DCInput_Z::GetDeviceStatus(S32 i_DeviceIdx, S32 i_Unused) {
    if (i_DeviceIdx >= 0 && i_DeviceIdx < DC_MAX_CONTROLLERS) {
        return m_Devices[i_DeviceIdx].m_Status;
    }

    return PAD_DISCONNECTED;
}

void DCInput_Z::ResetPads() {
}
