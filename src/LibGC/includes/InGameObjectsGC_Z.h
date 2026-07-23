#ifndef _INGAMEOBJECTSGC_Z_H_
#define _INGAMEOBJECTSGC_Z_H_
#include "InGameObjects_Z.h"

class InGameObjectsGC_Z : public InGameObjects_Z {
public:
    virtual ~InGameObjectsGC_Z() { }

    virtual void Draw(const Viewport_Z* i_Viewport);

    static BaseObject_Z* NewObject() {
        return NewL_Z(13) InGameObjectsGC_Z;
    }
};

#endif // _INGAMEOBJECTSGC_Z_H_
