#include "Heros_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_REMY, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_REMY::P_REMY() {
}

void P_REMY::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_REMY_R, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_REMY_R::P_REMY_R() {
}

void P_REMY_R::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_EMILE, P_REMY)
END_INIT_AGENT_CLASS

// clang-format on

P_EMILE::P_EMILE() {
}

void P_EMILE::Init() {
    P_REMY::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_CELI_M, P_REMY)
END_INIT_AGENT_CLASS

// clang-format on

P_CELI_M::P_CELI_M() {
}

void P_CELI_M::Init() {
    P_REMY::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_AIEMIL, P_EMILE)
END_INIT_AGENT_CLASS

// clang-format on

P_AIEMIL::P_AIEMIL() {
}

void P_AIEMIL::Init() {
    P_EMILE::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_AITWIT, P_AIEMIL)
END_INIT_AGENT_CLASS

// clang-format on

P_AITWIT::P_AITWIT() {
}

void P_AITWIT::Init() {
    P_AIEMIL::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_AICELI, P_AIEMIL)
END_INIT_AGENT_CLASS

// clang-format on

P_AICELI::P_AICELI() {
}

void P_AICELI::Init() {
    P_AIEMIL::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_REMY_C, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_REMY_C::P_REMY_C() {
}

void P_REMY_C::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_BUT, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_BUT::P_R_BUT() {
}

void P_R_BUT::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_BKD, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_BKD::P_R_BKD() {
}

void P_R_BKD::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_LINGUI, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_LINGUI::P_LINGUI() {
}

void P_LINGUI::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_CAKE, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_L_CAKE::P_L_CAKE() {
}

void P_L_CAKE::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_REMY, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_L_REMY::P_L_REMY() {
}

void P_L_REMY::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_PLAT01, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_PLAT01::P_PLAT01() {
}

void P_PLAT01::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_SOUP01, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_SOUP01::P_SOUP01() {
}

void P_SOUP01::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_WASH, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_L_WASH::P_L_WASH() {
}

void P_L_WASH::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_WASH2, P_L_WASH)
END_INIT_AGENT_CLASS

// clang-format on

P_L_WASH2::P_L_WASH2() {
}

void P_L_WASH2::Init() {
    P_L_WASH::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_CREP, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_L_CREP::P_L_CREP() {
}

void P_L_CREP::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_REMY_S, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_REMY_S::P_REMY_S() {
}

void P_REMY_S::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_REMY_T, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_REMY_T::P_REMY_T() {
}

void P_REMY_T::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_SWMK, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_SWMK::P_R_SWMK() {
}

void P_R_SWMK::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_SWKN, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_SWKN::P_R_SWKN() {
}

void P_R_SWKN::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_FISH, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_FISH::P_FISH() {
}

void P_FISH::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_WCCT, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_WCCT::P_R_WCCT() {
}

void P_R_WCCT::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_L_POTA, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_L_POTA::P_L_POTA() {
}

void P_L_POTA::Init() {
    P_LINGUI::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_SOUP, Player_G)
END_INIT_AGENT_CLASS

// clang-format on

P_R_SOUP::P_R_SOUP() {
}

void P_R_SOUP::Init() {
    Player_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(P_R_COL, P_LINGUI)
END_INIT_AGENT_CLASS

// clang-format on

P_R_COL::P_R_COL() {
}

void P_R_COL::Init() {
    P_LINGUI::Init();
}
