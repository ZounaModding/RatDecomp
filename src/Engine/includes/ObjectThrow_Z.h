#ifndef _OBJECTTHROW_Z_H_
#define _OBJECTTHROW_Z_H_
#include "ObjectMove_Z.h"
#include "ObjectThrow_ZHdl.h"

class ObjectMoveData_Z;
class ObjectMoveSphereColl_Z;

class ObjectThrow_Z : public ObjectMove_Z {
public:
    virtual ~ObjectThrow_Z();
    virtual void Init();

    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Update(const Vec3f& a1, const Vec3f& a2, const Vec3f& a3, Float a4, S32 a5);
    virtual void DisableCollision(World_Z* a1, Float a2, S32 a3);
    virtual void UpdateCollision(SeadZone_Z& a1, const Vec3f& a2, Vec3f& a3, Vec3f& a4, Vec3f& a5, Float a6, S32 a7);
    virtual void EndUpdateCollision(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4, Float a5, S32 a6);
    virtual void RestoreCollision(SeadZone_Z& a1, S32 a2);
    virtual void UpdateCollisionLineFlag(const ColLineResult_Z& a1, const Vec3f& a2, S32 a3);
    virtual void UpdateCollisionMsg(Node_Z* a1, const Vec3f& a2, U64 a3, S32 a4);
    virtual void SetNodeObject(const Node_ZHdl& a1, S32 a2);
    virtual void FatherUnLinked(S32 a1);
    virtual void Abort();
    virtual void GetSLidingSlopeAngle();
    virtual void GetSLidingMaxSlopeAngle();
    virtual void GetReflexLimitAngle();
    virtual void GetHCollMinAngle();
    virtual void UnLinkThrow(ObjectMoveData_Z* a1);
    virtual void EndLiveTimeObject();
    virtual void Bounce(const ColLineResult_Z& a1);
    virtual void UpdateCollision(const ColLineResult_Z& a1, ObjectMoveSphereColl_Z& a2, Float a3, S32 a4);
    virtual void UpdateCollisionCarrying(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3);
    virtual void SetNodeDepl(const Vec3f& a1, Float a2);
};

#endif // _OBJECTTHROW_Z_H_
