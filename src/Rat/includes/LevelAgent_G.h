#ifndef _LEVELAGENT_G_H_
#define _LEVELAGENT_G_H_
#include "Agent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(LevelAgent_G, Agent_Z, 28)
public:
    LevelAgent_G();

    virtual ~LevelAgent_G() {}
    virtual void Init();

    DECL_BHV(BhvToLevel);
    DECL_BHV(BhvChangeLevel);
    DECL_BHV(BhvToMenu);

private:
    

public:
    void InitClass();
END_AGENT_CLASS

// clang-format on
#endif // _LEVELAGENT_G_H_
