#ifndef _SPLINE_Z_H_
#define _SPLINE_Z_H_
#include "Object_Z.h"

#define SPLINE_NBSEGMENT 8

struct Segment {
    Vec3f m_Vtx[2];
    Float m_Length;
};

struct Spline {
    S16 m_PointIds[2];
    S16 m_TgtIds[2];
    U32 m_Flag;
    Float m_Length;
    Segment m_Segments[SPLINE_NBSEGMENT];
};

typedef DynArray_Z<Vec3f, 32, FALSE, FALSE, 32> PointsDA;
typedef DynArray_Z<Spline, 32, FALSE, TRUE> SplineDA;

class Spline_Z : public Object_Z {
public:
    virtual ~Spline_Z();
    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    Float GetValueOnTrack(Float i_Ratio, Vec3f& i_Pos);
    Float GetValueOutTrack(Float i_Ratio, Vec3f& i_Pos);

private:
    PointsDA m_Points;
    SplineDA m_Splines;
    Vec4f m_UnkVec4f_0x90;
    Float m_Length;
};

#endif // _SPLINE_Z_H_
