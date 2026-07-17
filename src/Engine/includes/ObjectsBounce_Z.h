#ifndef _OBJECTSBOUNCE_Z_H_
#define _OBJECTSBOUNCE_Z_H_
#include "ObjectsMove_Z.h"

class ObjectsBounce_Z : public ObjectsMove_Z {
public:
    virtual void Init();

    virtual ~ObjectsBounce_Z() { }

    virtual void EndLiveTimeObject(S32 i_Index);
    virtual void UpdateCollision(const ColLineResult_Z& i_Result, ObjectMoveSphereColl_Z& io_SphereColl, Float i_DeltaTime, S32 i_Index);

    static BaseObject_Z* NewObject() { return NewL_Z(16) ObjectsBounce_Z; }
};

#endif // _OBJECTSBOUNCE_Z_H_
