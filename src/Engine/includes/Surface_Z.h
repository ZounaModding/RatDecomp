#ifndef _SURFACE_Z_H_
#define _SURFACE_Z_H_
#include "Points_Z.h"

enum PatchFlag {
    EDGE1 = 2,
    EDGE2 = 4,
    EDGE3 = 8,
    EDGE4 = 16,
};

struct QuadCtrlPoint_Z {
    Vec4f m_ControlPoints[4][4];
};

struct Patch {
    U16 m_Flag;
    U16 m_EdgeIndices[4];
};

struct Edge {
    U16 P[2];
    U16 T[2];
};

typedef DynArray_Z<Edge, 32, FALSE, TRUE> EdgeDA;
typedef DynArray_Z<Patch, 32, TRUE, TRUE> PatchDA;

class Surface_Z : public Points_Z {
public:
    Surface_Z();
    virtual ~Surface_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetClingLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ClingLineResult_Z& o_Result);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual U32 GetNbNormals() const;

    void GetQuadPatchCtrlPoint(const Patch& i_Patch, QuadCtrlPoint_Z& o_QuadCtrlPoint);

protected:
    U8 m_Pad_0xa0[16];
    PatchDA m_PatchTab;
    EdgeDA m_EdgeTab;
    U8 m_Pad_0xc0[48];
};

class SurfaceDatas_Z : public PointsDatas_Z {
public:
    virtual ~SurfaceDatas_Z();
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(264) SurfaceDatas_Z;
    }
};

#endif
