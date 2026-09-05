#ifndef _OBJECTSGAME_Z_H_
#define _OBJECTSGAME_Z_H_
#include "ObjectGame_Z.h"
#include "AnimMessage_Z.h"
#include "Skel_Z.h"
#include "ObjectsGame_ZHdl.h"
#include "PlayParticles_ZHdl.h"
#include "DynPtrArray_Z.h"
#include "StaticArray_Z.h"

class ObjectMove_Z;
class Node_Z;
class Agent_Z;
class PlayParticles_Z;
class Particles_Z;
class PlayAnim_Z;
class Skel_Z;
class AnimatedAgent_Z;
class World_Z;

struct ObjectMoveGame_Z {
    ObjectMove_Z* m_ObjectMove;
    Node_Z* m_Node;
    Agent_Z* m_Agent;
    Name_Z m_NodeName;
};

typedef DynArray_Z<ObjectMoveGame_Z, 16, FALSE> ObjectMoveGame_ZDA;

struct PlayParticlesGame_Z {
    PlayParticles_Z* m_PlayParticles;
    Node_Z* m_Node;
    Agent_Z* m_Agent;
    Particles_Z* m_Particles;
    Name_Z m_Name;

    PlayParticlesGame_Z(PlayParticles_Z* i_PlayParticles, Node_Z* i_Node, Agent_Z* i_Agent, Particles_Z* i_Particles, const Name_Z& i_Name)
        : m_PlayParticles(i_PlayParticles)
        , m_Node(i_Node)
        , m_Agent(i_Agent)
        , m_Particles(i_Particles)
        , m_Name(i_Name) { }
};

typedef DynArray_Z<PlayParticlesGame_Z, 16, FALSE> PlayParticlesGame_ZDA;

class ObjectsGame_Z : public ObjectGame_Z {
    friend class ObjectsGameClip_Z;

public:
    virtual ~ObjectsGame_Z() { }

    virtual void Init();
    virtual void Reset();
    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();
    virtual void Update(Float a1);
    virtual void AddGameObject(const ObjectMove_ZHdl& a1);
    virtual void RemoveGameObject(const ObjectMove_ZHdl& a1);
    virtual void UnregistredFromGame(const Agent_ZHdl& a1);

    void AddGamePlayParticles(const PlayParticles_ZHdl& i_PlayParticlesHdl);
    void UpdateSkelCollision();
    S32 GetCollisionSkelMsg(const SphereColBone_ZDA& i_Spheres, const SphereColBone_ZDA& i_VsSpheres, const Mat4x4& i_Matrix, SkelMessage_Z* o_Msgs);
    void UpdateBuffer();
    void UpdateObject(Float i_DeltaTime);

    void SetRunningUpdateFromManager(Bool i_Running) {
        m_IsRunningUpdateFromManager = i_Running;
    }

    static BaseObject_Z* NewObject() { return NewL_Z(125) ObjectsGame_Z; }

protected:
    Bool m_IsRunningUpdate;
    Bool m_IsRunningUpdateFromManager;
    ObjectMoveGame_ZDA m_ObjectMoveGameDA;
    PlayParticlesGame_ZDA m_PlayParticlesGameDA;
    DynPtrArray_Z<Node_Z*, 32> m_NodeDPA;
    DynPtrArray_Z<PlayAnim_Z*, 32> m_PlayAnimsDPA;
    StaticArray_Z<ObjectMove_ZHdl, 128> m_AddedDuringUpdateObjectMoveHdls;
    StaticArray_Z<PlayParticles_ZHdl, 32> m_AddedDuringUpdatePlayParticlesHdls;
    StaticArray_Z<ObjectMove_ZHdl, 128> m_RemovedDuringUpdateObjectMoveHdls;
    StaticArray_Z<PlayParticles_ZHdl, 32> m_RemovedDuringUpdatePlayParticlesHdls;
    DynPtrArray_Z<Skel_Z*, 32> m_CachedSkelPtrs;
    DynPtrArray_Z<Node_Z*, 32> m_CachedNodePtrs;
    DynPtrArray_Z<AnimatedAgent_Z*, 32> m_CachedAnimatedAgentPtrs;
};

class ObjectsGameClip_Z : public ObjectGame_Z {
public:
    virtual ~ObjectsGameClip_Z() { }

    virtual void Init();
    virtual void Update(Float i_DeltaTime);

    void UpdateObjectActivity(World_Z* i_World);

    static BaseObject_Z* NewObject() { return NewL_Z(31) ObjectsGameClip_Z; }

    void SetObjectsGameHdl(ObjectsGame_ZHdl& i_ObjectsGameHdl) {
        m_ObjectsGameHdl = i_ObjectsGameHdl;
    }

private:
    Bool m_UnkBool_0x2c;
    ObjectsGame_ZHdl m_ObjectsGameHdl;
};

#endif // _OBJECTSGAME_Z_H_
