#include "GCInput_Z.h"
#include "GCMain_Z.h"
#include "Console_Z.h"
#include "InputAction_Z.h"
#include <si.h>

Bool AddInputDevice() {
    gData.InputMgr->AddDevice();
    return TRUE;
}

GCInput_Z::GCInput_Z() {
    REGISTERCOMMANDC("AddInputDevice", AddInputDevice, "Adds a gamepad.");
}

Bool GCInput_Z::Init() {
    InputPlatForm_Z::Init();
    PADSetAnalogMode(3);
    PADInit();
    AddDevice();
    return TRUE;
}

void GCInput_Z::AddDevice() {
    S32 l_DeviceIdx = m_Devices.GetSize();
    if (l_DeviceIdx >= PAD_MAX_CONTROLLERS) {
        return;
    }
    m_Devices.Add();
    m_Devices[l_DeviceIdx].m_Status = 0;
    m_Devices[l_DeviceIdx].Reset();
}

void GCInput_Z::UpdateInput(Float i_DeltaTime) {
    InputPlatForm_Z::UpdateInput(i_DeltaTime);
    PADRead(m_PadStatus);
    PADClampCircle(m_PadStatus);

    for (S16 i = 0; i < m_Devices.GetSize(); i++) {
        m_Devices[i].Reset();
        UpdatePaddle(i);
    }
}

void GCInput_Z::ResetPad(S16 i_PadIdx) {
    switch (i_PadIdx) {
        case 0:
            PADReset(PAD_CHAN0_BIT);
            break;
        case 1:
            PADReset(PAD_CHAN1_BIT);
            break;
        case 2:
            PADReset(PAD_CHAN2_BIT);
            break;
        case 3:
            PADReset(PAD_CHAN3_BIT);
            break;
    }
}

Float GCInput_Z::GetControl(InputDevice_Z* i_Device, S32 i_ControlId, void* i_ControllerData, Bool i_Unknown) {
    PADStatus* l_ControllerData = (PADStatus*)i_ControllerData;
    Float l_Result = 0.0f;

    switch (i_ControlId) {
        case 0:
            l_Result = (l_ControllerData->button & PAD_BUTTON_A) ? 255 : 0;
            break;
        case 1:
            l_Result = (l_ControllerData->button & PAD_BUTTON_B) ? 255 : 0;
            break;
        case 2:
            l_Result = (l_ControllerData->button & PAD_BUTTON_X) ? 255 : 0;
            break;
        case 3:
            l_Result = (l_ControllerData->button & PAD_BUTTON_Y) ? 255 : 0;
            break;
        case 4:
            l_Result = (l_ControllerData->button & PAD_TRIGGER_Z) ? 255 : 0;
            break;
        case 5:
            l_Result = l_ControllerData->triggerLeft;
            break;
        case 6:
            l_Result = l_ControllerData->triggerRight;
            break;
        case 7:
            l_Result = (l_ControllerData->button & PAD_BUTTON_START) ? 255 : 0;
            break;
        case 8:
            l_Result = (l_ControllerData->button & PAD_BUTTON_LEFT) ? 255 : 0;
            break;
        case 9:
            l_Result = (l_ControllerData->button & PAD_BUTTON_RIGHT) ? 255 : 0;
            break;
        case 10:
            l_Result = (l_ControllerData->button & PAD_BUTTON_UP) ? 255 : 0;
            break;
        case 11:
            l_Result = (l_ControllerData->button & PAD_BUTTON_DOWN) ? 255 : 0;
            break;
        case 12:
            l_Result = -Clamp(5.5f * l_ControllerData->stickX, -255.0f, 0.0f);
            break;
        case 13:
            l_Result = Clamp(5.5f * l_ControllerData->stickX, 0.0f, 255.0f);
            break;
        case 14:
            l_Result = Clamp(5.5f * l_ControllerData->stickY, 0.0f, 255.0f);
            break;
        case 15:
            l_Result = -Clamp(5.5f * l_ControllerData->stickY, -255.0f, 0.0f);
            break;
        case 16:
            l_Result = -Clamp(6.5f * l_ControllerData->substickX, -255.0f, 0.0f);
            break;
        case 17:
            l_Result = Clamp(6.5f * l_ControllerData->substickX, 0.0f, 255.0f);
            break;
        case 18:
            l_Result = Clamp(6.5f * l_ControllerData->substickY, 0.0f, 255.0f);
            break;
        case 19:
            l_Result = -Clamp(6.5f * l_ControllerData->substickY, -255.0f, 0.0f);
            break;
    }

    return l_Result / 255.0f;
}

