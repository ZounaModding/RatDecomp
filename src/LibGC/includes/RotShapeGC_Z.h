#ifndef _ROTSHAPEGC_Z_H_
#define _ROTSHAPEGC_Z_H_
#include "RotShape_Z.h"
#include "DisplayList_Z.h"
#include "StreamList_Z.h"
#include <dolphin.h>

class RotShapeStreamList_Z : public StreamList_Z {
public:
    virtual void SetStream() {
        PPCSync();
        GXInvalidateVtxCache();
        GXSetArray(GX_VA_POS, m_Positions, sizeof(Vec3f_S16_Z));
        GXSetArray(GX_VA_CLR0, m_Colors, sizeof(ColorU8));
        GXSetArray(GX_VA_TEX0, m_TextureCoordinates, sizeof(ByteColor));
    }

    virtual void SetVtxDesc() {
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    }

    Vec3f_S16_Z* m_Positions;
    ColorU8* m_Colors;
    ByteColor* m_TextureCoordinates;
};

class RotShapeGC_Z : public RotShape_Z {
public:
    virtual ~RotShapeGC_Z() { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    static BaseObject_Z* NewObject() {
        return NewL_Z(50) RotShapeGC_Z;
    }

private:
    DynArray_Z<DisplayList_Z, 64> m_DisplayLists;
    U8 m_Pad_0xd8[8];
};

class RotShapeGCData_Z : public RotShapeData_Z {
public:
    virtual ~RotShapeGCData_Z() { }

    virtual void Load(void** i_Data);
    virtual void UpdatePointers(Object_Z* i_Obj, S32 i_Param = -1);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(77) RotShapeGCData_Z;
    }

private:
    RotShapeStreamList_Z m_StreamList;
    Vec3f_S16_ZDA m_Positions;
    ByteColorDA m_TextureCoordinates;
    S16DA m_Indices;
};

#endif // _ROTSHAPEGC_Z_H_
