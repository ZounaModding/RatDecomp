#include "SurfaceGC_Z.h"

DisplayList_Z SurfaceCacheGCEntry_Z::DisplayList[2];

void SurfaceCache_Z::Reset() {
    SurfaceCacheGCEntry_Z::DisplayList[0].Reset();
    SurfaceCacheGCEntry_Z::DisplayList[1].Reset();
    BuildDisplayList(SurfaceCacheGCEntry_Z::DisplayList[0], 3);
    BuildDisplayList(SurfaceCacheGCEntry_Z::DisplayList[1], 8);
}

void SurfaceCache_Z::BuildDisplayList(DisplayList_Z& o_DisplayList, S32 i_Size) {
    S32 y;
    S32 l_VertexCount = i_Size + i_Size * i_Size;
    l_VertexCount += i_Size;
    l_VertexCount = (l_VertexCount - 1) * 2;
    o_DisplayList.Alloc((l_VertexCount * 5 + 0x42) & ~31);
    o_DisplayList.Begin();
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, l_VertexCount);

    S32 l_VertexIndex = 0;
    for (y = 0; y < i_Size; y++) {
        for (S32 x = 0; x < i_Size; x++) {
            GXPosition1x8(l_VertexIndex + i_Size + 1);
            GXNormal1x8(l_VertexIndex + i_Size + 1);
            GXColor1x8(l_VertexIndex + i_Size + 1);
            GXTexCoord1x8(l_VertexIndex + i_Size + 1);
            GXTexCoord1x8(l_VertexIndex + i_Size + 1);
            GXPosition1x8(l_VertexIndex);
            GXNormal1x8(l_VertexIndex);
            GXColor1x8(l_VertexIndex);
            GXTexCoord1x8(l_VertexIndex);
            GXTexCoord1x8(l_VertexIndex);
            l_VertexIndex++;
        }

        GXPosition1x8(l_VertexIndex + i_Size + 1);
        GXNormal1x8(l_VertexIndex + i_Size + 1);
        GXColor1x8(l_VertexIndex + i_Size + 1);
        GXTexCoord1x8(l_VertexIndex + i_Size + 1);
        GXTexCoord1x8(l_VertexIndex + i_Size + 1);
        GXPosition1x8(l_VertexIndex);
        GXNormal1x8(l_VertexIndex);
        GXColor1x8(l_VertexIndex);
        GXTexCoord1x8(l_VertexIndex);
        GXTexCoord1x8(l_VertexIndex);

        if (y < i_Size - 1) {
            GXPosition1x8(l_VertexIndex);
            GXNormal1x8(l_VertexIndex);
            GXColor1x8(l_VertexIndex);
            GXTexCoord1x8(l_VertexIndex);
            GXTexCoord1x8(l_VertexIndex);
            l_VertexIndex++;
            GXPosition1x8(l_VertexIndex + i_Size + 1);
            GXNormal1x8(l_VertexIndex + i_Size + 1);
            GXColor1x8(l_VertexIndex + i_Size + 1);
            GXTexCoord1x8(l_VertexIndex + i_Size + 1);
            GXTexCoord1x8(l_VertexIndex + i_Size + 1);
        }
        else {
            l_VertexIndex++;
        }
    }

    o_DisplayList.End();
}

void SurfaceCacheGCEntry_Z::FreeEntry(const CacheStateFrame_Z& i_CacheState) {
    ((SurfaceGC_Z*)i_CacheState.m_OwnerPtr)->m_SurfaceCacheEntryIDs[i_CacheState.m_Key] = -1;
}
