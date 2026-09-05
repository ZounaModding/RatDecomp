#ifndef _SEADINDEX_Z_H_
#define _SEADINDEX_Z_H_
#include "Types_Z.h"
#include "BitArray_Z.h"
#include "DynArray_Z.h"
#include "Math_Z.h"
#include "SystemObject_Z.h"

struct SeadVoxel_Z {
    S16 m_ElementEntry;
    S16 m_NumElement;
};

typedef DynArray_Z<SeadVoxel_Z, 32> SeadVoxel_ZDA;
typedef DynArray_Z<S16, 1, FALSE, FALSE> ElementList_ZDA;

class SeadIndex_Z {
public:
    SeadIndex_Z();

    Bool RayCast(const Segment_Z& i_Segment);
    Bool RayCasted(const Segment_Z& i_Segment, Segment_Z& o_Segment);

    inline S32 GetHitIndex() const {
        return m_HitIndex;
    }

    inline S16* GetHitElementList() {
        return m_HitElementList.GetArrayPtr();
    }

    void VoxelTrace(const Vec4i& i_Voxel, const Vec4f& i_Delta, const Vec4i& i_Step, const Vec4f& i_Ray);
    Bool SetZone(const Sphere_Z& i_Sphere);
    void Load(void** i_Data);

    inline void AddHitVoxel(int i_VoxelId, S16*& io_PatchIndices) { }

    SeadVoxel_ZDA m_Voxels;
    ElementList_ZDA m_ElementList;
    Vec4f m_UnkVec4f_0x10;
    Vec4f m_UnkVec4f_0x20;
    Vec4f m_UnkVec4f_0x30;
    Mat4x4 m_Axis;
    Vec4f m_HalfSize;
    Vec4f m_Step;
    Vec4f m_Center;
    Vec4f m_UnkVec4f_0xb0;
    Vec4f m_FSize;
    U32 m_NumElement;
    Vec3i m_Size;
    S32 m_HitIndex;
    BitArray_Z m_FlagsList;
    ElementList_ZDA m_HitElementList;
};

#endif // _SEADINDEX_Z_H_
