#ifndef _DCINPUT_Z_H_
#define _DCINPUT_Z_H_
#include "InputEngine_Z.h"
#include <dc/maple.h>
#include <dc/maple/controller.h>

#define DC_MAX_CONTROLLERS 4

class DCInput_Z : public InputPlatForm_Z {
public:
    DCInput_Z();
    virtual ~DCInput_Z();
    virtual Bool Init();
    virtual void Shut();
    virtual void AddDevice();
    virtual void ResetPads();
    virtual void RemoveDevice(S32 a1);
    virtual void UpdateInput(Float a1);
    virtual void IsButtonPressed(U8 a1);
    virtual void Vibration(S32 a1, U8 a2, U8 a3);
    virtual S32 GetDeviceStatus(S32 a1, S32 a2);
    virtual Float GetControl(InputDevice_Z* i_Device, S32 i_ControlId, void* i_ControllerData, Bool i_Unknown);
    Bool UpdatePaddle(S16 i_PadIdx);

private:
    maple_device* m_Controllers[DC_MAX_CONTROLLERS];
};

#endif // _GCINPUT_Z_H_
