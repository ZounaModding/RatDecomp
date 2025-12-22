#ifndef _ANIMATEDMSGAGENT_Z_H_
#define _ANIMATEDMSGAGENT_Z_H_
#include "Agent_Z.h"
// clang-format off

class RegMessage_Z;

BEGIN_AGENT_CLASS(AnimatedMsgAgent_Z, Agent_Z, 7)
public:
    virtual ~AnimatedMsgAgent_Z() { }

    RegMessage_Z& GetMessage();
END_AGENT_CLASS

// clang-format on
#endif // _ANIMATEDMSGAGENT_Z_H_
