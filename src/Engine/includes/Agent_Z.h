#ifndef _AGENT_Z_H_
#define _AGENT_Z_H_
#include "ABC_Agent.h"
#include "Node_ZHdl.h"
#include "Game_ZHdl.h"
#include "Agent_ZHdl.h"
#include "ABC_AgentMacros.h"
// clang-format off

BEGIN_AGENT_CLASS(Agent_Z, ABC_Agent, 13)
public:
    Agent_Z();
    virtual void Init();                                              /* 0x08 */
    virtual ~Agent_Z() {}                                             /* 0x0C */
    virtual void Reset();                                             /* 0x20 */
    virtual void UseNode(Node_ZHdl& a1);                              /* 0x24 */
    virtual void RegisterWithGame(Game_ZHdl& a1);                     /* 0x28 */
    virtual void UnRegisterFromGame(Game_ZHdl& a1);                   /* 0x2C */
    virtual void AuthorizeInteractionWith(Agent_ZHdl& a1, U64 a2);    /* 0x30 */
    virtual void GetCollisionMessageIn(int& a1);                      /* 0x34 */
    virtual void GetCollisionMessageOut(int& a1);                     /* 0x38 */
    virtual void GetCollisionMessageContactActive(int& a1);           /* 0x3C */
    virtual void GetCollisionMessageContactPassive(int& a1);          /* 0x40 */
    virtual void SetCollisionMessageContactActive(int a1);            /* 0x44 */
    virtual void SetCollisionMessageContactPassive(int a1);           /* 0x48 */
    virtual void GetCollisionMessageAction(int& a1);                  /* 0x4C */
    virtual void SetCollisionMessageAction(int a1);                   /* 0x50 */
    virtual void RemoveObjectMove(S32 a1);                            /* 0x54 */
    virtual void AddObjectMove(S32 a1);                               /* 0x58 */
    virtual void GetDisplayValue(const Name_Z& a1, const Name_Z& a2); /* 0x5C */

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
