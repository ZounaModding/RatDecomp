#ifndef _SPLINE_Z_H_
#define _SPLINE_Z_H_
#include "Object_Z.h"
#include "Node_ZHdl.h"

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
    Spline_Z();
    virtual ~Spline_Z();
    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    static BaseObject_Z* NewObject() { return NewL_Z(119) Spline_Z; }

    Float GetValueOnTrack(Float i_Ratio, Vec3f& i_Pos);
    Float GetValueOnTrack(const Node_ZHdl& i_NodeHdl, Float i_Ratio, Vec3f& i_Pos);
    Float GetValueOutTrack(Float i_Ratio, Vec3f& i_Pos);
    S32 GetSplineId(Float i_Ratio);

    static Float CalcSplineLen(Vec3f& i_PosA, Vec3f& i_PosB, Vec3f& i_PosC, Vec3f& i_PosD, Float i_Ratio);

private:
    PointsDA m_Points;
    SplineDA m_Splines;
    Vec4f m_UnkVec4f_0x90;
    Float m_Length;
};

#endif // _SPLINE_Z_H_
