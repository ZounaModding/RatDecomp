#ifndef _PERSOLIGHT_G_H_
#define _PERSOLIGHT_G_H_
#include "Manipulator_Z.h"

class PersoLight_G : public Manipulator_Z {
public:
    virtual ~PersoLight_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(75) PersoLight_G; }

private:
    U8 m_Unk_0x20[0xfc];
};
#endif // _PERSOLIGHT_G_H_
