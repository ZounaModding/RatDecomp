#ifndef _VEHICULEMOVE_Z_H_
#define _VEHICULEMOVE_Z_H_
#include "LodMove_Z.h"

class VehiculeMove_Z : public LodMove_Z {
public:
    virtual ~VehiculeMove_Z();
    virtual void Init();
    virtual void Reset();
    virtual void Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);

    static BaseObject_Z* NewObject() { return NewL_Z(17) VehiculeMove_Z; }
};

#endif // _VEHICULEMOVE_Z_H_
