#include "EvenementVolAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(EvenementVolAgent_G, VolumeDelayedStart_G)
    ADD_BEHAVIOR(BhvCheckActionDansVolume)
    ADD_BEHAVIOR(BhvWaitExplosion)
END_INIT_AGENT_CLASS

// clang-format on

EvenementVolAgent_G::EvenementVolAgent_G() {
}

void EvenementVolAgent_G::Init() {
    VolumeDelayedStart_G::Init();
}

// clang-format off

BEHAVIOR(EvenementVolAgent_G,BhvCheckActionDansVolume)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_action_in_event_volume)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(EvenementVolAgent_G,BhvWaitExplosion)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_explosion_event_volume)
    ACTION
        ;
END_BEHAVIOR

// clang-format on
