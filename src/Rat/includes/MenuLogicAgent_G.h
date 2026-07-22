#ifndef _MENULOGICAGENT_G_H_
#define _MENULOGICAGENT_G_H_
#include "LogicAgent_G.h"
// clang-format off

BEGIN_AGENT_CLASS(MenuLogicAgent_G, LogicAgent_G, 23)
public:
    MenuLogicAgent_G();

    virtual ~MenuLogicAgent_G() {}
    virtual void Init();

    DECL_BHV(ActivateGame);
    DECL_BHV(ReallyStartMenu);

private:
    // TODO: fields - see decomp_dump/types.h `struct MenuLogicAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _MENULOGICAGENT_G_H_
