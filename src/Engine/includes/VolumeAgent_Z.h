#ifndef _VOLUMEAGENT_Z_H_
#define _VOLUMEAGENT_Z_H_
#include "ABC_AgentMacros.h"
#include "Agent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(VolumeAgent_Z, Agent_Z, 11)
public:
    virtual ~VolumeAgent_Z() { }
    virtual void UseNode(Node_ZHdl& a1);

    DECL_BHV(VolumeAnimReset)

    virtual void StartAnims();
    virtual void ResetAnims();
    virtual void Entering(const Agent_ZHdl& a1);
    virtual void Inside(const Agent_ZHdl& a1);
    virtual void Exiting(const Agent_ZHdl& a1);
    virtual void Staying(const Agent_ZHdl& a1);
END_AGENT_CLASS

// clang-format on
#endif // _VOLUMEAGENT_Z_H_
