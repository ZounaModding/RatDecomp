#ifndef _FLAREGC_Z_H_
#define _FLAREGC_Z_H_
#include "Flare_Z.h"
#include "DisplayList_Z.h"
#include "StreamList_Z.h"

#define FL_FLAREGC_REBUILD_DISPLAYLIST (FL_OBJECTDATAS_LAST << 4) // 0x100000 - Rebuild display list for this flare

class VizQueryStreamList_Z : public StreamList_Z {
public:
    virtual void SetStream();
    virtual void SetVtxDesc();
};

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
    static VizQueryStreamList_Z VizQueryStreamList;

    virtual ~FlareDataGC_Z() { }

    virtual void UpdatePointers(Object_Z* i_Obj, S32 i_Param = -1);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(59) FlareDataGC_Z;
    }

private:
    DynArray_Z<DisplayList_Z, 64, TRUE, TRUE, 4> m_DisplayLists;
    DisplayList_Z m_DisplayList;
};

#endif // _FLAREGC_Z_H_
