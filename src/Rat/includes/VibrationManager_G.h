#ifndef _VIBRATIONMANAGER_G_H_
#define _VIBRATIONMANAGER_G_H_
#include "Manipulator_Z.h"

class VibrationManager_C : public Manipulator_Z {
public:
    virtual ~VibrationManager_C() { }

    virtual void Init();
    virtual void Reset();
    virtual void ActionOnDeactivate();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(101) VibrationManager_C; }

private:
    U8 m_Unk_0x20[0x2c];
};
#endif // _VIBRATIONMANAGER_G_H_
