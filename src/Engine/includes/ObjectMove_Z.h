#ifndef _OBJECTMOVE_Z_H_
#define _OBJECTMOVE_Z_H_
#include "Manipulator_Z.h"
#include "Node_Z.h"
#include "ObjectMove_ZHdl.h"
#include "Node_ZHdl.h"
#include "Agent_ZHdl.h"

class SeadZone_Z;
class World_Z;
class ObjectMoveCollData_Z;

class ObjectMove_Z : public Manipulator_Z {
public:
    virtual ~ObjectMove_Z();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Update(const Vec3f& a1, const Vec3f& a2, const Vec3f& a3, Float a4, S32 a5);
    virtual void Update(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4, Float a5, S32 a6);
    virtual void ComputeNewPos(const Vec3f& a1, Vec3f& a2, Vec3f& a3, Float a4, S32 a5);
    virtual void ResetPosAndRot(const Vec3f& a1, const Quat& a2);
    virtual void DisableCollision(World_Z* a1, Float a2, S32 a3);
    virtual void BeforeUpdateCollision(SeadZone_Z& a1, const Vec3f& a2);
    virtual void UpdateCollision(SeadZone_Z& a1, const Vec3f& a2, Vec3f& a3, Vec3f& a4, Vec3f& a5, Float a6, S32 a7);
    virtual void EndUpdateCollision(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4, Float a5, S32 a6);
    virtual void RestoreCollision(SeadZone_Z& a1, S32 a2);
    virtual void UpdateCollisionCollectable(SeadZone_Z& a1, const Vec3f& a2);
    virtual void UpdateSphereCollisionCollectable(SeadZone_Z& a1, const Sphere_Z& a2);
    virtual void SetCollideData(const Node_ZHdl& a1, const ObjectMoveCollData_Z& a2);
    virtual void UpdateShadow(SeadZone_Z& a1, Float a2, S32 a3);
    virtual void UpdateCollisionShadow(World_Z* a1, Float a2);
    virtual void UpdateCollisionLineFlag(const ColLineResult_Z& a1, const Vec3f& a2, S32 a3);
    virtual void UpdateCollisionSphereFlag(const ColSphereResult_Z& a1, const Vec3f& a2);
    virtual void UpdateCollisionMsg(Node_Z* a1, const Vec3f& a2, U64 a3, S32 a4);
    virtual void UpdateDepth(SeadZone_Z& a1, const Vec3f& a2);
    virtual void GetDepthAdd();
    virtual void GetDepthOrigin();
    virtual void SetDepthValue(Float a1);
    virtual void SetDepthOrigin(Float a1);
    virtual void SetNoDepthFound();
    virtual void GetName();
    virtual void SetNodeObject(const Node_ZHdl& a1, S32 a2);
    virtual void GetSphere(Sphere_Z& a1, U16 a2, S32 a3);
    virtual void GetNode(S32 a1);
    virtual void IsNodeFlag(U32 a1, S32 a2);
    virtual void SetNodeFlag(U32 a1, S32 a2);
    virtual void EnableNodeFlag(U32 a1, S32 a2);
    virtual void DisableNodeFlag(U32 a1, S32 a2);
    virtual void GetPos(S32 a1);
    virtual void GetScale(S32 a1);
    virtual void GetRot(S32 a1);
    virtual void SetPos(const Vec3f& a1, S32 a2);
    virtual void SetPosAndRot(const Vec3f& a1, const Quat& a2, S32 a3);
    virtual void SetPosAndScale(const Vec3f& a1, Float a2, S32 a3);
    virtual void SetPosRotAndScale(const Vec3f& a1, const Quat& a2, Float a3, S32 a4);
    virtual void SetScale(Float a1, S32 a2);
    virtual void ParentLink(S32 a1);
    virtual void RestoreStartingPos(S32 a1);
    virtual void StoreStartingPos(S32 a1);
    virtual void ChangeLink(const Node_ZHdl& a1, Node_Z* a2, bool a3, S32 a4);
    virtual void GetLastEffectiveSpeed(S32 a1);
    virtual void GetLastEffectiveAccel(S32 a1);
    virtual void FatherUnLinked(S32 a1);
    virtual void Abort();
    virtual void UpdateDynamic(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3, S32 a4);
    virtual void UpdateVolume(SeadZone_Z& a1, bool a2);
    virtual void UpdateCollisionVolume(World_Z* a1, bool a2);
    virtual void UpdateCollisionNode(SeadZone_Z& a1, const Vec3f& a2, const Sphere_Z& a3, Vec3f& a4, Vec3f& a5, S32 a6, bool a7);
    virtual void GetCollisionNodeDepl(const Vec3f& a1, Vec3f& a2, Vec3f& a3);
    virtual void InitLinkCollisionNode(ColLineResult_Z& a1, const Vec3f& a2, S32 a3);
    virtual void EndCollisionNode(bool a1, S32 a2);
    virtual void GetLinkCollisionNode(Vec3f& a1, Vec3f& a2, Vec3f& a3, Vec3f& a4, S32 a5);
    virtual void GetSpeed(void) const;
    virtual void GetAcceleration(void) const;
    virtual void GetVolumeMemory(S32 a1);

    inline Bool IsOwnerCollisionRestored() {
        return !m_OwnerHdl.IsValid();
    }

    inline void SetOwnerNoCollision(ObjectMove_ZHdl& i_Owner) {
        m_OwnerHdl = i_Owner;
    }

    inline ObjectMove_ZHdl& GetOwnerNoCollision() {
        return m_OwnerHdl;
    }

private:
    Node_ZHdl m_NodeHdl;
    Agent_ZHdl m_AgentHdl;
    ObjectMove_ZHdl m_OwnerHdl;
    U8 m_Pad_0x2c[0x194];
};

#endif // _OBJECTMOVE_Z_H_