Bool GCInput_Z::UpdatePaddle(S16 i_PadIdx) {
    S32 l_PadIdx = i_PadIdx;
    InputDevice_Z& l_Device = m_Devices[l_PadIdx];
    U32 l_ControllerType = SIProbe(l_PadIdx);

    switch (l_ControllerType) {
        case SI_GC_CONTROLLER:
        case SI_GC_WAVEBIRD:
            if (m_PadStatus[l_PadIdx].err == PAD_ERR_NO_CONTROLLER) {
                m_Devices[l_PadIdx].m_Status = 1;
                ResetPad(i_PadIdx);
                return FALSE;
            }
            break;
        default:
            m_PadStatus[l_PadIdx].err = PAD_ERR_NO_CONTROLLER;
            m_Devices[l_PadIdx].m_Status = 1;
            ResetPad(i_PadIdx);
            return FALSE;
    }

    m_Devices[l_PadIdx].m_Status = 0;
    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_UP) {
        l_Device.m_DPadUpPressedPassed = 255;
    }
    else if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_DOWN) {
        l_Device.m_DPadDownPressedPassed = 255;
    }

    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_LEFT) {
        l_Device.m_DPadLeftPressedPassed = 255;
    }
    else if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_RIGHT) {
        l_Device.m_DPadRightPressedPassed = 255;
    }

    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_Y) l_Device.m_YPressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_X) l_Device.m_XPressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_A) l_Device.m_APressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_B) l_Device.m_BPressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_TRIGGER_L) l_Device.m_TriggerL1PressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_TRIGGER_R) l_Device.m_TriggerR1PressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_BUTTON_START) l_Device.m_StartPressedPassed = 255;
    if (m_PadStatus[l_PadIdx].button & PAD_TRIGGER_Z) l_Device.m_TriggerZPressedPassed = 255;

    l_Device.m_StickXValue = (S16)(5.5f * m_PadStatus[l_PadIdx].stickX);
    l_Device.m_StickYValue = (S16)(5.5f * m_PadStatus[l_PadIdx].stickY);
    l_Device.m_SubStickXValue = (S16)(6.5f * m_PadStatus[l_PadIdx].substickX);
    l_Device.m_SubStickYValue = (S16)(6.5f * m_PadStatus[l_PadIdx].substickY);

    if (l_Device.m_DPadRightPressedPassed) l_Device.m_StickXValue = 255;
    if (l_Device.m_DPadLeftPressedPassed) l_Device.m_StickXValue = -255;
    if (l_Device.m_DPadUpPressedPassed) l_Device.m_StickYValue = 255;
    if (l_Device.m_DPadDownPressedPassed) l_Device.m_StickYValue = -255;

    l_Device.m_StickZValue = (U16)l_Device.m_TriggerL1PressedPassed - (U16)l_Device.m_TriggerR1PressedPassed;

    if ((gData.m_EngineFlag & FL_ENABLE_L2R2) && l_Device.m_TriggerL1PressedPassed && l_Device.m_TriggerR1PressedPassed) {
        gData.Cons->InterpCommand("Source GCUser.tsc");
    }

    GetControls(&l_Device, &m_PadStatus[l_PadIdx], FALSE);

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

S32 GCInput_Z::GetDeviceStatus(S32 i_DeviceIdx, S32 i_Unused) {
    if (i_DeviceIdx >= 0 && i_DeviceIdx < 4) {
        return m_Devices[i_DeviceIdx].m_Status;
    }

    return 1;
}

void InputPlatForm_Z::Minimize() {
    m_Devices.Minimize();
    m_ActionButtonMappings.Minimize();
    m_RegisteredInputActionContexts.Minimize();
    m_ActiveInputActionContexts.Minimize();
}
