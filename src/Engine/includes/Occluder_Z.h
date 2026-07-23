#ifndef _OCCLUDER_Z_H_
#define _OCCLUDER_Z_H_
#include "SplineZone_Z.h"

class Occluder_Z : public SplineZone_Z {
public:
    virtual ~Occluder_Z() { }

    virtual void Load(void** i_Data);

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) { }

    static BaseObject_Z* NewObject() { return NewL_Z(145) Occluder_Z; }
};

#endif // _OCCLUDER_Z_H_
