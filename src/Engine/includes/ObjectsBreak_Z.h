#ifndef _OBJECTSBREAK_Z_H_
#define _OBJECTSBREAK_Z_H_
#include "ObjectsBounce_Z.h"
#include "ObjectThrow_ZHdl.h"
#include "DynPtrArray_Z.h"

class ObjectsBreak_Z : public ObjectsBounce_Z {
public:
    ObjectsBreak_Z() {
    }

    virtual void Init();

    virtual ~ObjectsBreak_Z() { }

    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Update(const Vec3f& a1, const Vec3f& a2, const Vec3f& a3, Float a4, S32 a5);
    virtual void DisableCollision(World_Z* a1, Float a2, S32 a3);
    virtual void EndUpdateCollision(SeadZone_Z& a1, const Vec3f& a2, const Vec3f& a3, const Vec3f& a4, Float a5, S32 a6);
    virtual void RestoreCollision(SeadZone_Z& a1, S32 a2);
    virtual void UpdateShadow(SeadZone_Z& a1, Float a2, S32 a3);
    virtual void UpdateCollisionMsg(Node_Z* a1, const Vec3f& a2, U64 a3, S32 a4);
    virtual void GetSphere(Sphere_Z& a1, U16 a2, S32 a3);
    virtual const Vec3f& GetPos(S32 a1);
    virtual Float GetScale(S32 a1);
    virtual Quat& GetRot(S32 a1);
    virtual void SetPos(const Vec3f& a1, S32 a2);
    virtual void SetPosAndRot(const Vec3f& a1, const Quat& a2, S32 a3);
    virtual void SetPosAndScale(const Vec3f& a1, Float a2, S32 a3);
    virtual void SetPosRotAndScale(const Vec3f& a1, const Quat& a2, Float a3, S32 a4);
    virtual void SetScale(Float a1, S32 a2);
    virtual Bool ParentLink(S32 a1);
    virtual void RestoreStartingPos(S32 a1);
    virtual void StoreStartingPos(S32 a1);

    virtual void ChangeLink(const Node_ZHdl& a1, Node_Z* a2, Bool a3, S32 a4) { }

    virtual void UpdateCollisionNode(SeadZone_Z& a1, const Vec3f& a2, const Sphere_Z& a3, Vec3f& a4, Vec3f& a5, S32 a6, Bool a7);
    virtual void IsVisible();
    virtual void SetNodeObjectNb(S32 a1);
    virtual void CheckObjectMove(S32 a1, Float a2);
    virtual void EndLiveTimeObject(S32 a1);
    virtual void RemoveObjectMove(S32 a1);
    virtual void UpdateCollision(const ColLineResult_Z& a1, ObjectMoveSphereColl_Z& a2, Float a3, S32 a4);

    static BaseObject_Z* NewObject() { return NewL_Z(96) ObjectsBreak_Z; }

private:
    ObjectThrow_ZHdl m_ObjectThrowHdl;
    Vec3fDA m_StartingBoneNodeLocalScaleDA;                   // for BoneNode_Z
    DynPtrArray_Z<BoneNode_Z*, 4> m_StartingBoneNodeParentDA; // for BoneNode_Z
    FloatDA m_StartingObjectScaleDA;
    S32DA m_ObjectDatasIdDA; // Map from object id -> MeshData_Z id in the skel
    DynArray_Z<Quat, 8> m_CurrentObjectRotationDA;
    FloatDA m_CurrentObjectScaleDA;
    Float m_CurrentTime;
};

#endif // _OBJECTSBREAK_Z_H_
