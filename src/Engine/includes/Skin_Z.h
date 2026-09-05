#ifndef _SKIN_Z_H_
#define _SKIN_Z_H_
#include "Object_Z.h"
#include "Points_ZHdl.h"

struct BlendVtx_Z {
    U32 m_VertexId;
    Float m_Percent;
};

typedef DynArray_Z<BlendVtx_Z, 32, FALSE> BlendVtx_ZDA;

struct ObjectBlend {
    S16 m_ObjectId;
    BlendVtx_ZDA m_Vertices;
    BlendVtx_ZDA m_Normals;
};

typedef DynArray_Z<ObjectBlend, 32> ObjectBlendDA;

struct Bone_Z {
    inline ObjectBlendDA& GetObjectBlends() {
        return m_ObjectBlends;
    }

    U32 m_NameId;
    ObjectBlendDA m_ObjectBlends;
};

typedef DynArray_Z<Bone_Z, 32> Bone_ZDA;

struct RotShapeObject {
    U32 m_Data[2];
};

typedef DynArray_Z<RotShapeObject, 4> RotShapeObjectDA;

class Skin_Z : public Object_Z {
public:
    Skin_Z();
    virtual ~Skin_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual void Clean();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual Bool GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Sphere_Z& i_Seg, StaticArray_Z<ColSphereResult_Z, NUM_SPHERE_HIT_MAX, FALSE>& o_Result, U64 i_Flag, U64 i_NoFlag);
    virtual Bool GetCollisionMovingSphere(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Capsule_Z& i_Cap, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);

    virtual void UpdateBSphere(const ObjectDatas_ZHdl& i_DataHdl) { }

protected:
    Bone_ZDA m_Bones;
    Points_ZHdlDA m_ObjectHdls;
    RotShapeObjectDA m_RotshapeObjects;
};

#endif // _SKIN_Z_H_
