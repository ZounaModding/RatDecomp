#ifndef _ACTIONHELPER_G_H_
#define _ACTIONHELPER_G_H_
#include "Game_ZHdl.h"
#include "Manipulator_Z.h"
#include "Math_Z.h"
#include "Node_ZHdl.h"

class ActionHelperInfos;
class SubParticlesActionHelper;
class Node_Z;
class BoneNode_Z;
class MissionDef_G;

typedef DynArray_Z<ActionHelperInfos, 4> ActionHelperInfosDA;

#define ACTIONHELPER_SUBPARTICLE_ARRAY_SIZE 6

struct SubParticlesActionHelper {
    SubParticlesActionHelper() {
        m_MaterialIndex = -1;
        m_UnkFloat_0x8 = 0.0f;
        m_UnkFloat_0x4 = 0.0f;
    }

    S32 m_MaterialIndex;
    Float m_UnkFloat_0x4;
    Float m_UnkFloat_0x8;
    Float m_DelayTimer;
    Float m_CurrentLifeTimeLeft;
    Float m_MaxLifeTime;
    Float m_UnkFloat_0x18;
    Float m_UnkFloat_0x1c;
    Float m_MaxTimeForSomething;
    Float m_CurrentTimeForSomething;
    Float m_LifeProgress; // $SABE: Maps life time range from 0-1
};

struct ActionHelperInfos {
    ActionHelperInfos() {
        m_HasAction = FALSE;
        m_IsHidden = FALSE;
        m_IsOnScreen = FALSE;
        m_IsActiveOrSomething = FALSE;
        m_OtherTime = 0.0f;
        m_TimeSpentOnScreen = 0.0f;
        m_MissionDef = NULL;
        m_Node = NULL;
        m_BoneNode = NULL;
        m_HasOffset = FALSE;
        m_MaybeOpacity = 0.1f;
    }

    SubParticlesActionHelper m_SubPartricles[ACTIONHELPER_SUBPARTICLE_ARRAY_SIZE];
    U32 m_HasAction; // $SABE: Not a Bool for some reason :|
    Bool m_IsActiveOrSomething;
    Bool m_IsOnScreen;
    Float m_TimeSpentOnScreen;
    Float m_OtherTime;
    Bool m_IsHidden;
    Float m_MaybeOpacity;
    Bool m_HasOffset;    // $SABE: May be m_HasMagnet (related to magnet sphere)
    Vec3f m_LocalOffset; // $SABE: Position offset in Node local space
    Node_Z* m_Node;
    BoneNode_Z* m_BoneNode;
    MissionDef_G* m_MissionDef;
};

// $SABE: Action helper "manager"
class ActionHelper_G : public ManipulatorSceneDraw_Z {
public:
    static BaseObject_Z* NewObject() { return NewL_Z(123) ActionHelper_G; }

    Bool Minimize();
    void StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);
    void StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);

private:
    virtual void Init();

    virtual ~ActionHelper_G() { }

    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);
    virtual void Activate();

private:
    Game_ZHdl m_GameHdl;
    ActionHelperInfosDA m_ActionHelperDA; // $SABE: DynArray of all action helpers
};

#endif // _ACTIONHELPER_G_H_
