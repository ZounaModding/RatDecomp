#include "InputAction_Z.h"
#include "InputEngine_Z.h"
#include "Program_Z.h"

void InputAction_Z::Reset() {
    m_WasPressed = FALSE;
    m_Pressed = FALSE;
    m_JustPressed = FALSE;
    m_JustReleased = FALSE;
    m_HeldTime = 0.0f;
    m_TimeSincePressed = 0.0f;
    m_TimeSinceReleased = 0.0f;
    m_ManualValueDirty = FALSE;
    m_DeviceIdx = -1;
    m_ActionId = -1;
    m_UseManualValue = FALSE;
    m_RepeatEnabled = FALSE;
    m_RepeatTimer = 0.0f;
    m_FirstRepeatDelay = 0.4f;
    m_RepeatInterval = 0.08;
    m_RepeatPulseActive = FALSE;
    m_ManualValue = 0.0f;
    m_PressThreshold = 0.5f;
    m_MinValue = 0.2f;
    m_MaxValue = 0.9f;
    m_SmoothingRate = 1.1f;
    m_ValueCurve = -1;
    m_NormalizeValue = TRUE;
    m_Value = 0.0f;
}

void InputAction_Z::Update(Float i_DeltaTime) {
    m_JustPressed = FALSE;
    m_JustReleased = FALSE;

    if (m_DeviceIdx >= 0 || m_UseManualValue) {
        if (!m_UseManualValue && !m_ManualValueDirty) {
            InputDevice_Z& l_Device = gData.InputMgr->GetDevice(m_DeviceIdx);
            DeviceAction_Z& l_DeviceAction = l_Device.GetAction(m_ActionId);
            m_Pressed = l_DeviceAction.m_Value > m_PressThreshold;

            Float l_OldValue = m_Value;
            m_Value = l_DeviceAction.m_Value;
            if (l_DeviceAction.m_SmoothValue) {
                Smooth(l_OldValue, m_Value, m_SmoothingRate, i_DeltaTime, m_Value);
            }
            else if (m_NormalizeValue) {
                if (m_Value >= m_MaxValue) {
                    m_Value = 1.0f;
                }
                else if (m_Value < m_MinValue) {
                    m_Value = 0.0f;
                }
                else {
                    m_Value = (m_Value - m_MinValue) / (m_MaxValue - m_MinValue);
                }
            }
        }
        else {
            m_Pressed = m_ManualValue > m_PressThreshold;
            m_ManualValueDirty = FALSE;
            Smooth(m_Value, m_ManualValue, m_SmoothingRate, i_DeltaTime, m_Value);
        }

        switch (m_ValueCurve) {
            case INPUT_VALUE_CURVE_DOUBLE_EXPONENTIAL:
                m_Value *= m_Value;
                break;
        }

        m_TimeSincePressed += i_DeltaTime;
        m_TimeSinceReleased += i_DeltaTime;

        if (!m_RepeatEnabled) {
            if (m_WasPressed && !m_Pressed) {
                m_JustReleased = TRUE;
                m_TimeSinceReleased = 0.0f;
            }
            if (!m_WasPressed && m_Pressed) {
                m_JustPressed = TRUE;
                m_TimeSincePressed = 0.0f;
            }
            if (m_Pressed) {
                m_HeldTime += i_DeltaTime;
            }
            else {
                m_HeldTime = 0.0f;
            }
            m_WasPressed = m_Pressed;
        }
        else if (m_Pressed) {
            if (m_WasPressed) {
                m_RepeatTimer -= i_DeltaTime;
                if (m_RepeatTimer < 0.0f) {
                    m_TimeSincePressed = 0.0f;
                    m_RepeatTimer = m_RepeatInterval;
                    m_RepeatPulseActive = TRUE;
                }
                else {
                    if (m_RepeatPulseActive) {
                        m_RepeatPulseActive = FALSE;
                        m_TimeSinceReleased = 0.0f;
                    }
                    m_Pressed = FALSE;
                }
            }
            else {
                m_RepeatTimer = m_FirstRepeatDelay;
                m_TimeSincePressed = 0.0f;
                m_RepeatPulseActive = TRUE;
            }
            m_WasPressed = TRUE;
        }
        else {
            m_RepeatTimer = 0.0f;
            m_RepeatPulseActive = FALSE;
            m_TimeSincePressed = 0.0f;
            m_WasPressed = FALSE;
        }
    }
}

S32 InputActionContext_Z::AddAction(S32 i_ActionId) {
    S32 l_ActionIdx = m_Actions.Add();
    m_Actions[l_ActionIdx].m_DeviceIdx = m_DeviceIdx;
    m_Actions[l_ActionIdx].m_ActionId = i_ActionId;
    return l_ActionIdx + m_FirstActionIdx;
}

InputAction_Z& InputActionContext_Z::GetAction(S32 i_ActionIdx) {
    return m_Actions[(U16)i_ActionIdx];
}

void InputActionContext_Z::Update(Float i_DeltaTime) {
    if (m_IsActive) {
        S32 l_Nb = m_Actions.GetSize();
        for (S32 i = 0; i < l_Nb; i++) {
            m_Actions[i].Update(i_DeltaTime);
        }
    }
}
