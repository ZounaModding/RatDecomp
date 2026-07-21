#ifndef _SURFACEGC_Z_H_
#define _SURFACEGC_Z_H_
#include "Surface_Z.h"
#include "StreamList_Z.h"
#include "DisplayList_Z.h"
#include "ObjectCache_Z.h"
#include "Renderer_Z.h"
#include <dolphin.h>

class SurfaceGC_Z : public Surface_Z {
    friend class SurfaceCacheGCEntry_Z;

public:
    virtual ~SurfaceGC_Z();
    virtual void Load(void** i_Data);
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    static BaseObject_Z* NewObject() {
        return NewL_Z(25) SurfaceGC_Z;
    }

private:
    U16DA m_SurfaceCacheEntryIDs;
    U8 m_Pad_0xf8[8];
};

class SurfaceGCVertex_Z {
public:
    Vec4f m_Position;
    GXColor m_Color;
    S8 m_NormalX;
    S8 m_NormalY;
    S8 m_NormalZ;
    U8 m_NormalPadding;
    S16 m_UVs[2];
    S16 m_LightmapUVs[2];
};

typedef DynArray_Z<SurfaceGCVertex_Z, 8, FALSE, FALSE, 32> SurfaceGCVertex_ZDA;

class SurfaceCacheGCEntry_Z : public StreamList_Z {
public:
    static DisplayList_Z DisplayList[2];

    virtual void SetStream() {
        PPCSync();
        GXInvalidateVtxCache();
        GXSetArray(GX_VA_POS, &m_VertexCache.GetArrayPtr()->m_Position, sizeof(SurfaceGCVertex_Z));
        GXSetArray(GX_VA_NRM, &m_VertexCache.GetArrayPtr()->m_NormalX, sizeof(SurfaceGCVertex_Z));
        GXSetArray(GX_VA_CLR0, &m_VertexCache.GetArrayPtr()->m_Color, sizeof(SurfaceGCVertex_Z));
        GXSetArray(GX_VA_TEX0, m_VertexCache.GetArrayPtr()->m_UVs, sizeof(SurfaceGCVertex_Z));
        GXSetArray(GX_VA_TEX1, m_VertexCache.GetArrayPtr()->m_LightmapUVs, sizeof(SurfaceGCVertex_Z));
    }

    virtual void SetVtxDesc() {
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX1, GX_INDEX8);
    }

    void FreeEntry(const CacheStateFrame_Z& i_CacheState);

private:
    S32 m_Lod;
    DynArray_Z<SurfaceGCVertex_Z, 8, FALSE, FALSE, 32> m_VertexCache;
};

typedef CacheEntryFrame_Z<SurfaceCacheGCEntry_Z, TRUE, 32> SurfaceCacheGCEntryFrame_Z;
typedef BnkCacheEntryFrame_Z<SurfaceCacheGCEntry_Z, 128, TRUE, TRUE, 32> SurfaceCacheBnk_Z;
typedef BnkCacheEntryFrame_Z<SurfaceCacheGCEntry_Z, 128, TRUE, TRUE, 32>::BnkCacheEntryFrameEle_Z SurfaceCacheBnkEle_Z;

class SurfaceCache_Z {
    friend class GCRenderer_Z;

public:
    SurfaceCache_Z() {
        Reset();
    }

    ~SurfaceCache_Z();

    void Update() {
        m_BnkCache.Update();
    }

    void GetState(CacheState_Z& o_State) {
        o_State.Reset();
        m_BnkCache.GetState(o_State);
    }

    void Reset();
    void BuildDisplayList(DisplayList_Z& o_DisplayList, S32 i_Size);

private:
    SurfaceCacheBnk_Z m_BnkCache;
};

#endif // _SURFACEGC_Z_H_
