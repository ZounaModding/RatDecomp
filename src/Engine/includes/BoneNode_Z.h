#ifndef _BONENODE_Z_H_
#define _BONENODE_Z_H_
#include "Math_Z.h"
#include "UserDefine_ZHdl.h"
#include "BoneNodeFlag_Z.h"
#include "DynPtrArray_Z.h"

class BoneNode_Z;

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

class BoneNode_Z {
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

    inline void SetRotInWorld(const Quat& i_Rotation) { m_RotInWorld = i_Rotation; }

    inline Quat& GetRotInWorld() { return m_RotInWorld; }

    inline void DisableFlag(U32 i_Flag) { m_Trs.m_Flag &= ~i_Flag; }

    //inline void DisableFlag(U32 i_Flag) { m_Flag &= ~i_Flag; }

    inline void EnableFlag(U32 i_Flag) { m_Trs.m_Flag |= i_Flag; }

    //inline void EnableFlag(U32 i_Flag) { m_Flag |= i_Flag; }

    inline void SetWorldMatrixId(U16 i_Id) { m_RotInWorldMatrix.m.m03.dummy.u16[0] = i_Id; }

    inline U16 GetWorldMatrixId() const { return m_RotInWorldMatrix.m.m03.dummy.u16[0]; }

    inline void SetWorldMatrix(const Mat4x4& i_World) { *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32 = i_World; }

    inline Mat4x4& GetWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline Mat4x4* GetWorldMatrixPtr() { return (Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline void SetInverseWorldMatrixId(U16 i_Id) { m_RotInWorldMatrix.m.m03.dummy.u16[1] = i_Id; }

    inline U16 GetInverseWorldMatrixId() const { return m_RotInWorldMatrix.m.m03.dummy.u16[1]; }

    inline void SetInverseWorldMatrix(const Mat4x4& i_InvWorld) { *(Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32 = i_InvWorld; }

    inline Mat4x4& GetInverseWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32; }

    inline Mat4x4* GetInverseWorldMatrixPtr() { return (Mat4x4*)m_RotInWorldMatrix.m.m23.dummy.i32; }

    inline void SetName(const Name_Z& i_Name) { m_Name = i_Name; }

    inline const Name_Z& GetName() const { return m_Name; }

    inline BoneTRS_Z& GetTRS() { return m_Trs; }

    inline BoneNode_Z* GetHeadSon() { return m_Trs.m_HeadSon; }

    BoneNode_Z* GetRoot();

    void MarkHandles();

private:
    BoneTRS_Z m_Trs;
    // Quat m_Rotation;
    // Vec3f m_Scale;
    // U32 m_Flag;
    // Vec3f m_Translation;
    Mat3x3 m_RotInWorldMatrix; // m[0][3] is 2 S/U16 m_WorldMatrixId and m_InverseWorldMatrixId, m[1][3] is Mat4x4* m_WorldMatrixPtr, m[2][3] is Mat4x4* m_InverseWorldMatrixPtr
    Quat m_RotInWorld;
    Mat3x3 m_InverseRotInWorldMatrix; // Mat3x3 assumed, m[0][3] is m_Parent, m[1][3] is m_PrevSibling and m[2][3] is m_NextSibling
    UserDefine_ZHdl m_UserDefineHdl;
    Name_Z m_Name;
};

typedef DynPtrArray_Z<BoneNode_Z*, 4, 4> BoneNodePtr_ZDA;

#endif
