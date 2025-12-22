#ifndef _SKINAGENT_Z_H_
#define _SKINAGENT_Z_H_
#include "AnimatedAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(SkinAgent_Z, AnimatedAgent_Z, 7)
public:
    SkinAgent_Z();
    virtual ~SkinAgent_Z() { }

    DECL_BHV(Act)
END_AGENT_CLASS

// clang-format on
#endif // _SKINAGENT_Z_H_
