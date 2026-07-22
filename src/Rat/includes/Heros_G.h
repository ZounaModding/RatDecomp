#ifndef _HEROS_G_H_
#define _HEROS_G_H_
#include "Player_G.h"
// clang-format off

BEGIN_AGENT_CLASS(P_REMY, Player_G, 25)
public:
    P_REMY();

    virtual ~P_REMY() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_REMY` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_REMY_R, Player_G, 40)
public:
    P_REMY_R();

    virtual ~P_REMY_R() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_REMY_R` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_EMILE, P_REMY, 55)
public:
    P_EMILE();

    virtual ~P_EMILE() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_EMILE` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_CELI_M, P_REMY, 60)
public:
    P_CELI_M();

    virtual ~P_CELI_M() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_CELI_M` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_AIEMIL, P_EMILE, 65)
public:
    P_AIEMIL();

    virtual ~P_AIEMIL() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_AIEMIL` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_AITWIT, P_AIEMIL, 70)
public:
    P_AITWIT();

    virtual ~P_AITWIT() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_AITWIT` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_AICELI, P_AIEMIL, 75)
public:
    P_AICELI();

    virtual ~P_AICELI() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_AICELI` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_REMY_C, Player_G, 80)
public:
    P_REMY_C();

    virtual ~P_REMY_C() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_REMY_C` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_BUT, Player_G, 94)
public:
    P_R_BUT();

    virtual ~P_R_BUT() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_BUT` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_BKD, Player_G, 98)
public:
    P_R_BKD();

    virtual ~P_R_BKD() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_BKD` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_REMY_S, Player_G, 104)
public:
    P_REMY_S();

    virtual ~P_REMY_S() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_REMY_S` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_REMY_T, Player_G, 108)
public:
    P_REMY_T();

    virtual ~P_REMY_T() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_REMY_T` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_SWMK, Player_G, 112)
public:
    P_R_SWMK();

    virtual ~P_R_SWMK() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_SWMK` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_SWKN, Player_G, 116)
public:
    P_R_SWKN();

    virtual ~P_R_SWKN() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_SWKN` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_LINGUI, Player_G, 121)
public:
    P_LINGUI();

    virtual ~P_LINGUI() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_LINGUI` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_CAKE, P_LINGUI, 128)
public:
    P_L_CAKE();

    virtual ~P_L_CAKE() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_CAKE` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_REMY, P_LINGUI, 134)
public:
    P_L_REMY();

    virtual ~P_L_REMY() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_REMY` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_PLAT01, P_LINGUI, 140)
public:
    P_PLAT01();

    virtual ~P_PLAT01() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_PLAT01` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_SOUP01, P_LINGUI, 145)
public:
    P_SOUP01();

    virtual ~P_SOUP01() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_SOUP01` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_WASH, P_LINGUI, 151)
public:
    P_L_WASH();

    virtual ~P_L_WASH() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_WASH` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_WASH2, P_L_WASH, 156)
public:
    P_L_WASH2();

    virtual ~P_L_WASH2() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_WASH2` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_CREP, P_LINGUI, 161)
public:
    P_L_CREP();

    virtual ~P_L_CREP() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_CREP` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_L_POTA, P_LINGUI, 166)
public:
    P_L_POTA();

    virtual ~P_L_POTA() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_L_POTA` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_SOUP, Player_G, 172)
public:
    P_R_SOUP();

    virtual ~P_R_SOUP() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_SOUP` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_COL, P_LINGUI, 178)
public:
    P_R_COL();

    virtual ~P_R_COL() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_COL` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_FISH, Player_G, 184)
public:
    P_FISH();

    virtual ~P_FISH() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_FISH` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(P_R_WCCT, Player_G, 204)
public:
    P_R_WCCT();

    virtual ~P_R_WCCT() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct P_R_WCCT` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _HEROS_G_H_
