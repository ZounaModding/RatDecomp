#ifndef _BONENODE_Z_H_
#define _BONENODE_Z_H_
#include "Math_Z.h"
#include "UserDefine_ZHdl.h"
#include "BoneNodeFlag_Z.h"

class BoneNode_Z {
public:
    void UpdateTM(BoneNode_Z* i_Parent);

    inline void SetTranslation(const Vec3f& i_Translation) { m_Translation = i_Translation; }

    inline Vec3f& GetTranslation() { return m_Translation; }

    inline void SetScale(const Vec3f& i_Scale) { m_Scale = i_Scale; }

    inline Vec3f& GetScale() { return m_Scale; }

    inline void SetRotation(const Quat& i_Rotation) { m_Rotation = i_Rotation; }

    inline Quat& GetRotation() { return m_Rotation; }

    inline void SetRotInWorld(const Quat& i_Rotation) { m_RotInWorld = i_Rotation; }

    inline Quat& GetRotInWorld() { return m_RotInWorld; }

    inline void DisableFlag(U32 i_Flag) { m_Flag &= ~i_Flag; }

    inline void EnableFlag(U32 i_Flag) { m_Flag |= i_Flag; }

    inline Mat4x4& GetWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

private:
    Quat m_Rotation;
    Vec3f m_Scale;
    U32 m_Flag;
    Vec3f m_Translation;
    BoneNode_Z* m_HeadSon;
    Mat3x3 m_RotInWorldMatrix; // m[0][3] is 2 S/U16 m_WorldMatrixId and m_InverseWorldMatrixId, m[1][3] is Mat4x4* m_WorldMatrixPtr, m[2][3] is Mat4x4* m_InverseWorldMatrixPtr
    Quat m_RotInWorld;
    Mat3x3 m_InverseRotInWorldMatrix; // Mat3x3 assumed, m[0][3] is m_Parent, m[1][3] is m_PrevSibling and m[2][3] is m_NextSibling
    UserDefine_ZHdl m_UserDefineHdl;
    Name_Z m_BoneName;
};

#endif
