#ifndef _POINTJUMP_G_H_
#define _POINTJUMP_G_H_
#include "Manipulator_Z.h"

class PointJump_G : public Manipulator_Z {
public:
    virtual ~PointJump_G() { }

    virtual void Init();
    virtual void Reset();

    virtual void Update(Float i_DeltaTime) { }

    static BaseObject_Z* NewObject() { return NewL_Z(53) PointJump_G; }

    void Minimize();

private:
    U8 m_Unk_0x20[0xc];
};
#endif // _POINTJUMP_G_H_
