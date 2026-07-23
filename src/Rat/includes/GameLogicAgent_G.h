#ifndef _GAMELOGICAGENT_G_H_
#define _GAMELOGICAGENT_G_H_
#include "LogicAgent_G.h"
// clang-format off

BEGIN_AGENT_CLASS(GameLogicAgent_G, LogicAgent_G, 18)
public:
    GameLogicAgent_G();

    virtual ~GameLogicAgent_G() {}
    virtual void Init();

    DECL_BHV(ActivateGame);
    DECL_BHV(GameReseted);
    DECL_BHV(StopPauseGameBhv);
private:
END_AGENT_CLASS

// clang-format on
#endif // _GAMELOGICAGENT_G_H_
