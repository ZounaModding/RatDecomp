#ifndef _OBJECTMOVE_Z_H_
#define _OBJECTMOVE_Z_H_
#include "Manipulator_Z.h"
#include "Node_Z.h"
#include "ObjectMove_ZHdl.h"
#include "Node_ZHdl.h"
#include "Agent_ZHdl.h"
#include "ObjectMoveCollData_Z.h"
#include "ObjectMoveCollNode_Z.h"

class SeadZone_Z;
class World_Z;
class ObjectMoveCollData_Z;

class ObjectMove_Z : public Manipulator_Z {
public:
    static ColLineResult_Z ColLineResult;

    ObjectMove_Z() { }

    virtual ~ObjectMove_Z() { }

    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void Update(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void ComputeNewPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void ResetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot);
    virtual void DisableCollision(World_Z* i_World, Float i_DeltaTime, S32 i_Index);
    virtual void BeforeUpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_Pos);
    virtual void UpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime, S32 i_Index);
    virtual void EndUpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_CurNodePos, const Vec3f& i_NewPos, Float i_DeltaTime, S32 i_Index);
    virtual void RestoreCollision(SeadZone_Z& i_Zone, S32 i_Index);
    virtual void UpdateCollisionCollectable(SeadZone_Z& i_Zone, const Vec3f& i_Pos);

    virtual void UpdateSphereCollisionCollectable(SeadZone_Z& i_Zone, const Sphere_Z& i_Sphere) { }

    virtual void SetCollideData(const Node_ZHdl& i_NodeHdl, const ObjectMoveCollData_Z& i_CollideData) { }

    virtual void UpdateShadow(SeadZone_Z& i_Zone, Float i_DeltaTime, S32 i_Index);
    virtual void UpdateCollisionShadow(World_Z* i_World, Float i_DeltaTime);
    virtual Bool UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index);
    virtual Bool UpdateCollisionSphereFlag(const ColSphereResult_Z& i_Result, const Vec3f& i_Pos);
    virtual void UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index);
    virtual void UpdateDepth(SeadZone_Z& i_Zone, const Vec3f& i_Pos);

    virtual void GetDepthAdd() { }

    virtual void GetDepthOrigin() { }

    virtual void SetDepthValue(Float i_Value) { }

    virtual void SetDepthOrigin(Float i_Origin) { }

    virtual void SetNoDepthFound() { }

    virtual void GetName() { }

    virtual void SetNodeObject(const Node_ZHdl& i_NodeHdl, S32 i_Index);
    virtual void GetSphere(Sphere_Z& o_Sphere, U16 i_Type, S32 i_Index);

    virtual Node_ZHdl& GetNode(S32 i_Index) {
    }

    virtual void IsNodeFlag(U32 i_Flag, S32 i_Index) { }

    virtual void SetNodeFlag(U32 i_Flag, S32 i_Index) { }

    virtual void EnableNodeFlag(U32 i_Flag, S32 i_Index) { }

    virtual void DisableNodeFlag(U32 i_Flag, S32 i_Index) { }

    virtual const Vec3f& GetPos(S32 i_Index);
    virtual Float GetScale(S32 i_Index);
    virtual Quat& GetRot(S32 i_Index);
    virtual void SetPos(const Vec3f& i_Pos, S32 i_Index);
    virtual void SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index);
    virtual void SetPosAndScale(const Vec3f& i_Pos, Float i_Scale, S32 i_Index);
    virtual void SetPosRotAndScale(const Vec3f& i_Pos, const Quat& i_Rot, Float i_Scale, S32 i_Index);
    virtual void SetScale(Float i_Scale, S32 i_Index);
    virtual Bool ParentLink(S32 i_Index);
    virtual void RestoreStartingPos(S32 i_Index);
    virtual void StoreStartingPos(S32 i_Index);
    virtual void ChangeLink(const Node_ZHdl& i_NodeHdl, Node_Z* i_Parent, Bool i_Update, S32 i_Index);

    virtual Vec3f& GetLastEffectiveSpeed(S32 i_Index) {
    }

    virtual Vec3f& GetLastEffectiveAccel(S32 i_Index) {
    }

    virtual void FatherUnLinked(S32 i_Index);
    virtual void Abort();
    virtual void UpdateDynamic(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos, S32 i_Index);
    virtual void UpdateVolume(SeadZone_Z& i_Zone, Bool i_IsInit);
    virtual void UpdateCollisionVolume(World_Z* i_World, Bool i_IsInit);
    virtual void UpdateCollisionNode(SeadZone_Z& i_Zone, const Vec3f& i_Pos, const Sphere_Z& i_Sphere, Vec3f& io_Orient, Vec3f& io_Dir, S32 i_Index, Bool i_GetDisplacement);
    virtual void GetCollisionNodeDepl(const Vec3f& i_CurPos, Vec3f& o_Orient, Vec3f& o_Dir);
    virtual void InitLinkCollisionNode(ColLineResult_Z& i_Result, const Vec3f& i_Orient, S32 i_Index);
    virtual void EndCollisionNode(Bool i_Abort, S32 i_Index);

    virtual void GetLinkCollisionNode(Vec3f& o_Intersection, Vec3f& o_InterGround, Vec3f& o_Local, Vec3f& o_LocalLookAt, S32 i_Index) { }

    virtual void GetSpeed(void) const { }

    virtual void GetAcceleration(void) const { }

    virtual StaticArray_Z<Node_ZHdl, 8>* GetVolumeMemory(S32 i_Index) {
        return NULL;
    }

    void SetCollideData(const ObjectMoveCollData_Z& i_CollideData, S32 i_Index);
    Float ComputeGotoPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime);
    void DeadCodeUseUndefinedFValue();

    static BaseObject_Z* NewObject() { return NewL_Z(276) ObjectMove_Z; }

    inline void EnableFlag(U64 i_Flag) {
        m_CollisionBools |= i_Flag;
    }

    inline void DisableFlag(U64 i_Flag) {
        m_CollisionBools &= ~i_Flag;
    }

    inline Bool IsFlagEnable(U64 i_Flag) {
        return (m_CollisionBools & i_Flag) ? TRUE : FALSE;
    }

    inline Bool IsOwnerCollisionRestored() {
        return !m_OwnerHdl.IsValid();
    }

    inline void SetOwnerNoCollision(ObjectMove_ZHdl& i_Owner) {
        m_OwnerHdl = i_Owner;
    }

    inline ObjectMove_ZHdl& GetOwnerNoCollision() {
        return m_OwnerHdl;
    }

    inline S32 GetNodeObjectId(Node_ZHdl& i_NodeHdl) const {
        return 0;
    }

