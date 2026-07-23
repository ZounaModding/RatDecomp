#ifndef _GENWORLD_Z_H_
#define _GENWORLD_Z_H_

#include "Object_Z.h"

class GenWorld_Z : public Object_Z {
public:
    virtual ~GenWorld_Z() { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) { }

    static BaseObject_Z* NewObject() { return NewL_Z(16) GenWorld_Z; }
};

#endif // _GENWORLD_Z_H_
