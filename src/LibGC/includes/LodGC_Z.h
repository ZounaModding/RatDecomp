#ifndef _LODGC_Z_H_
#define _LODGC_Z_H_
#include "Lod_Z.h"

class LodGC_Z : public Lod_Z {
public:
    virtual ~LodGC_Z() { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    virtual void DrawReceiveShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data, S32 i_ObjID) { }

    static BaseObject_Z* NewObject() {
        return NewL_Z(18) LodGC_Z;
    }
};

class LodGCData_Z : public LodData_Z {
public:
    LodGCData_Z();

    virtual ~LodGCData_Z() { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() {
        return NewL_Z(43) LodGCData_Z;
    }
};

#endif // _LODGC_Z_H_
