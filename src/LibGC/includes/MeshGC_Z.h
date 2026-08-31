#ifndef _MESHGC_Z_H_
#define _MESHGC_Z_H_
#include "Mesh_Z.h"
#include "DisplayList_Z.h"
#include "StreamList_Z.h"
#include <dolphin.h>

class MeshStreamList_Z : public StreamList_Z {
public:
    virtual void SetStream() {
        PPCSync();
        GXInvalidateVtxCache();
        GXSetArray(GX_VA_POS, m_PosData, sizeof(Vec3f_S16_Z));
        GXSetArray(GX_VA_NRM, m_NormalData, sizeof(S8) * 3);
        GXSetArray(GX_VA_TEX0, m_Tex0Data, sizeof(ByteColor));
        GXSetArray(GX_VA_TEX1, m_Tex1Data, sizeof(ByteColor));
    }

    virtual void SetVtxDesc() {
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxDesc(GX_VA_TEX1, GX_INDEX16);
    }

    Vec3f_S16_Z* m_PosData;
    S8* m_NormalData;
    ByteColor* m_UVData;
    S16* m_Tex0Data;
    S16* m_Tex1Data;
};

class StencilStreamList_Z : public StreamList_Z {
public:
    virtual void SetStream() {
        PPCSync();
        GXInvalidateVtxCache();
        GXSetArray(GX_VA_POS, m_PosData, sizeof(Vec3f));
    }

    virtual void SetVtxDesc() {
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    }

    Vec3f* m_PosData;
};

class MeshGC_Z : public Mesh_Z {
public:
    virtual ~MeshGC_Z() { }

    virtual void Load(void** i_Data);
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    virtual void DrawReceiveShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data, S32 i_ObjID) { }

    virtual void DrawCastingShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual Bool DrawWithMaterials(DrawInfo_Z& i_DrawInfo, StreamList_Z& i_StreamList, Material_ZHdlDA& i_Materials, U32 i_Flag, Bool i_IsFaded);

    static BaseObject_Z* NewObject() {
        return NewL_Z(97) MeshGC_Z;
    }

private:
    S8DA m_OptimizedNormals;
    S16DA m_OptimizedTextureCoordinates;
    S32DA m_MaterialIndices;
    DynArray_Z<DisplayList_Z, 64> m_DisplayLists;
    MeshStreamList_Z m_StreamList;
    U8 m_Pad_0x148[8];
};

class MeshDataGC_Z : public MeshData_Z {
public:
    virtual ~MeshDataGC_Z() { }

    virtual void AfterEndLoad();

    static BaseObject_Z* NewObject() {
        return NewL_Z(118) MeshDataGC_Z;
    }

private:
    StencilStreamList_Z m_StencilStreamLists[2];
    DisplayList_Z m_DisplayLists[2];
};

#endif // _MESHGC_Z_H_
