#ifndef _CAMERAUSER_Z_H_
#define _CAMERAUSER_Z_H_
#include "Manipulator_Z.h"

class CameraUser_Z : public Manipulator_Z {
public:
    virtual void Init();

    virtual ~CameraUser_Z() { }

    virtual void Update(Float i_DeltaTime) { }

    virtual void Wheel(Float i_DeltaTime);

    static BaseObject_Z* NewObject() {
        return NewL_Z(22) CameraUser_Z;
    }

    void SetInputNum(const S32 i_InputNum) {
        m_InputNum = i_InputNum;
    }

    void SetViewportId(const S32 i_ViewportId) {
        m_ViewportId = i_ViewportId;
    }

    S32 m_InputNum;
    S32 m_ViewportId;
    S32 m_UnkS32_0x28;
};

#endif // _CAMERAUSER_Z_H_
