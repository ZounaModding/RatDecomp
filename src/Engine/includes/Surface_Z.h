#ifndef _SURFACE_Z_H_
#define _SURFACE_Z_H_
#include "Points_Z.h"
#include "MaterialAnim_ZHdl.h"
#include "SystemObject_Z.h"
#include "SurfaceTree_Z.h"

class Material_Z;
class SeadIndex_Z;

#define FL_IS_SURFACE_UNK_0x200000 (FL_OBJECT_LAST << 1)
#define FL_IS_SURFACE_UNK_0x800000 (FL_OBJECT_LAST << 3)

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
    U16 m_OccluderZoneStartId;
    U16 m_EdgeIndices[4];
    Material_Z* m_Material;
    Sphere_Z m_BSphere;
    CullCone_Z m_CullCone;
    Box_Z m_BBox;
    U16 m_UnkIndices[4];
    U16 m_ColorIndices[4];
    U16 m_NormalIndices[4];
    U16 m_UVIndices[8];
    U16 m_ColCacheId;
    U16 m_UnkIndex_0xaa;
    MaterialAnim_ZHdl m_MaterialAnimHdl;

    Patch() {
        FIXDEBUGINLINING_Z();
    }
};

struct Edge {
    U16 P[2];
    U16 T[2];
};

struct EdgeCol {
    Sphere_Z m_Sphere;
    S32 m_Flag;
    U32 m_Index;
    S32 m_CacheId;
    U8 m_Pad_0x1c[4];
};

typedef DynArray_Z<Patch, 32> PatchDA;
typedef DynArray_Z<Edge, 32, FALSE> EdgeDA;
typedef DynArray_Z<EdgeCol, 32, FALSE> EdgeColDA;

#define MAX_RECURSE 16

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
    virtual Bool GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionCapsule(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, StaticArray_Z<ColSphereResult_Z, NUM_CAPSULE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionTriangles(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<TriangleFlag_Z, 8, FALSE, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);

    virtual U32 GetNbNormals() const {
        return m_Normals.GetSize();
    }

    void GetQuadPatchCtrlPoint(const Patch& i_Patch, QuadCtrlPoint_Z& o_QuadCtrlPoint);
    Vec4f* GetCollisionPtr(U16 i_PatchId);
    void FillColCache(Vec4f* o_Cache, const Patch& i_Patch);

    friend class ColSurfaceCache_Z;

protected:
    EdgeColDA m_EdgeColTab;
    EdgeColDA m_ClingEdgeColTab;
    PatchDA m_PatchTab;
    EdgeDA m_EdgeTab;
    Vec3fDA m_Normals;
    Vec3fDA m_Colors;
    Vec2fDA m_UVs;
    S16DA m_OccluderZoneIndices;
    SurfaceTreeEleDA m_STreeTab;
    SeadIndex_Z* m_Sead;
};

class SurfaceDatas_Z : public PointsDatas_Z {
public:
    virtual ~SurfaceDatas_Z() { }

    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(264) SurfaceDatas_Z;
    }
};

#endif
