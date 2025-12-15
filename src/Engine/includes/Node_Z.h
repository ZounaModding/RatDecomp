#ifndef _NODE_Z_H_
#define _NODE_Z_H_
#include "BaseObject_Z.h"
#include "Bitmap_Z.h"
#include "Math_Z.h"
#include "Color_Z.h"
#include "Object_Z.h"
#include "ResourceObject_Z.h"
#include "SystemObject_Z.h"
#include "UserDefine_Z.h"
#include "Agent_ZHdl.h"
#include "Agent_Z.h"
#include "HFog_Z.h"
#include "NodeFlag_Z.h"

class LightData_Z;

class Node_Z : public ResourceObject_Z {
public:
    void UpdateRootTM();
    void UpdateTM(Node_Z* i_Parent);
    void GetLocal(const Segment_Z& i_WorldSegment, Segment_Z& o_LocalSegment);
    void GetLocal(const Sphere_Z& i_WorldSph, Sphere_Z& o_LocalSph);
    void GetLocal(const Capsule_Z& i_WorldCapsule, Capsule_Z& o_LocalCapsule);
    void Changed();

    Object_Z* GetObject(Bool) const;

    inline Quat& GetRotInWorld() { return m_RotInWorld; }

    inline U32 GetFlag(void) const { return m_Flag; }

    inline void DisableFlag(U32 i_Flag) { m_Flag &= ~i_Flag; }

    inline void EnableFlag(U32 i_Flag) { m_Flag |= i_Flag; }

    inline Sphere_Z& GetBSphere() { return m_BSphere; }

    inline void SetBSphere(const Sphere_Z& i_Sph) { m_BSphere = i_Sph; }

    inline Mat3x3& GetRotInWorldMatrix() { return m_RotInWorldMatrix; }

    inline void SetWorldMatrix(const Mat4x4& i_World) { *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32 = i_World; }

    inline Mat4x4& GetWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline Mat4x4* GetWorldMatrixPtr() { return (Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    void SetRotation(const Quat& i_Rotation);

    inline Quat& GetRotation() { return m_Rotation; }

    void SetScale(Float i_Scale);

    inline Float GetUniformScale() { return m_UniformScale; }

    void SetTranslation(const Vec3f& i_Translation);

    inline Mat4x4& GetInverseWorldMatrix() {
        if (GetFlag() & FL_NODE_INVALIDMAT) {
            Inverse2(GetWorldMatrix(), m_InverseWorldMatrix);
            DisableFlag(FL_NODE_INVALIDMAT);
        }
        return m_InverseWorldMatrix;
    }

    inline Quat GetInverseRotInWorld() {
        Quat l_Quat(m_RotInWorld);
        l_Quat.SetInverse();
        return l_Quat;
    }

private:
    Agent_ZHdl m_Agent;
    Mat4x4 m_InverseWorldMatrix;
    Mat3x3 m_RotInWorldMatrix;
    Mat3x3 m_InverseRotInWorldMatrix;
    Quat m_RotInWorld;
    Vec3f m_Translation;
    U32 m_Flag;
    Quat m_Rotation;
    Float m_Scale;
    Float m_UniformScale;
    Float m_InverseUniformScale;
    U32 m_OccluderZoneStartId; // Similar to the one in Surface_Z | index into the occluder zone index DA of Occluder_Z
    Color m_Color;
    Sphere_Z m_BSphere;
    Rect_Z m_DisplaySeadRect;
    Rect_Z m_CollideSeadRect;
    ObjectDatas_Z* m_ObjDatas;
    Object_Z* m_Object;
    UserDefine_Z* m_UserDefine;
    Node_Z* m_Parent;
    Node_Z* m_HeadSon;
    Node_Z* m_PrevSibling;
    Node_Z* m_NextSibling;
    LightData_Z* m_LightData; // needs actually defined.
    HFogData_Z* m_FogData;
    Bitmap_Z* m_BitmapRadiosityRelated;
    BaseObject_Z* m_UnkObj;
};

#endif
