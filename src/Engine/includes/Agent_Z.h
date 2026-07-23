#ifndef _AGENT_Z_H_
#define _AGENT_Z_H_
#include "ABC_Agent.h"
#include "ABC_Message_Engine.h"
#include "Node_ZHdl.h"
#include "Game_ZHdl.h"
#include "Agent_ZHdl.h"
#include "ABC_AgentMacros.h"
// clang-format off

BEGIN_AGENT_CLASS(Agent_Z, ABC_Agent, 13)
public:
    Agent_Z();
    virtual void Init();
    virtual ~Agent_Z() {}
    virtual void Reset();
    virtual void UseNode(Node_ZHdl& a1);
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);
    virtual void AuthorizeInteractionWith(Agent_ZHdl& a1, U64 a2) {}
    virtual Bool GetCollisionMessageIn(abc_message& o_Msg) { return FALSE; }
    virtual Bool GetCollisionMessageOut(abc_message& o_Msg) { return FALSE; }
    virtual Bool GetCollisionMessageContactActive(abc_message& o_Msg) { return FALSE; }
    virtual Bool GetCollisionMessageContactPassive(abc_message& o_Msg) { return FALSE; }
    virtual void SetCollisionMessageContactActive(abc_message i_Msg) {}
    virtual void SetCollisionMessageContactPassive(abc_message i_Msg) {}
    virtual Bool GetCollisionMessageAction(abc_message& o_Msg) { return FALSE; }
    virtual void SetCollisionMessageAction(abc_message i_Msg) {}
    virtual void RemoveObjectMove(S32 a1) {}
    virtual void AddObjectMove(S32 a1) {}
    virtual void GetDisplayValue(const Name_Z& a1, const Name_Z& a2) {}

protected:
    Float m_MaxSqrDistActive;
    Float m_MinSqrDistActive;
    CategoryArray m_NonResetableCategories;
    S32 m_ClassSoundIndex; // $VIOLET: Jimmy DWARF
    Node_ZHdl m_NodeHdl;
    Game_ZHdl m_GameHdl;
    U32 m_AgentFlags;
END_AGENT_CLASS

// clang-format on
#endif
