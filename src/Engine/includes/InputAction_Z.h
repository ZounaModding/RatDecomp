#ifndef _INPUTACTION_Z_H_
#define _INPUTACTION_Z_H_

#include "DynArray_Z.h"
#include "Main_Z.h"
#include "Math_Z.h"
#include "HoleArray_Z.h"

#define INPUT_ACTION_COUNT 96

class DeviceAction_Z {
public:
    Float m_CurrentStatus;
    Bool m_Unk_0x4;
};

class InputDevice_Z {
public:
    InputDevice_Z() {
    }

    void Reset() {
        m_YPressedPassed = 0;
        m_APressed = FALSE;
        m_XPressedPassed = 0;
        m_XPressed = FALSE;
        m_APressedPassed = 0;
        m_YPressed = FALSE;
        m_BPressedPassed = 0;
        m_BPressed = FALSE;
        m_UnkPressedPassed_0x338 = 0;
        m_TriggerZPressed = FALSE;
        m_UnkPressedPassed_0x339 = 0;
        m_StartPressed = FALSE;
        m_DPadUpPressedPassed = 0;
        m_DPadUpPressed = FALSE;
        m_DPadRightPressedPassed = 0;
        m_DPadDownPressed = FALSE;
        m_DPadLeftPressedPassed = 0;
        m_DPadLeftPressed = FALSE;
        m_DPadDownPressedPassed = 0;
        m_DPadRightPressed = FALSE;
        m_TriggerZPressedPassed = 0;
        m_TriggerLBPressed = FALSE;
        m_StartPressedPassed = 0;
        m_TriggerRBPressed = FALSE;
        m_TriggerL1PressedPassed = 0;
        m_TriggerLTPressed = FALSE;
        m_TriggerR1PressedPassed = 0;
        m_TriggerRTPressed = FALSE;
        m_TriggerL2PressedPassed = 0;
        m_UnkPressed_0x322 = FALSE;
        m_TriggerR2PressedPassed = 0;
        m_UnkPressed_0x323 = FALSE;
        m_StickZValue = 0;
        m_StickYValue = 0;
        m_StickXValue = 0;
        m_SubStickZValue = 0;
        m_SubStickYValue = 0;
        m_SubStickXValue = 0;
        m_AllButtons = 0;
        m_EcoMode = TRUE;

        for (U32 i = 0; i < INPUT_ACTION_COUNT; i++) {
            m_DeviceActions[i].m_CurrentStatus = 0.0f;
            m_DeviceActions[i].m_Unk_0x4 = FALSE;
        }
    }

    Bool UpdateAllButtons() {
        for (S32 i = 0; i < 16; i++) {
            if (m_PressedPassed[i]) {
                m_AllButtons |= 1 << i;
            }
        }
        return FALSE;
    }

    S32 m_Status;
    Bool m_EcoMode;
    U8 m_Padding_0x5[3];
    U32 m_Unk_0x8;
    DeviceAction_Z m_DeviceActions[INPUT_ACTION_COUNT];
    U32 m_Unk_0x30c;
    U32 m_Unk_0x310;
    U8 m_APressed;
    U8 m_XPressed;
    U8 m_YPressed;
    U8 m_BPressed;
    U8 m_TriggerZPressed;
    U8 m_StartPressed;
    U8 m_DPadUpPressed;
    U8 m_DPadDownPressed;
    U8 m_DPadLeftPressed;
    U8 m_DPadRightPressed;
    U8 m_TriggerLBPressed;
    U8 m_TriggerRBPressed;
    U8 m_TriggerLTPressed;
    U8 m_TriggerRTPressed;
    U8 m_UnkPressed_0x322;
    U8 m_UnkPressed_0x323;
    U32 m_AllButtons;
    S16 m_StickXValue;
    S16 m_StickYValue;
    S16 m_StickZValue;
    S16 m_SubStickXValue;
    S16 m_SubStickYValue;
    S16 m_SubStickZValue;

    union {
        struct {
            U8 m_YPressedPassed;
            U8 m_XPressedPassed;
            U8 m_APressedPassed;
            U8 m_BPressedPassed;
            U8 m_UnkPressedPassed_0x338;
            U8 m_UnkPressedPassed_0x339;
            U8 m_DPadUpPressedPassed;
            U8 m_DPadRightPressedPassed;
            U8 m_DPadLeftPressedPassed;
            U8 m_DPadDownPressedPassed;
            U8 m_TriggerZPressedPassed;
            U8 m_StartPressedPassed;
            U8 m_TriggerL1PressedPassed;
            U8 m_TriggerR1PressedPassed;
            U8 m_TriggerL2PressedPassed;
            U8 m_TriggerR2PressedPassed;
        };

        U8 m_PressedPassed[16];
    };

    U32 m_Unk_0x344[4];
};

struct InputAction_Z {
    Bool m_UnkBool_0x0;
    Bool m_UnkBool_0x1;
    Bool m_UnkBool_0x2;
    Float m_UnkFloat_0x4;
    Bool m_UnkBool_0x8;
    Bool m_UnkBool_0x9;
    Bool m_UnkBool_0xa;
    Float m_UnkFloat_0xc;
    Float m_UnkFloat_0x10;
    Float m_UnkFloat_0x14;
    Float m_BaseValue;
    Float m_MinValue;
    Float m_MaxValue;
    Float m_UnkFloat_0x24;
    S32 m_UnkS32_0x28;
    Float m_UnkFloat_0x2c;
    Bool m_Pressed;
    Bool m_UnkBool_0x31;
    Bool m_UnkBool_0x32;
    Float m_PressedTimer;
    Float m_SomeTimer_0x38;
    Float m_SomeOtherTimer_0x3c;
    S32 m_DeviceIdx;
    S32 m_ActionId;

    InputAction_Z() {
        m_UnkBool_0x0 = FALSE;
        m_Pressed = FALSE;
        m_UnkBool_0x31 = FALSE;
        m_UnkBool_0x32 = FALSE;
        m_PressedTimer = 0.0f;
        m_SomeTimer_0x38 = 0.0f;
        m_SomeOtherTimer_0x3c = 0.0f;
        m_UnkBool_0x1 = FALSE;
        m_DeviceIdx = -1;
        m_ActionId = -1;
        m_UnkBool_0x8 = FALSE;
        m_UnkBool_0x9 = FALSE;
        m_UnkFloat_0xc = 0.0f;
        m_UnkFloat_0x10 = 0.4f;
        m_UnkFloat_0x14 = 0.08f;
        m_UnkBool_0xa = FALSE;
        m_UnkFloat_0x4 = 0.0f;
        m_BaseValue = 0.5f;
        m_MinValue = 0.2f;
        m_MaxValue = 0.9f;
        m_UnkFloat_0x24 = 1.1f;
        m_UnkS32_0x28 = -1;
        m_UnkBool_0x2 = TRUE;
        m_UnkFloat_0x2c = 0.0f;
    }
};

struct InputActionContext_Z {
    S32 m_DeviceIdx;
    Bool m_AlwaysActive;
    DynArray_Z<InputAction_Z, 4> m_Actions;
    S32 m_UnkS32_0x10;
    S32 m_FirstActionIdx;

    S32 AddAction(S32 i_ActionId);
    InputAction_Z& GetAction(S32 i_ActionId);
    void Update(Float i_DeltaTime);
};

#endif // _INPUTACTION_Z_H_
