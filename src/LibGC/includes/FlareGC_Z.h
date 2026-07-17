#ifndef _FLAREGC_Z_H_
#define _FLAREGC_Z_H_
#include "Flare_Z.h"

#define FL_FLAREGC_REBUILD_DISPLAYLIST (FL_OBJECTDATAS_LAST << 4) // 0x100000 - Rebuild display list for this flare

class FlareGC_Z : public Flare_Z {
public:
    virtual ~FlareGC_Z();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    static BaseObject_Z* NewObject() {
        return NewL_Z(42) FlareGC_Z;
    }
};

class FlareDataGC_Z : public ObjectDatas_Z {
public:
    virtual ~FlareDataGC_Z();
    virtual void UpdatePointers(Object_Z* i_Obj, S32 i_Param = -1);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(59) FlareDataGC_Z;
    }

private:
    U8 m_Pad_0x28[24];
};

#endif // _FLAREGC_Z_H_
