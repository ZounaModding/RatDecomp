#include "SurfaceGC_Z.h"

void SurfaceGC_Z::Load(void** i_Data) {
    Surface_Z::Load(i_Data);
    S32 l_NbPatch = m_PatchTab.GetSize();
    m_SurfaceCacheEntryIDs.SetSize(l_NbPatch);
    S32 i = 0;
    while (i < l_NbPatch) {
        m_SurfaceCacheEntryIDs[i++] = -1;
    }
}
