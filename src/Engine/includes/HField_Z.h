#ifndef _HFIELD_Z_H_
#define _HFIELD_Z_H_
#include "Object_Z.h"
#include "Node_Z.h"
#include "LightData_Z.h"

struct HFieldLayer {
    // $SABE: There's some members we can figure out from the PC version,
    //        but since it's not used at all in GC I'll just leave it like this
    U8 m_Pad[232];

    void GetGrid(Float, Float, Float, Float, Rect_Z&, Float&, Float&);
};

// $SABE: We don't really know the granularity of this DA, doesn't matter
typedef DynArray_Z<HFieldLayer> HFieldLayerDA;

// $SABE: This class is not generated at all in the GC version,
//        but World_Z stores an HField_ZHdl so might as well define it
class HField_Z : public Object_Z {
public:
    HField_Z();
    virtual ~HField_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& a1, ObjectDatas_Z* a2);
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionTriangles(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<TriangleFlag_Z, 8, FALSE, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);

    void GetSize(Vec2f&, Vec2f&);
    Bool UpdateLocalCollisionSphereSA(StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& io_Result, S32 i_PrevResultNb);
    void UpdateLocalCollisionSphere(ColSphereResult_Z* io_Result, S32 i_ResultNb);

    inline Node_Z* GetNode() { return &m_Node; }

protected:
    Node_Z m_Node; // $SABE: This looks gross as fuck, why did they embed a Node_Z in an object class lmfao
    LightData_Z m_LightData;
    HFieldLayerDA m_Layers;
};

#endif // _HFIELD_Z_H_
