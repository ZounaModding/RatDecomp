#ifndef _PARTICLESAGENT_Z_H_
#define _PARTICLESAGENT_Z_H_
#include "Agent_Z.h"
#include "PlayParticles_ZHdl.h"
#include "Skel_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(ParticlesAgent_Z, Agent_Z, 12)
public:
    ParticlesAgent_Z();
    virtual void Init();                                /* 0x08 */
    virtual ~ParticlesAgent_Z() { }                     /* 0x0C */
    virtual Bool MarkHandles();                         /* 0x10 */
    virtual void Suspend();                             /* 0x24 */
    virtual void Restore();                             /* 0x28 */
    virtual void Reset();                               /* 0x2C */
    virtual void UseNode(Node_ZHdl& a1);                /* 0x30 */
    virtual void RegisterWithGame(Game_ZHdl& a1);       /* 0x34 */
    virtual void UnRegisterFromGame(Game_ZHdl& a1);     /* 0x38 */
    virtual void DoFollow();                            /* 0x3C */

    void Pause();
    void Resume();
    void Replay();

private:
    PlayParticles_ZHdl m_PlayParticlesHdl;
    Node_ZHdl m_FollowNodeHdl;
    Skel_ZHdl m_FollowSkelHdl;
    S32 m_FollowBoneId;
END_AGENT_CLASS

// clang-format on
#endif // _PARTICLESAGENT_Z_H_
