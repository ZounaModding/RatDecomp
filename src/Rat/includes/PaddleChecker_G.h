#ifndef _PADDLECHECKER_G_H_
#define _PADDLECHECKER_G_H_
#include "Manipulator_Z.h"

class PaddleChecker_G : public Manipulator_Z {
public:
    PaddleChecker_G();
    virtual ~PaddleChecker_G();
    virtual void Init();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(94) PaddleChecker_G; }

private:
    U8 m_Unk_0x20[0x7c];
};
#endif // _PADDLECHECKER_G_H_
