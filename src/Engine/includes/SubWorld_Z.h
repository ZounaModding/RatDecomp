#ifndef _SUBWORLD_Z_H_
#define _SUBWORLD_Z_H_
#include "ResourceObject_Z.h"

class SubWorld_Z : public ResourceObject_Z {
public:
    static Float SubWorldDisplayScale;
    static S32 SubWorldDisplayID;
    static Bool SubWorldDisplaySelection;

    SubWorld_Z();

    virtual ~SubWorld_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(89) SubWorld_Z; }
};

#endif // _SUBWORLD_Z_H_
