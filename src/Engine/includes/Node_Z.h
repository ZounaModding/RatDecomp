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
#include "HFog_Z.h"
#include "NodeFlag_Z.h"
#include "MatrixCache_Z.h"

class World_Z;
class LightData_Z;

class Node_Z : public ResourceObject_Z {
public:
    Node_Z();
    virtual ~Node_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(309) Node_Z; }

    void UpdateRootTM();
    void UpdateTM(Node_Z* i_Parent);
    void GetLocal(const Segment_Z& i_WorldSegment, Segment_Z& o_LocalSegment);
    void GetLocal(const Sphere_Z& i_WorldSph, Sphere_Z& o_LocalSph);
    void GetLocal(const Capsule_Z& i_WorldCapsule, Capsule_Z& o_LocalCapsule);
    void Changed();
    void Changed(S32 i_Flag);
    void AddSon(const Node_ZHdl& i_Son, Bool i_IsWorldRelative = FALSE, Bool i_Changed = TRUE);
    void GetByObjectType(Node_ZHdlDA& o_Nodes, S32 i_Type, Bool i_Next = FALSE);
    void GetByName(const Name_Z& i_Name, Node_ZHdl& o_Node, Bool i_Next = FALSE);
    void GetNodesFromFlag(Node_ZHdlDA& o_Nodes, U32 i_Flag, U32 i_NoFlag, Bool i_Next = FALSE);
    void BuildNonShared(Bool i_Recursive, Bool i_Next);
    void GetAllSons(Node_ZHdlDA& i_Sons, Bool i_Recursive = FALSE) const;
    void GetAllNodes(Node_ZHdlDA& o_Nodes) const;
    void Clone(Node_ZHdl& o_NodeHdl, Bool i_BuildNonShared);
    void Remove(Bool a1, Bool a2);
    Node_ZHdl& GetRoot();
    void Ref(World_Z* i_World, Object_Z* i_Object);

    inline Bool IsRoot() const { return m_Parent == NULL; }

    inline Node_Z* GetParent() const { return m_Parent; }

    inline Node_Z* GetHeadSon() const { return m_HeadSon; }

    inline Node_Z* GetPrev() const { return m_Prev; }

    inline Node_Z* GetNext() const { return m_Next; }

    Object_Z* GetObject(Bool i_Update = FALSE) const {
        Object_Z* l_Object;
        if (i_Update && m_Object) {
            l_Object = (Object_Z*)GETPTR(m_Object->GetHandle());
        }
        else {
            return m_Object;
        }
        return l_Object;
    }

    Object_Z* GetObjectA() const { return m_Object; }

    ObjectDatas_Z* GetObjectDatas() const { return m_ObjectDatas; }

    inline Quat& GetRotInWorld() { return m_RotInWorld; }

    inline Sphere_Z& GetBSphere() { return m_BSphere; }

    inline void SetBSphere(const Sphere_Z& i_Sph) { m_BSphere = i_Sph; }

    inline Mat3x3& GetRotInWorldMatrix() { return m_RotInWorldMatrix; }

    inline void SetWorldMatrix(const Mat4x4& i_World) { *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32 = i_World; }

    inline Mat4x4& GetWorldMatrix() { return *(Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    inline void SetWorldMatrixPtr() {
        U16 l_WorldMatrixId = GetWorldMatrixId();
        m_RotInWorldMatrix.m.m13.dummy.u32 = (U32)gData.MatrixBuffer->GetMatrix(l_WorldMatrixId);
    }

    inline Mat4x4* GetWorldMatrixPtr() { return (Mat4x4*)m_RotInWorldMatrix.m.m13.dummy.i32; }

    void SetRotation(const Quat& i_Rotation);

    inline Quat& GetRotation() { return m_Rotation; }

    void SetScale(Float i_Scale);

    inline Float& GetScale() { return m_Scale; }

    inline Float GetUniformScale() { return m_UniformScale; }

    void SetTranslation(const Vec3f& i_Translation);

    inline Vec3f& GetLocalTranslation() { return m_Translation; }

    void SetFromWorldTrans(const Vec3f& i_Trans);
    void SetFromWorldTransEpsilon(Node_Z* i_Parent, const Vec3f& i_Trans);
    void SetFromWorldTransRot(const Vec3f& i_Trans, const Quat& i_Rot);
    void SetFromWorldTransScale(const Vec3f& i_Trans, Float i_Scale);
    void SetFromWorldScale(Float i_Scale);
    void SetFromWorldTransRotScale(const Vec3f& i_Trans, const Quat& i_Rot, const Float& i_Scale);

    inline const Vec3f& GetTranslation() {
        return GetWorldMatrix().GetMatrixTrans();
    }

    inline Mat4x4& GetInverseWorldMatrix() {
        if (GetFlag() & FL_NODE_INVALIDMAT) {
            Inverse2(GetWorldMatrix(), m_InverseWorldMatrix);
            DisableFlag(FL_NODE_INVALIDMAT);
        }
        return m_InverseWorldMatrix;
    }

    inline Quat GetInverseRotInWorld() {
        return Quat(m_RotInWorld.w, -m_RotInWorld.v.x, -m_RotInWorld.v.y, -m_RotInWorld.v.z);
    }

    inline void SetFlag(U32 i_Flag) { m_Flag = i_Flag; }

    inline void SetFlag(U32 i_Flag, Bool Set) { Set ? m_Flag |= i_Flag : m_Flag &= ~i_Flag; }

    inline U32 GetFlag(void) const { return m_Flag; }

    inline U32 HasFlag(U32 i_Flag) const { return m_Flag & i_Flag; }

    inline void EnableFlag(U32 i_Flag) { m_Flag |= i_Flag; }

    inline void DisableFlag(U32 i_Flag) { m_Flag &= ~i_Flag; }

    void EnableFlag(U32 i_Flag, Bool Resursive);
    void DisableFlag(U32 i_Flag, Bool Resursive);

    inline Bool IsFlagEnable(U32 i_Flag) const {
        return (m_Flag & i_Flag) != 0;
    }

    inline Bool IsFlagEnable(U32 i_Flag, U32 i_TrueFlag) const { return ((m_Flag & i_Flag) == i_TrueFlag); }

    inline Bitmap_Z* GetRadiosityBmap() const { return m_RadiosityBitmap; }

    void Update(World_Z* i_World, Node_Z* i_Parent);

    void Update();

    void LightingChanged();
    void SetLighting(LightData_Z* i_LightData, HFogData_Z* i_HFogData, Bool i_KeepExisting);

    void Hide(Bool i_Recursive = FALSE);
    void HideSons();
    void UnHideSons();
    void UnHide(Bool i_Recursive = FALSE);
    void UnRef();

    Agent_ZHdl& GetAgent() {
        return m_Agent;
    }

    inline void SetLight(LightData_Z* i_LightData) {
        m_LightData = i_LightData;
    }

    inline void SetObject(Object_Z* i_Object) {
        m_Object = i_Object;
        Changed();
    }

    inline void SetCollideSeadId(S32 i_Id) {
        m_RotInWorldMatrix.m.m03.dummy.i32 = i_Id;
    }

    inline S32& GetCollideSeadId() {
        return m_RotInWorldMatrix.m.m03.dummy.i32;
    }

    inline void SetDisplaySeadId(S32 i_Id) {
        m_RotInWorldMatrix.m.m23.dummy.i32 = i_Id;
    }

    inline S32& GetDisplaySeadId() {
        return m_RotInWorldMatrix.m.m23.dummy.i32;
    }

    inline void SetWorldId(S16 i_Id) {
        m_InverseRotInWorldMatrix.m.m23.dummy.i16[0] = i_Id;
    }

    void SetWorldId(S16 i_Id, Bool i_Recursive);

    inline S16 GetWorldId() const {
        return m_InverseRotInWorldMatrix.m.m23.dummy.i16[0];
    }

    inline void SetStart(Float i_Value) {
        m_InverseRotInWorldMatrix.m.m03.dummy.f32 = i_Value;
    }

    inline Float& GetStart() {
        return m_InverseRotInWorldMatrix.m.m03.dummy.f32;
    }

    inline void SetEnd(Float i_Value) {
        m_InverseRotInWorldMatrix.m.m13.dummy.f32 = i_Value;
    }

    inline Float& GetEnd() {
        return m_InverseRotInWorldMatrix.m.m13.dummy.f32;
    }

    inline void SetWorldMatrixId(U16 i_Id) { m_InverseRotInWorldMatrix.m.m23.dummy.u16[1] = i_Id; }

    inline U16 GetWorldMatrixId() { return m_InverseRotInWorldMatrix.m.m23.dummy.u16[1]; }

    inline U32 GetOccluderZoneId() { return m_OccluderZoneStartId; }

    inline void SetOccluderZoneId(U32 i_Id) { m_OccluderZoneStartId = i_Id; }

    inline void ClrOccluderZoneId() { m_OccluderZoneStartId = 0; }

    inline HFogData_Z* GetHFog() const { return m_HFogData; }

    inline LightData_Z* GetLight() const { return m_LightData; }

    Float GetInverseWorldScale() const { return m_InverseUniformScale; }

    Mat3x3& GetInverseMatRotInWorld() {
        if (IsFlagEnable(FL_NODE_INVALIDROT)) {
            GetInverseRotInWorld().GetMatrix(m_InverseRotInWorldMatrix);
            DisableFlag(FL_NODE_INVALIDROT);
        }
        return m_InverseRotInWorldMatrix;
    }

private:
    Agent_ZHdl m_Agent;
    Mat4x4 m_InverseWorldMatrix;
    Mat3x3 m_RotInWorldMatrix;        // has 3 hidden values (cause it's 3x4) mat[0][3] = collideSeadsId1, mat[1][3] = placeholderWorldMatrixPtr, mat[2][3] = displaySeadsId1
    Mat3x3 m_InverseRotInWorldMatrix; // has 3 hidden values (cause it's 3x4) mat[0][3] = Sound Attenuation Start Distance, mat[1][3] = Sound Attenuation End Distance and mat[2][3] is 2 uint16's (worldId and worldMatrixId)
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
    ObjectDatas_Z* m_ObjectDatas;
    Object_Z* m_Object;
    UserDefine_Z* m_UserDefine;
    Node_Z* m_Parent;
    Node_Z* m_HeadSon;
    Node_Z* m_Prev;
    Node_Z* m_Next;
    LightData_Z* m_LightData;
    HFogData_Z* m_HFogData;
    Bitmap_Z* m_RadiosityBitmap;
    Bitmap_Z* m_RadiosityNormalBitmap; // From Monopoly MAP
};

#endif
