#ifndef _WANDERINGPATH_G_H_
#define _WANDERINGPATH_G_H_
#include "BaseObject_Z.h"

class WanderingPath_G : public BaseObject_Z {
public:
    virtual ~WanderingPath_G();
    virtual void Init();

    static BaseObject_Z* NewObject() { return NewL_Z(105) WanderingPath_G; }

    void Minimize();

private:
    U8 m_Unk_0xc[0xc];
};
#endif // _WANDERINGPATH_G_H_
