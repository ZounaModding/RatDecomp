#ifndef _COLLISIONVOL_Z_H_
#define _COLLISIONVOL_Z_H_
#include "Object_Z.h"
#include "AnimFrame_ZHdl.h"
#include "MaterialAnim_ZHdl.h"
#include "Node_ZHdl.h"

#define COLVOL_NB_PARAM 12

struct CollisionVol {
    Mat4x4 m_Mat;
    Mat4x4 m_InvMat;
};

typedef DynArray_Z<CollisionVol, 4, FALSE, FALSE> CollisionVolDA;

class CollisionVol_Z : public Object_Z {
public:
    virtual ~CollisionVol_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();
    virtual void Draw(DrawInfo_Z& a1, ObjectDatas_Z* a2);
    virtual S32 GetCollisions(
        Node_Z* i_Node,
        ObjectDatas_Z* i_Data,
        const Sphere_Z& i_Sph,
        const Vec3f& i_Dir,
        Float i_Len,
        ColLineResult_Z* o_Result,
        S32 i_MaxElem,
        U64 i_Flag,
        U64 i_NoFlag
    );

    static BaseObject_Z* NewObject() { return NewL_Z(112) CollisionVol_Z; }

    inline Node_ZHdl& GetNodeParam(S32 i_Idx) {
        return m_NodeParams[i_Idx];
    }

    inline Float GetFloatParam(S32 i_Idx) const {
        return m_FloatParams[i_Idx];
    }

    inline S32 GetNodeAnimsCount() const {
        return m_AnimFrameHdls.GetSize();
    }

    inline AnimFrame_ZHdl& GetNodeAnim(S32 Id) {
        return m_AnimFrameHdls[Id];
    }

    void StartNodeAnims();
    void StartMatAnims();
    void StopNodeAnims();
    void StopMatAnims();
    void ResetNodeAnims();
    void ResetMatAnims();
    Bool HasAgentClass(Name_Z& o_AgentClassName);

private:
    CollisionVolDA m_CollisionVolInfos;
    abc_message m_InMessage;
    abc_message m_OutMessage;
    Node_ZHdl m_NodeParams[COLVOL_NB_PARAM];
    Float m_FloatParams[COLVOL_NB_PARAM];
    AnimFrame_ZHdlDA m_AnimFrameHdls;
    MaterialAnim_ZHdlDA m_MaterialAnimHdls;
    Name_Z m_AgentClassName;
    Float m_AnimTime;
};

#endif // _COLLISIONVOL_Z_H_
