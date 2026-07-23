#include "InputEngine_Z.h"
#include "DynArray_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

#define BUTTON_COUNT 21

// $VIOLET: sinit will match when DynArray_Z is used elsewhere
static Name_Z InputButtonName[BUTTON_COUNT] = {
    Name_Z::GetID("A"),
    Name_Z::GetID("B"),
    Name_Z::GetID("X"),
    Name_Z::GetID("Y"),
    Name_Z::GetID("Z"),
    Name_Z::GetID("L"),
    Name_Z::GetID("R"),
    Name_Z::GetID("START"),
    Name_Z::GetID("LEFT"),
    Name_Z::GetID("RIGHT"),
    Name_Z::GetID("UP"),
    Name_Z::GetID("DOWN"),
    Name_Z::GetID("LANALOG_LEFT"),
    Name_Z::GetID("LANALOG_RIGHT"),
    Name_Z::GetID("LANALOG_UP"),
    Name_Z::GetID("LANALOG_DOWN"),
    Name_Z::GetID("RANALOG_LEFT"),
    Name_Z::GetID("RANALOG_RIGHT"),
    Name_Z::GetID("RANALOG_UP"),
    Name_Z::GetID("RANALOG_DOWN"),
    Name_Z::GetID("CROSS"),
};

S32 InputPlatForm_Z::FindButtonId(Name_Z i_ButtonName) {
    for (S32 i = 0; i < BUTTON_COUNT; i++) {
        if (InputButtonName[i] == i_ButtonName) {
            return i;
        }
    }
    return -1;
}

void InputPlatForm_Z::SetControl(S32 i_RemapIdx, Name_Z i_PrimaryButtonName, Name_Z i_SecondaryButtonName) {
    m_ActionButtonMappings[i_RemapIdx].m_ButtonId = FindButtonId(i_PrimaryButtonName);
    m_ActionButtonMappings[i_RemapIdx].m_SecondaryButtonId = FindButtonId(i_SecondaryButtonName);
}

void InputPlatForm_Z::SetControlMode(S32 i_RemapIdx, Bool i_ControlMode) {
    m_ActionButtonMappings[i_RemapIdx].m_ControlMode = i_ControlMode;
}

void InputPlatForm_Z::GetControls(InputDevice_Z* i_Device, void* i_ControllerData, Bool i_Unknown) {
}

void InputPlatForm_Z::IsAnyButton(U32 a1) {
}

void InputPlatForm_Z::UpdateInput(Float i_DeltaTime) {
    S32 l_Input = m_ActiveInputActionContexts.FindFirst();
    while (l_Input > -1) {
        if (!m_IsPaused || m_ActiveInputActionContexts[l_Input].m_AlwaysActive) {
            m_ActiveInputActionContexts[l_Input].Update(i_DeltaTime);
        }
        l_Input = m_ActiveInputActionContexts.FindNext(l_Input);
    }
}

void InputPlatForm_Z::SetEcoMode(S32 a1, Bool a2) {
}
