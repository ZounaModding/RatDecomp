#ifndef _SKEL_Z_H_
#define _SKEL_Z_H_
#include "Object_Z.h"
#include "Camera_Z.h"
#include "BoneNode_Z.h"
#include "Material_ZHdl.h"
#include "MeshMorph_Z.h"
#include "Points_ZHdl.h"

struct SphereColBone_Z {
    Sphere_Z m_Sphere;
    U32 m_UnkU32_0x10;
    Name_Z m_Name;
    U8 m_Unk_0x18[8];
    BoneNode_Z* m_BoneNode;
    U8 m_Unk_0x24[12];

    SphereColBone_Z() {
        m_UnkU32_0x10 = 0;
        m_BoneNode = NULL;
    }

    SphereColBone_Z(const SphereColBone_Z& i_Sphere)
        : m_Sphere(i_Sphere.m_Sphere)
        , m_UnkU32_0x10(i_Sphere.m_UnkU32_0x10)
        , m_Name(i_Sphere.m_Name)
        , m_BoneNode(i_Sphere.m_BoneNode) { }
};

struct BoxColBone_Z {
    Box_Z m_Box;
    U32 m_UnkU32_0x40;
    Name_Z m_Name;
    U8 m_Unk_0x48[8];
    BoneNode_Z* m_BoneNode;
    U8 m_Unk_0x54[0xc];

    BoxColBone_Z() {
        m_UnkU32_0x40 = 0;
        m_BoneNode = NULL;
    }

    BoxColBone_Z(const BoxColBone_Z& i_Box)
        : m_Box(i_Box.m_Box)
        , m_UnkU32_0x40(i_Box.m_UnkU32_0x40)
        , m_Name(i_Box.m_Name)
        , m_BoneNode(i_Box.m_BoneNode) { }
};

typedef DynArray_Z<SphereColBone_Z, 8> SphereColBone_ZDA;
typedef DynArray_Z<BoxColBone_Z, 8> BoxColBone_ZDA;
typedef DynArray_Z<S16, 32, FALSE, FALSE> S16DA;
typedef DynArray_Z<S16DA, 32> S16DADA;
typedef DynArray_Z<MorphTarget_Z, 64> MorphTarget_ZDA;

class Skel_Z : public ObjectDatas_Z {
public:
    Skel_Z();
    virtual ~Skel_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();
    virtual void Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl);
    virtual Bool IsBSphere(Node_Z* i_Node, Frustrum_Z& i_Frust, DrawInfo_Z& i_DrawInfo);
    virtual void GetCollisionLines(Node_Z* i_Node, ObjectDatas_Z* i_Data, const Segment_Z& i_Seg, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag);

    void Reset();
    void ResetBoneNodes();
    BoneNode_Z* GetOrgBoneNode(S32 i_Index);
    MorphTarget_Z* GetMorphTargetObject(S16 i_Index);
    void ResetMorphTarget();
    void Update();
    void Update(BoneNode_Z* i_BoneNode, Bool i_UpdateSiblings);
    SphereColBone_Z* ItSphere(S32& io_Index, const Name_Z& i_Name) const;
    Bool GetLocalMatrix(S32 i_Index, Mat4x4& o_Matrix);
    Bool GetLocalRot(S32 i_Index, Quat& o_Rotation);
    BoneNode_Z* FindNodeByName(const Name_Z& i_Name);
    S32 GetNodeIndex(const BoneNode_Z* i_BoneNode);
    Bool InsertNode(const Name_Z& i_NodeName, const Name_Z& i_ParentName);

    BoneNode_Z* GetBone(S32 i_Index) const {
        Bool l_IsValid = FALSE;
        if (i_Index >= 0 && i_Index < m_Bones.GetSize()) {
            l_IsValid = TRUE;
        }
        if (l_IsValid) {
            return m_Bones[i_Index];
        }
        return NULL;
    }

    void DefaultObjectsVisibility();

    S32 HideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);
    S32 UnhideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);

protected:
    static MorphTarget_Z DfltMorphTarget;

    Sphere_Z m_BSphere;
    BoneNodePtr_ZDA m_Bones;
    BoneNode_Z* m_SkelRoot;
    Material_ZHdlDA m_MaterialHdls;
    S32 m_SkelId;
    SphereColBone_ZDA m_HitSpheres;
    SphereColBone_ZDA m_AllSpheres;
    BoxColBone_ZDA m_AllBoxes;
    PointsDatas_ZHdlDA m_ObjectDatasHdls;
    S16DADA m_ObjectDatasBoneNodeIds;
    MorphTarget_ZDA m_MorphTargets;
    S16DA m_MorphTargetIds;
};

#endif // _SKEL_Z_H_
