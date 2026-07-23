#ifndef _GHOST_Z_H_
#define _GHOST_Z_H_
#include "Manipulator_Z.h"
#include "Math_Z.h"
#include "Node_ZHdl.h"
#include "PlayAnim_ZHdl.h"

class File_Z;

struct NodeState {
    Float m_Time;
    Vec3f m_Pos;
    Quat m_Rot;
};

class Ghost_Z : public Manipulator_Z {
public:
    Ghost_Z();

    virtual ~Ghost_Z() { }

    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void ResetGhost();
    virtual void UpdateBestGhost();

    virtual void GetPlayerState(Float a1) { }

    virtual void Save(Float a1);
    virtual void Play(Float a1);
    virtual void StartSave(const Node_ZHdl& a1);
    virtual void SaveToFile(File_Z& a1);
    virtual void LoadFromFile(File_Z& a1);

    static BaseObject_Z* NewObject() { return NewL_Z(99) Ghost_Z; }

private:
    S32 m_CurKey;
    Node_ZHdl m_SaveNodeHdl;
    Node_ZHdl m_PlayNodeHdl;
    U8 m_Flag;
    PlayAnim_ZHdl m_PlayAnimHdl;
    DynArray_Z<NodeState, 64> m_CurNodeStatesDA;
    DynArray_Z<NodeState, 64> m_BestNodeStatesDA;
};

#endif // _GHOST_Z_H_
