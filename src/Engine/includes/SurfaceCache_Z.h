#ifndef _SURFACECACHE_Z_H_
#define _SURFACECACHE_Z_H_
#include "ObjectCache_Z.h"
#include "Math_Z.h"
#include "Surface_Z.h"

#define SURFACECACHED_LOD_MAX 3
#define SURFACECACHED_COLLISION_LOD_MAX 3
#define SURFACECACHED_COLLISION_CACHE_ENTRY_COUNT 50

class BaseColSurfaceCache_Z {
public:
    BaseColSurfaceCache_Z() {
        m_CollisionLod = SURFACECACHED_COLLISION_LOD_MAX;
        S32 l_Lod = m_CollisionLod;
        S32 l_LodP1 = l_Lod + 1;
        S32 l_Size = l_LodP1 * l_LodP1;
        l_Size += (l_Lod * l_Lod * 2);
        l_Size += l_Lod * l_Lod;
        l_Size += (l_LodP1 * l_LodP1 * 3);
        l_Size += l_LodP1 * 2;
        m_Cache.SetSize(SURFACECACHED_COLLISION_CACHE_ENTRY_COUNT, l_Size);
    }

    inline S32 GetCollisionLod() const {
        return m_CollisionLod;
    }

    inline Vec4f* GetCollisionCache(U16 i_Id) {
        return m_Cache.GetData(i_Id);
    }

protected:
    S32 m_CollisionLod;
    CacheEntryLRU_Z<Vec4f> m_Cache;
};

class ColSurfaceCache_Z : public BaseColSurfaceCache_Z {
public:
    U16 GetFreeEntry(Surface_Z* i_Surface, U16 i_PatchId);

    inline void FreeEntry(U16 i_Id) {
        m_Cache.m_LRU.m_LRUList[i_Id + 1].m_Val.m_OwnerId = U16_MAX;
    }
};

#endif
