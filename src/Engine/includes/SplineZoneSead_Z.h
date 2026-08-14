#ifndef _SPLINEZONESEAD_Z_H_
#define _SPLINEZONESEAD_Z_H_
#include "Math_Z.h"
#include "DynArray_Z.h"

// Not really a voxel because it's 2D (Like SeadHandle_Z)
struct SplineZoneVoxel_Z {
    U16 m_ZoneNb;  // Number of zones in the voxel
    U16 m_ZoneIdx; // Index of the first zone in the voxel
};

typedef DynArray_Z<SplineZoneVoxel_Z, 32, FALSE, FALSE> SplineZoneVoxelDA;

class SplineZoneSead_Z {
public:
    SplineZoneSead_Z() {
        m_MaxZoneIndex = 0;
        m_SizeY = 0;
        m_SizeX = 0;
    }

    void Empty();
    void IsRef(const Vec2f& i_Pos, U16** o_ZoneIds);

private:
    Vec2f m_PMin;
    Vec2f m_PMax;
    Vec2f m_InvDiag;
    S32 m_MaxZoneIndex;
    S32 m_SizeX;
    S32 m_SizeY;
    SplineZoneVoxelDA m_GridDA;
    U16DA m_ZoneIdDA; // Array to get zone ids in a given grid cell
};

#endif
