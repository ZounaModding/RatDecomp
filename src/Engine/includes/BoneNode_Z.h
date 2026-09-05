#ifndef _BONENODE_Z_H_
#define _BONENODE_Z_H_
#include "Math_Z.h"
#include "UserDefine_ZHdl.h"
#include "BoneNodeFlag_Z.h"
#include "DynPtrArray_Z.h"
#include "MatrixCache_Z.h"

class BoneNode_Z;
typedef DynPtrArray_Z<BoneNode_Z*, 4> BoneNodePtr_ZDA;

struct BoneTRS_Z {
    Quat m_Rotation;
    Vec3f m_Scale;
    U32 m_Flag;
    Vec3f m_Translation;
    BoneNode_Z* m_HeadSon;

    BoneTRS_Z& operator=(const BoneTRS_Z& i_Trs) {
        m_Rotation = i_Trs.m_Rotation;
        m_Scale = i_Trs.m_Scale;
        m_Flag = i_Trs.m_Flag;
        m_Translation = i_Trs.m_Translation;
        m_HeadSon = i_Trs.m_HeadSon;
        return *this;
    }
};

class Skel_Z;

class BoneNode_Z {
    friend class Skel_Z;

public:
    BoneNode_Z();
    ~BoneNode_Z();

    void UpdateTM(BoneNode_Z* i_Parent);

    inline void SetTranslation(const Vec3f& i_Translation) { m_Trs.m_Translation = i_Translation; }

    //inline void SetTranslation(const Vec3f& i_Translation) { m_Translation = i_Translation; }

    inline Vec3f& GetTranslation() { return m_Trs.m_Translation; }

    //inline Vec3f& GetTranslation() { return m_Translation; }

    inline void SetScale(const Vec3f& i_Scale) { m_Trs.m_Scale = i_Scale; }

    //inline void SetScale(const Vec3f& i_Scale) { m_Scale = i_Scale; }

    inline Vec3f& GetScale() { return m_Trs.m_Scale; }

    //inline Vec3f& GetScale() { return m_Scale; }

    inline void SetRotation(const Quat& i_Rotation) { m_Trs.m_Rotation = i_Rotation; }

    //inline void SetRotation(const Quat& i_Rotation) { m_Rotation = i_Rotation; }

    inline Quat& GetRotation() { return m_Trs.m_Rotation; }

    //inline Quat& GetRotation() { return m_Rotation; }

    inline void SetRotInWorld(const Quat& i_Rotation) {
        m_RotInWorld = i_Rotation;
        m_RotInWorld.GetMatrix(m_RotInWorldMatrix);
        EnableBoneFlag(FL_BONENODE_INVALIDROT);
    }

    inline Quat& GetRotInWorld() { return m_RotInWorld; }

    inline Quat GetInverseRotInWorld() {
        return Quat(m_RotInWorld.w, -m_RotInWorld.v.x, -m_RotInWorld.v.y, -m_RotInWorld.v.z);
    }

    inline void DisableBoneFlag(U32 i_Flag) { m_Trs.m_Flag &= ~i_Flag; }

    void DisableBoneFlag(U32 i_Flag, Bool i_Recursive);

    inline void EnableBoneFlag(U32 i_Flag) { m_Trs.m_Flag |= i_Flag; }

    inline void SetWorldMatrixId(U16 i_Id) { m_RotInWorldMatrix.m.m03.dummy.u16[0] = i_Id; }

    inline U16 GetWorldMatrixId() const { return m_RotInWorldMatrix.m.m03.dummy.u16[0]; }

    inline void SetWorldMatrix(const Mat4x4& i_World) { *GetWorldMatrixPtr() = i_World; }

