#ifndef _OBJECTSMOVE_Z_H_
#define _OBJECTSMOVE_Z_H_
#include "ObjectMove_Z.h"
#include "HoleArray_Z.h"
#include "Game_ZHdl.h"

class ObjectMoveSphereColl_Z;
class BoneNode_Z;

struct ObjectMove {
    Vec3f m_TotalSpeed;
    Vec3f m_Accel;
    Float m_TimeToLive;
    Float m_TimeToFade;
    Float m_TimeToScale;
    Float m_BounceRatio;
    Float m_ShadowOldRayLen;
    Float m_BounceRatioWall;
    Float m_EndSpeedBounce;
    U64 m_CollisionBools;
    Node_ZHdl m_NodeHdl;
    BoneNode_Z* m_BoneNode;
    ObjectMove_ZHdl m_SenderHdl;
    ObjectMove_ZHdl m_OwnerHdl;
    ObjectMove_ZHdl m_TargetHdl;
    StaticArray_Z<Node_ZHdl, 8> m_CollisionVolNodeHdls;
    ObjectMoveCollNode_Z m_CollideNode;

    ObjectMove() {
        m_NodeHdl = HANDLE_NULL;
        m_BoneNode = NULL;
        m_SenderHdl = HANDLE_NULL;
        m_TargetHdl = HANDLE_NULL;
        m_TotalSpeed = VEC3F_NULL;
    }

    ~ObjectMove() {
        m_NodeHdl = HANDLE_NULL;
        m_OwnerHdl = HANDLE_NULL;
        m_SenderHdl = HANDLE_NULL;
        m_TargetHdl = HANDLE_NULL;
        m_TotalSpeed = VEC3F_NULL;
        m_Accel = VEC3F_NULL;
        m_TimeToLive = -1.0f;
        m_BounceRatio = 0.0f;
        m_ShadowOldRayLen = -1.0f;
        m_CollideNode.m_NodeHdl = HANDLE_NULL;
        FIXDEBUGINLINING_Z();
    }
};

class ObjectsMove_Z : public ObjectMove_Z {
public:
    ObjectsMove_Z() {
        m_BankDataId = -1;
        // TODO: See if this can be removed
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
        FIXDEBUGINLINING_Z();
    }

    virtual void Init();

    virtual ~ObjectsMove_Z() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void ComputeNewPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime, S32 i_Index);

    virtual void ResetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot) {
        Reset();
    }

    virtual void DisableCollision(World_Z* i_World, Float i_DeltaTime, S32 i_Index);
    virtual void UpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime, S32 i_Index);
    virtual void RestoreCollision(SeadZone_Z& i_Zone, S32 i_Index);
    virtual void SetCollideData(const Node_ZHdl& i_NodeHdl, const ObjectMoveCollData_Z& i_CollideData);
    virtual void UpdateShadow(SeadZone_Z& i_Zone, Float i_DeltaTime, S32 i_Index);
    virtual void UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index);

    virtual void GetName() { }

    virtual Node_ZHdl& GetNode(S32 i_Index) {
        return m_Objects[i_Index].m_NodeHdl;
    }

    virtual const Vec3f& GetPos(S32 i_Index);
    virtual Float GetScale(S32 i_Index);
    virtual Quat& GetRot(S32 i_Index);
    virtual void SetPos(const Vec3f& i_Pos, S32 i_Index);
    virtual void SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index);
    virtual void SetPosAndScale(const Vec3f& i_Pos, Float i_Scale, S32 i_Index);
    virtual void SetPosRotAndScale(const Vec3f& i_Pos, const Quat& i_Rot, Float i_Scale, S32 i_Index);
    virtual void SetScale(Float i_Scale, S32 i_Index);

    virtual Vec3f& GetLastEffectiveSpeed(S32 i_Index) {
        return m_Objects[i_Index].m_TotalSpeed;
    }

    virtual Vec3f& GetLastEffectiveAccel(S32 i_Index) {
        return m_Objects[i_Index].m_Accel;
    }

    virtual void FatherUnLinked(S32 i_Index);
    virtual void UpdateDynamic(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos, S32 i_Index);
    virtual void InitLinkCollisionNode(ColLineResult_Z& i_Result, const Vec3f& i_Orient, S32 i_Index);
    virtual void EndCollisionNode(Bool i_Abort, S32 i_Index);
    virtual void GetLinkCollisionNode(Vec3f& o_Intersection, Vec3f& o_InterGround, Vec3f& o_Local, Vec3f& o_LocalLookAt, S32 i_Index);

    virtual StaticArray_Z<Node_ZHdl, 8>* GetVolumeMemory(S32 i_Index) {
        return &m_Objects[i_Index].m_CollisionVolNodeHdls;
    }

    virtual void Shut();
    virtual void IsVisible();
    virtual void SetNodeObjectNb(S32 i_Count);

    virtual void GetBoneNodeObject(S32 i_Index) { }

    virtual void CheckObjectMove(S32 i_Index, Float i_DeltaTime) { }

    virtual void EndLiveTimeObject(S32 i_Index);
    virtual void RemoveObjectMove(S32 i_Index);
    virtual void UpdateCollision(const ColLineResult_Z& i_Result, ObjectMoveSphereColl_Z& io_SphereColl, Float i_DeltaTime, S32 i_Index);

    static BaseObject_Z* NewObject() { return NewL_Z(230) ObjectsMove_Z; }

private:
    Game_ZHdl m_GameHdl;
    Name_Z m_TitleName;
    S32 m_BankDataId;
    Object_ZHdl m_BankObjectHdl;
    abc_message m_MsgEnd;
    abc_message m_MsgHit;
    HoleArray_Z<ObjectMove, 1> m_Objects;
};

#endif // _OBJECTSMOVE_Z_H_
