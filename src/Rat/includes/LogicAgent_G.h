#ifndef _LOGICAGENT_G_H_
#define _LOGICAGENT_G_H_
#include "Agent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(LogicAgent_G, Agent_Z, 27)
public:
    LogicAgent_G();

    virtual ~LogicAgent_G() {}
    virtual void Init();

    DECL_BHV(ActivateGame);
    DECL_BHV(BlockedFaderBhv);

private:
    // TODO: fields - see decomp_dump/types.h `struct LogicAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _LOGICAGENT_G_H_