    inline Mat4x4& GetWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline Mat4x4* GetWorldMatrixPtr() const { return (Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline void SetWorldMatrixPtr(Mat4x4* i_WorldMatrix) { m_RotInWorldMatrix.m.m13.dummy.i32 = (S32)i_WorldMatrix; }

    inline void SetInverseWorldMatrixId(U16 i_Id) { m_RotInWorldMatrix.m.m03.dummy.u16[1] = i_Id; }

    inline U16 GetInverseWorldMatrixId() const { return m_RotInWorldMatrix.m.m03.dummy.u16[1]; }

    inline void SetInverseWorldMatrixPtr(Mat4x4* i_InvWorldMatrix) { m_RotInWorldMatrix.m.m23.dummy.i32 = (S32)i_InvWorldMatrix; }

    inline Mat4x4& GetInverseWorldMatrix() {
        if (m_Trs.m_Flag & FL_BONENODE_UNK_0x1) {
            m_RotInWorldMatrix.m.m23.dummy.i32 = (S32)gData.MatrixBuffer->GetMatrix(GetInverseWorldMatrixId());
            Inverse2(GetWorldMatrix(), *(Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32);
            DisableBoneFlag(FL_BONENODE_UNK_0x1);
        }
        return *(Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32;
    }

    inline Mat4x4* GetInverseWorldMatrixPtr() { return (Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32; }

    inline void SetName(const Name_Z& i_Name) { m_Name = i_Name; }

    inline const Name_Z& GetName() const { return m_Name; }

    inline UserDefine_ZHdl& GetUserDefine() { return m_UserDefineHdl; }

    inline BoneTRS_Z& GetTRS() { return m_Trs; }

    inline BoneNode_Z* GetHeadSon() const { return m_Trs.m_HeadSon; }

    inline BoneNode_Z* GetParent() const { return (BoneNode_Z*)m_InverseRotInWorldMatrix.m.m03.dummy.i32; }

    inline BoneNode_Z* GetPrev() const { return (BoneNode_Z*)m_InverseRotInWorldMatrix.m.m13.dummy.i32; }

    inline BoneNode_Z* GetNext() const { return (BoneNode_Z*)m_InverseRotInWorldMatrix.m.m23.dummy.i32; }

    inline void SetParent(BoneNode_Z* i_Parent) { m_InverseRotInWorldMatrix.m.m03.dummy.i32 = (S32)i_Parent; }

    inline void SetPrev(BoneNode_Z* i_Prev) { m_InverseRotInWorldMatrix.m.m13.dummy.i32 = (S32)i_Prev; }

    inline void SetNext(BoneNode_Z* i_Next) { m_InverseRotInWorldMatrix.m.m23.dummy.i32 = (S32)i_Next; }

    inline void SetHeadSon(BoneNode_Z* i_HeadSon) { m_Trs.m_HeadSon = i_HeadSon; }

    BoneNode_Z* GetRoot();
    S32 GetNodeCount(S32 i_NodeCount);
    void GetAllSons(BoneNodePtr_ZDA& o_BoneNodes, Bool i_AddThis) const;
    void GetAllNodes(BoneNodePtr_ZDA& o_BoneNodes);
    void AddSon(BoneNode_Z* i_Son, Bool i_IsWorldRelative);
    BoneNode_Z* Clone(BoneNode_Z** io_BoneNode, Bool i_CloneSons);
    static Bool InsertNode(BoneNodePtr_ZDA& io_BoneNodes, const Name_Z& i_NodeName, const Name_Z& i_ParentName);
    void Update(BoneNode_Z* i_Parent);
    void InitNode();
    void BuildSameLink(const BoneNodePtr_ZDA& i_BoneNodes, BoneNode_Z* i_BoneNode);

    inline BoneNode_Z* GetBoneNode(const BoneNodePtr_ZDA& i_BoneNodes, const Name_Z& i_Name) const {
        for (S32 i = 0; i < i_BoneNodes.GetSize(); i++) {
            BoneNode_Z* l_BoneNode = i_BoneNodes[i];
            if (l_BoneNode->GetName() == i_Name) {
                return l_BoneNode;
            }
        }
        return NULL;
    }

    void MarkHandles();

private:
    BoneTRS_Z m_Trs;
    Mat3x3 m_RotInWorldMatrix; // m[0][3] is 2 S/U16 m_WorldMatrixId and m_InverseWorldMatrixId, m[1][3] is Mat4x4* m_WorldMatrixPtr, m[2][3] is Mat4x4* m_InverseWorldMatrixPtr
    Quat m_RotInWorld;
    Mat3x3 m_InverseRotInWorldMatrix; // Mat3x3 assumed, m[0][3] is m_Parent, m[1][3] is m_Prev and m[2][3] is m_Next
    UserDefine_ZHdl m_UserDefineHdl;
    Name_Z m_Name;
};

#endif
