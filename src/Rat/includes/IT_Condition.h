#ifndef _IT_CONDITION_H_
#define _IT_CONDITION_H_
#include "Manipulator_Z.h"

class IT_Condition : public Manipulator_Z {
public:
    virtual ~IT_Condition();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(119) IT_Condition; }

    void Minimize();

private:
    U8 m_Unk_0x20[0xc];
};
#endif // _IT_CONDITION_H_
