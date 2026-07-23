#include "RtcScriptAgent_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(RtcScriptAgent_G, RtcAgent_Z)
    ADD_BEHAVIOR(CheckNextCommand)
    ADD_BEHAVIOR(CheckSkipRtc)
    ADD_BEHAVIOR(CheckSkipText)
    ADD_BEHAVIOR(CheckEndOfGoto)
    ADD_BEHAVIOR(RtcParticlesEfx)
    ADD_BEHAVIOR(RtcWaitPreLoad)
END_INIT_AGENT_CLASS

// clang-format on

RtcScriptAgent_G::RtcScriptAgent_G() {
}

void RtcScriptAgent_G::Init() {
    RtcAgent_Z::Init();
}

// clang-format off

BEHAVIOR(RtcScriptAgent_G,CheckNextCommand)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_script_interp_command)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(RtcScriptAgent_G,CheckSkipRtc)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_script_check_skip_rtc)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(RtcScriptAgent_G,CheckSkipText)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_script_check_skip_text)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(RtcScriptAgent_G,CheckEndOfGoto)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_script_check_end_goto)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(RtcScriptAgent_G,RtcParticlesEfx)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_particles_efx)
    ACTION
        ;
END_BEHAVIOR

BEHAVIOR(RtcScriptAgent_G,RtcWaitPreLoad)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        result_FALSE /* TODO: message not found in ABC_Message_Engine.h / ABC_Message_Game.h cross-refs - check decomp_dump/Rat/*.cpp.ghidra.c (RtcScriptAgent_G::RtcWaitPreLoad) for `ABC_Agent::Message(this, N)` */
    ACTION
        ;
END_BEHAVIOR

// clang-format on
