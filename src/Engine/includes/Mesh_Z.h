#ifndef _MESH_Z_H_
#define _MESH_Z_H_
#include "Points_Z.h"
#include "Camera_Z.h"
#include "Material_ZHdl.h"
#include "MeshCollision_Z.h"
#include "MeshStrip_Z.h"

class FaceVolume_Z {
public:
    Vec4f* m_Vertex1;
    Vec4f* m_Vertex2;
    Vec4f* m_Vertex3;
    U8 m_Unk_0xC[20];
};

class EdgeVolume_Z {
public:
    FaceVolume_Z* m_Face1;
    FaceVolume_Z* m_Face2;
    Vec4f* m_Vertex1;
    Vec4f* m_Vertex2;
};

class MeshVolume_Z {
public:
    void Load(void** i_Data);
    void Clone(const MeshVolume_Z& i_MeshVolume);

    Vec4fDA m_Vertices;
    DynArray_Z<FaceVolume_Z, 64, FALSE, FALSE> m_FaceVolumes;
    DynArray_Z<EdgeVolume_Z, 64, FALSE, FALSE> m_EdgeVolumes;
    Vec4fDA m_UnkVertices;
};

class MeshData_Z : public PointsDatas_Z {
public:
    virtual ~MeshData_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual Bool IsBSphere(Node_Z* i_Node, Frustrum_Z& i_Frust, DrawInfo_Z& i_DrawInfo);

protected:
    MeshVolume_Z m_MeshVolume;
};

class Mesh_Z : public Points_Z {
public:
    Mesh_Z();
    virtual ~Mesh_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void EndLoadLinks();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& a1, ObjectDatas_Z* a2);
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionCapsule(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, StaticArray_Z<ColSphereResult_Z, NUM_CAPSULE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionBoxes(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<BoxFlag_Z, 8, FALSE, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionSpheres(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Sph, DynArray_Z<SphereFlag_Z, 8, FALSE, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);

    virtual U32 GetNbNormals() const {
        return m_Normals.GetSize();
    }

protected:
    Vec3fDA m_Normals;
    Vec2fDA m_TextureCoordinates;
    Strip_ZDA m_Strips;
    StripExt_ZDA m_StripExtensions;
    Material_ZHdlDA m_Materials;
    SphereCol_ZDA m_SphereCollisions;
    BoxCol_ZDA m_BoxCollisions;
    CylindreCol_ZDA m_CylindreCollisions;
    AABBCol_Z m_AABBCol;
    Vec3f_S16_ZDA m_OptimizedVertices;
    Float m_DrawingStartDistance;
    Float m_DrawingCutoffDistance;
    U32 m_MeshVolumeVertexCount;
    U32 m_UnkCount2;
    U32 m_UnkCount3;
    U32 m_UnkCount4;
};

#endif
