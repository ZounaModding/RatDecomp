#ifndef _OBJECTTHROW_Z_H_
#define _OBJECTTHROW_Z_H_
#include "ObjectMove_Z.h"
#include "ObjectThrow_ZHdl.h"
#include "Lod_ZHdl.h"
#include "Skel_ZHdl.h"

class ObjectMoveData_Z;
class ObjectMoveSphereColl_Z;

class ObjectThrow_Z : public ObjectMove_Z {
public:
    virtual ~ObjectThrow_Z() { }

    virtual void Init();

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void DisableCollision(World_Z* i_World, Float i_DeltaTime, S32 i_Index);
    virtual void UpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime, S32 i_Index);
    virtual void EndUpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_CurNodePos, const Vec3f& i_NewPos, Float i_DeltaTime, S32 i_Index);
    virtual void RestoreCollision(SeadZone_Z& i_Zone, S32 i_Index);
    virtual Bool UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index);
    virtual void UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index);
    virtual void SetNodeObject(const Node_ZHdl& i_NodeHdl, S32 i_Index);
    virtual void FatherUnLinked(S32 i_Index);
    virtual void Abort();

    virtual void GetSLidingSlopeAngle() { }

    virtual void GetSLidingMaxSlopeAngle() { }

    virtual void GetReflexLimitAngle() { }

    virtual void GetHCollMinAngle() { }

    virtual void UnLinkThrow(ObjectMoveData_Z* i_MoveData);
    virtual void EndLiveTimeObject();

    virtual void Bounce(const ColLineResult_Z& i_Result) { }

    virtual void UpdateCollision(const ColLineResult_Z& i_Result, ObjectMoveSphereColl_Z& io_SphereColl, Float i_DeltaTime, S32 i_Index);

    virtual void UpdateCollisionCarrying(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos) { }

    virtual void SetNodeDepl(const Vec3f& i_Displacement, Float i_DeltaTime) { }

    static BaseObject_Z* NewObject() { return NewL_Z(140) ObjectThrow_Z; }

private:
    Lod_ZHdl m_LodHdl;
    Skel_ZHdl m_SkelHdl;
    Float m_BounceRatio;
    Float m_TimeToLive;
    Float m_BounceRatioWall;
    Float m_EndSpeedBounce;
    S32 m_BounceCount;
    ObjectMove_ZHdl m_TargetHdl;
    ObjectThrow_ZHdl m_SenderHdl;
    Quat m_CurRot;
    Quat m_LastRot;
    Float m_SlerpTime;
    ObjectThrow_ZHdl m_CarryingHdl;
    ObjectThrow_ZHdl m_CarrierHdl;
    Float m_CarryingTime;
    Vec3f m_GroundNormalWorld;
    Vec3f m_ConstrainedNormal;
    Vec3f m_HorizontalColDir;
    Vec3f m_ForceMappingWorldDir;
    Vec3f m_ForceAnimMappingWorldDir;
};

#endif // _OBJECTTHROW_Z_H_
