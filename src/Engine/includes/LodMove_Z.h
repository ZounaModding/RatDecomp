#ifndef _LODMOVE_Z_H_
#define _LODMOVE_Z_H_
#include "ObjectThrow_Z.h"

class LodMove_Z : public ObjectThrow_Z {
public:
    virtual ~LodMove_Z();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index);
    virtual void DisableCollision(World_Z* i_World, Float i_DeltaTime, S32 i_Index);
    virtual void UpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime, S32 i_Index);
    virtual void RestoreCollision(SeadZone_Z& i_Zone, S32 i_Index);
    virtual void UpdateCollisionCollectable(SeadZone_Z& i_Zone, const Vec3f& i_Pos);
    virtual void UpdateShadow(SeadZone_Z& i_Zone, Float i_DeltaTime, S32 i_Index);
    virtual Bool UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index);
    virtual const Vec3f& GetPos(S32 i_Index);
    virtual void SetPos(const Vec3f& i_Pos, S32 i_Index);
    virtual void SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index);
    virtual void EndCollisionNode(bool i_Abort, S32 i_Index);
    virtual void UpdateCollisionCarrying(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos);
    virtual void GetCameraId();
    virtual void SetCameraAgent(Agent_ZHdl& i_AgentHdl);
    virtual void SetCameraDirFromZone(Vec3f& i_Dir);
    virtual void SetCameraPos(Vec3f& i_Pos);
    virtual void GetCameraDirFromZone();
    virtual void GetCameraPos();
    virtual void GetCameraNodeDisable();
    virtual void SetCameraNodeDisable(Node_ZHdl& i_NodeHdl);
    virtual void UpdateCollisionCling(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime);
    virtual void ClingEdgeBack();
    virtual void ClingEdgeAddY();
    virtual void ClingEdgeRayAdd();
    virtual void ClingEdgeLenMin();
    virtual void ClingEdgeLandDist();

    static BaseObject_Z* NewObject() { return NewL_Z(138) LodMove_Z; }

private:
    ObjectMove_ZHdl m_VehicleHdl;
    Vec3f m_PivotPoint; // offset from node to objectmove?
    Vec3f m_CameraFocus;
    Vec3f m_VehicleDelta;
    Vec3f m_CameraDistance;
    Vec3f m_CameraDelta;
    Vec3f m_DownVector;
    Bool m_IsFocusCamera;
    Bool m_IsFixedCamera;
    Node_ZHdl m_DisableCameraFromNodeHdl;
    Float m_StuckToGroundVectorLength;
    S32 m_LinkBoneId;
    S32 m_RightLinkBoneId; // shoulder or forearm, or arm
    S32 m_LeftLinkBoneId;  // shoulder or forearm, or arm
    Bool m_UseOldMethod;
    Bool m_IsPlayer;
    Bool m_EnableYSync;
    Float m_LastCorrectY;
    Bool m_LastYSet;
    Quat m_CarryingObjectLocalRot;
    Quat m_CarryingObjectLocalOrientedRot;
    Vec3f m_CarryingObjectLocalLinkTrans;
    Vec3f m_CarryingObjectLocalTrans;
    Vec3f m_CarryingPosA; // interpolation coefficients
    Vec3f m_CarryingPosB; // interpolation coefficients
    Vec3f m_CarryingPosC; // interpolation coefficients
    Vec3f m_CarryingPosD; // interpolation coefficients
    ColLineResult_Z m_CarryingColResult;
};

#endif // _LODMOVE_Z_H_
