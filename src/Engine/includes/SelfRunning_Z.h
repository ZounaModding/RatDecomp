#ifndef _SELFRUNNING_Z_H_
#define _SELFRUNNING_Z_H_
#include "Manipulator_Z.h"

struct SelfRunningKey {
    Float m_TimeSpentRunning;
    Float m_UnkFloat_0x4;
    Float m_UnkFloat_0x8;
    Float m_UnkFloat_0xC;
    Float m_UnkFloat_0x10;
    Float m_UnkFloat_0x14;
    Float m_UnkFloat_0x18;
    Float m_UnkFloat_0x1C;
};

class SelfRunning_Z : public Manipulator_Z {
public:
    SelfRunning_Z();

    virtual ~SelfRunning_Z() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(60) SelfRunning_Z; }

private:
    S32 m_UnkS32_0x20;
    U8 m_State;
    DynArray_Z<SelfRunningKey, 64> m_Keys;
    S32 m_FrameCount;
};

#endif // _SELFRUNNING_Z_H_
