#include "SeadIndex_Z.h"

SeadIndex_Z::SeadIndex_Z() {
    m_Size = VEC3I_NULL;
    // $SABE: This is probably really wrong?
    *(Vec4f*)&m_Axis.m[3][0] = VEC3F_NULL;
    m_HitIndex = 0;
}

// TODO: Finish matching
Bool SeadIndex_Z::RayCast(const Segment_Z& i_Segment) {
    Segment_Z l_Local;
    Vec4f l_VoxelLow;
    Vec4f l_Ray;
    Vec4i l_VoxelStart;
    Vec4i l_VoxelEnd;

    if (!RayCasted(i_Segment, l_Local)) {
        return FALSE;
    }

    Vec4f l_Start;
    l_Start.x = (l_Local.Origin.x + m_HalfSize.x) * m_FSize.x;
    l_Start.y = (l_Local.Origin.y + m_HalfSize.y) * m_FSize.y;
    l_Start.z = (l_Local.Origin.z + m_HalfSize.z) * m_FSize.z;
    l_Start.w = 1.0f + m_HalfSize.w;

    l_Ray.x = l_Local.Direction.x * (m_FSize.x * l_Local.Length);
    l_Ray.y = l_Local.Direction.y * (m_FSize.y * l_Local.Length);
    l_Ray.z = l_Local.Direction.z * (m_FSize.z * l_Local.Length);

    Vec4f l_End;
    l_End.x = l_Start.x + l_Ray.x;
    l_End.y = l_Start.y + l_Ray.y;
    l_End.z = l_Start.z + l_Ray.z;
    l_End.w = l_Start.w + l_Ray.w;

    Vec4_Max(l_Start, l_Start, VEC4F_NULL);
    Vec4_Max(l_End, l_End, VEC4F_NULL);
    Vec4_Min(l_Start, l_Start, m_Step);
    Vec4_Min(l_End, l_End, m_Step);

    l_VoxelStart.x = (S32)l_Start.x;
    l_VoxelStart.y = (S32)l_Start.y;
    l_VoxelStart.z = (S32)l_Start.z;

    l_VoxelLow.x = l_Start.x - (Float)l_VoxelStart.x;
    l_VoxelLow.y = l_Start.y - (Float)l_VoxelStart.y;
    l_VoxelLow.z = l_Start.z - (Float)l_VoxelStart.z;

    l_VoxelEnd.x = (S32)l_End.x;
    l_VoxelEnd.y = (S32)l_End.y;
    l_VoxelEnd.z = (S32)l_End.z;

    l_VoxelEnd.x -= l_VoxelStart.x;
    l_VoxelEnd.y -= l_VoxelStart.y;
    l_VoxelEnd.z -= l_VoxelStart.z;

    m_HitIndex = 0;

    VoxelTrace(l_VoxelStart, l_VoxelLow, l_VoxelEnd, l_Ray);

    if (m_HitIndex) {
        m_FlagsList.ClearAllBits();
    }

    return TRUE;
}

void SeadIndex_Z::VoxelTrace(const Vec4i& i_Voxel, const Vec4f& i_Delta, const Vec4i& i_Step, const Vec4f& i_Ray) {
}

Bool SeadIndex_Z::SetZone(const Sphere_Z& i_Sphere) {
    return FALSE;
}

void SeadIndex_Z::Load(void** i_Data) {
    S32 l_Nb;

    LOADDYNARRAY_Z(m_Voxels);
    LOADDYNARRAY_Z(m_ElementList);
    // $SABE: Might be worth making a macro for this
    l_Nb = (U8*)&m_Size - (U8*)&m_UnkVec4f_0x10 + sizeof(m_Size);
    LOADBYTES_Z(&m_UnkVec4f_0x10, l_Nb);

    LOAD_Z(l_Nb);
    m_FlagsList.SetSize(l_Nb);
    m_FlagsList.ClearAllBits();
    m_HitElementList.SetSize(l_Nb);
}