private:
    Node_ZHdl m_NodeHdl;
    Agent_ZHdl m_AgentHdl;
    ObjectMove_ZHdl m_OwnerHdl;
    StaticArray_Z<Node_ZHdl, 8> m_CollisionVolNodeHdls;
    U64 m_CollisionBools;
    Quat m_CurrentAlignRot;
    DynArray_Z<Vec3f, 1> m_StartingLocalPosDA;          // for Node_Z or BoneNode_Z
    DynArray_Z<Quat, 1> m_StartingLocalRotDA;           // for Node_Z or BoneNode_Z
    DynArray_Z<Float, 1> m_StartingLocalScaleDA;        // for Node_Z
    DynArray_Z<Node_ZHdl, 1> m_StartingNodeParentHdlDA; // for Node_Z
    DynArray_Z<ObjectMoveCollData_Z, 1> m_CollideDataDA;
    U64 m_Flag;
    Node_ZHdl m_LinkedNodeHdl;
    ObjectMoveCollNode_Z m_CollideNode;
    Float m_ShadowOldRayLen;
    Vec3f m_PosA; // Interpolation coefficients
    Vec3f m_PosB; // Interpolation coefficients
    Vec3f m_PosC; // Interpolation coefficients
    Vec3f m_PosD; // Interpolation coefficients
    Float m_Sinh;
    Float m_Omega;
    Quat m_RotStart;
    Quat m_RotEnd;
    Node_ZHdl m_SplineNodeHdl;
    Vec3f m_SplineWorldDelta;
    Node_ZHdl m_UnkNodeHdl_0x190;
    Float m_AnimTime;
    Float m_GotoPosTime;
    Vec3f m_TotalSpeed;
    Vec3f m_Accel;
    Vec3f m_PrevTotalSpeed;
};

#endif // _OBJECTMOVE_Z_H_
