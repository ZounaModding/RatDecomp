#ifndef _INPUTENGINE_Z_H_
#define _INPUTENGINE_Z_H_
#include "DynArray_Z.h"
#include "Main_Z.h"
#include "Math_Z.h"
#include "HoleArray_Z.h"
#include "InputAction_Z.h"

struct ButtonRemap_Z {
    S32 m_ButtonId;
    S32 m_SecondaryButtonId;
    Bool m_ControlMode;
};

// $SABE: Fake name
struct InputData_Z {
    Vec2f m_WiiMotePointerRelated1;
    Vec2f m_WiiMotePointerRelated2;
    Bool m_UnkBool_0x18;
    Bool m_UnkBool_0x19;
    Bool m_UnkBool_0x1a;
    Bool m_UnkBool_0x1b;
    Bool m_UnkBool_0x1c;
    Bool m_UnkBool_0x1d;
    Bool m_UnkBool_0x1e;
    Bool m_UnkBool_0x1f;

    InputData_Z()
        : m_UnkBool_0x18(FALSE)
        , m_UnkBool_0x19(FALSE)
        , m_UnkBool_0x1a(FALSE)
        , m_UnkBool_0x1b(FALSE)
        , m_UnkBool_0x1c(FALSE)
        , m_UnkBool_0x1d(FALSE)
        , m_UnkBool_0x1e(FALSE)
        , m_UnkBool_0x1f(FALSE) {
        m_WiiMotePointerRelated1 = VEC2F_NULL;
        m_WiiMotePointerRelated2 = VEC2F_NULL;
    }
};

class InputPlatForm_Z {
public:
    DynArray_Z<InputDevice_Z, 8> m_Devices;
    InputData_Z m_InputData;
    S32 m_UnkBytes_0x20;
    Bool m_UnkBool_0x24;
    Bool m_UnkBool_0x25;
    Bool m_IsPaused;
    HoleArray_Z<InputActionContext_Z, 4> m_RegisteredInputActionContexts;
    HoleArray_Z<InputActionContext_Z, 4> m_ActiveInputActionContexts;
    DynArray_Z<ButtonRemap_Z, 32> m_ActionButtonMappings; // for each action

public:
    InputPlatForm_Z()
        : m_UnkBytes_0x20(0) {
        m_UnkBool_0x24 = FALSE;
        m_UnkBool_0x25 = TRUE;
        m_IsPaused = FALSE;
    }

    inline S32 GetDeviceCount() const {
        return m_Devices.GetSize();
    }

    inline InputDevice_Z& GetDevice(S32 i_Index) {
        return m_Devices[i_Index];
    }

    inline void EndPause() {
        m_IsPaused = FALSE;
    }

    virtual ~InputPlatForm_Z() { }

    void SetControl(S32 i_ControlIdx, S32 i_ButtonId, S32 i_SecondaryButtonId) {
        m_ActionButtonMappings[i_ControlIdx].m_ButtonId = i_ButtonId;
        m_ActionButtonMappings[i_ControlIdx].m_SecondaryButtonId = i_SecondaryButtonId;
    }

    virtual Bool Init() {
        m_UnkBool_0x24 = FALSE;
        m_UnkBool_0x25 = TRUE;
        m_IsPaused = FALSE;

        m_ActionButtonMappings.SetSize(INPUT_ACTION_COUNT);

        for (S32 i = 0; i != INPUT_ACTION_COUNT; ++i) {
            SetControl(i, -1, -1);
            m_ActionButtonMappings[i].m_ControlMode = FALSE;
        }

        return TRUE;
    }

    virtual void Minimize();

    virtual void Shut() { }

    virtual void AddDevice() {
        // TODO: Finish matching
        m_Devices.Add();
    }

    virtual void ResetPads() { }

    virtual void RemoveDevice(S32 a1) { }

    virtual void UpdateInput(Float i_DeltaTime);

    virtual void IsButtonPressed(U8 a1) { }

    virtual void IsAnyButton(U32 a1);

    virtual void ChangeActionMapping(U32 a1, S32* a2, S32* a3) { }

    virtual void SetControl(S32 i_RemapIdx, Name_Z i_PrimaryButtonName, Name_Z i_SecondaryButtonName);
    virtual void SetControlMode(S32 i_RemapIdx, Bool i_ControlMode);

    virtual void EnableJoystick(S32 a1) { }

    virtual void DisableJoystick(S32 a1) { }

    virtual void EnableVibration(S32 a1, Bool a2) { }

    virtual void Vibration(S32 a1, U8 a2, U8 a3) { }

    virtual void SetEcoMode(S32 a1, Bool a2);

    virtual S32 GetDeviceStatus(S32 a1, S32 a2) {
        return 0;
    }

    virtual void GetPCStringFromInput(Char* a1, S32 a2, S32 a3) { }

    virtual void GetControls(InputDevice_Z* i_Device, void* i_ControllerData, Bool i_Unknown);

    virtual Float GetControl(InputDevice_Z* i_Device, S32 i_ControlId, void* i_ControllerData, Bool i_Unknown) {
        return 0.0f;
    }

    S32 UseActionContext(S32 i_ActionContext, S32 i_PreviousActionContext, S32 i_DeviceIdx);
    S32 SetActionContext(S32 i_ActionContext, S32 i_DeviceIdx);
    S32 FindButtonId(Name_Z i_ButtonName);
};

class InputEngine_Z {
public:
    virtual ~InputEngine_Z() { }

    virtual void Update(Float i_DeltaTime) { }

    virtual void Minimize() { }

protected:
    S32DA m_OldInput; // JIMMY DWARF
};

#endif