// TODO: Finish matching
Bool SeadIndex_Z::RayCasted(const Segment_Z& i_Segment, Segment_Z& o_Segment) {
    Vec4f l_Delta = *(const Vec4f*)&i_Segment.Origin - m_Center;

    Vec4_Rotate(*(Vec4f*)&o_Segment.Origin, m_Axis, l_Delta);
    Vec4_Rotate(*(Vec4f*)&o_Segment.Direction, m_Axis, *(const Vec4f*)&i_Segment.Direction);

    if (o_Segment.Direction.x >= 0.0f) {
        if (o_Segment.Origin.x >= m_HalfSize.x) {
            return FALSE;
        }
    }
    else if (o_Segment.Origin.x <= -m_HalfSize.x) {
        return FALSE;
    }

    if (o_Segment.Direction.y >= 0.0f) {
        if (o_Segment.Origin.y >= m_HalfSize.y) {
            return FALSE;
        }
    }
    else if (o_Segment.Origin.y <= -m_HalfSize.y) {
        return FALSE;
    }

    if (o_Segment.Direction.z >= 0.0f) {
        if (o_Segment.Origin.z >= m_HalfSize.z) {
            return FALSE;
        }
    }
    else if (o_Segment.Origin.z <= -m_HalfSize.z) {
        return FALSE;
    }

    Float l_Near;
    Float l_Far;

    if (o_Segment.Direction.x == 0.0f) {
        if (o_Segment.Origin.x <= -m_HalfSize.x || o_Segment.Origin.x >= m_HalfSize.x) {
            return FALSE;
        }
        l_Far = 1.e10f;
        l_Near = -l_Far;
    }
    else {
        Float l_Inv = 1.0f / o_Segment.Direction.x;
        Float l_T0 = l_Inv * (m_HalfSize.x - o_Segment.Origin.x);
        Float l_T1 = l_Inv * (-m_HalfSize.x - o_Segment.Origin.x);

        if (l_T1 <= l_T0) {
            l_Near = l_T1;
            l_Far = l_T0;
        }
        else {
            l_Near = l_T0;
            l_Far = l_T1;
        }

        if (l_Near > l_Far) {
            return FALSE;
        }
        if (l_Far < 0.0f) {
            return FALSE;
        }
        if (l_Near > i_Segment.Length) {
            return FALSE;
        }
    }

    if (o_Segment.Direction.y == 0.0f) {
        if (o_Segment.Origin.y <= -m_HalfSize.y || o_Segment.Origin.y >= m_HalfSize.y) {
            return FALSE;
        }
    }
    else {
        Float l_Inv = 1.0f / o_Segment.Direction.y;
        Float l_T0 = l_Inv * (m_HalfSize.y - o_Segment.Origin.y);
        Float l_T1 = l_Inv * (-m_HalfSize.y - o_Segment.Origin.y);

        if (l_T1 <= l_T0) {
            l_Near = Max(l_T1, l_Near);
            l_Far = Min(l_T0, l_Far);
        }
        else {
            l_Near = Max(l_T0, l_Near);
            l_Far = Min(l_T1, l_Far);
        }

        if (l_Near > l_Far) {
            return FALSE;
        }
        if (l_Far < 0.0f) {
            return FALSE;
        }
        if (l_Near > i_Segment.Length) {
            return FALSE;
        }
    }

    if (o_Segment.Direction.z == 0.0f) {
        if (o_Segment.Origin.z <= -m_HalfSize.z || o_Segment.Origin.z >= m_HalfSize.z) {
            return FALSE;
        }
    }
    else {
        Float l_Inv = 1.0f / o_Segment.Direction.z;
        Float l_T0 = l_Inv * (m_HalfSize.z - o_Segment.Origin.z);
        Float l_T1 = l_Inv * (-m_HalfSize.z - o_Segment.Origin.z);

        if (l_T1 <= l_T0) {
            l_Near = Max(l_T1, l_Near);
            l_Far = Min(l_T0, l_Far);
        }
        else {
            l_Near = Max(l_T0, l_Near);
            l_Far = Min(l_T1, l_Far);
        }

        if (l_Near > l_Far) {
            return FALSE;
        }
        if (l_Far < 0.0f) {
            return FALSE;
        }
        if (l_Near > i_Segment.Length) {
            return FALSE;
        }
    }

    Float l_Start = 0.0f;

    if (0.0f <= l_Near) {
        o_Segment.Origin.x = o_Segment.Origin.x + o_Segment.Direction.x * l_Near;
        o_Segment.Origin.y = o_Segment.Origin.y + o_Segment.Direction.y * l_Near;
        o_Segment.Origin.z = o_Segment.Origin.z + o_Segment.Direction.z * l_Near;
        o_Segment.Length = 1.0f;
        l_Start = l_Near;
    }

    if (i_Segment.Length < l_Far) {
        l_Far = i_Segment.Length;
    }

    o_Segment.Length = l_Far - l_Start;

    return TRUE;
}
