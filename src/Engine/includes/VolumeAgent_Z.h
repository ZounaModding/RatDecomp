#ifndef _VOLUMEAGENT_Z_H_
#define _VOLUMEAGENT_Z_H_
#include "ABC_AgentMacros.h"
#include "Agent_Z.h"
#include "AnimFrame_ZHdl.h"

// clang-format off

BEGIN_AGENT_CLASS(VolumeAgent_Z, Agent_Z, 11)
public:
    virtual ~VolumeAgent_Z() { }
    virtual void UseNode(Node_ZHdl& i_NodeHdl);

    DECL_BHV(VolumeAnimReset);

    virtual void StartAnims();
    virtual void ResetAnims();
    virtual void Entering(const Agent_ZHdl& i_AgentHdl);
    virtual void Inside(const Agent_ZHdl& i_AgentHdl) { } 
    virtual void Exiting(const Agent_ZHdl& i_AgentHdl) { } 
    virtual void Staying(const Agent_ZHdl& i_AgentHdl) { } 

    Float GetFloatParam(S32 i_Index);
    Node_ZHdl& GetNodeParam(S32 i_Index) const;
    S32 GetNbNodeAnim() const;
    AnimFrame_ZHdl& GetNodeAnim(S32 i_Index) const;
END_AGENT_CLASS

// clang-format on

#endif // _VOLUMEAGENT_Z_H_
