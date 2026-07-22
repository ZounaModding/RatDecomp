#include "Names.h"
#include "Creatures_G.h"
#include "ABC_Message_Game.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(Creatures_G, BaseAgent_G)
    ADD_BEHAVIOR(ParticlesEfx)
    ADD_BEHAVIOR(ParticlesEfxPause)
    ADD_BEHAVIOR(ParticlesEfxReset)
    ADD_BEHAVIOR(AnimChange)
    ADD_BEHAVIOR(Contact)
    ADD_BEHAVIOR(NoteTrackSample)
    ADD_BEHAVIOR(CheckRtcScriptMessage)
    ADD_BEHAVIOR(EnterTeleportZone)
    ADD_BEHAVIOR(ExecuteTeleportZone)
    ADD_BEHAVIOR(FX_WeaponBlur)
    ADD_BEHAVIOR(CheckFootOnGround)
    ADD_BEHAVIOR(CheckFootPrintOnGround)
    ADD_BEHAVIOR(CheckStartMission)
END_INIT_AGENT_CLASS

// clang-format on

Creatures_G::Creatures_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (Creatures_G::Creatures_G)
}

void Creatures_G::Init() {
    BaseAgent_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (Creatures_G::Init)
}

static DynPtrArray_Z<Creatures_G*> instances;

void Creatures_G::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void Creatures_G::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

// clang-format off

BEHAVIOR(Creatures_G,ParticlesEfx)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_particles_efx)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::ParticlesEfx)
END_BEHAVIOR

BEHAVIOR(Creatures_G,ParticlesEfxPause)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_particles_efx_pause)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::ParticlesEfxPause)
END_BEHAVIOR

BEHAVIOR(Creatures_G,ParticlesEfxReset)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_particles_efx_reset)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::ParticlesEfxReset)
END_BEHAVIOR

BEHAVIOR(Creatures_G,AnimChange)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_anim_change)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::AnimChange)
END_BEHAVIOR

BEHAVIOR(Creatures_G,Contact)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_sequenced_collide)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::Contact)
END_BEHAVIOR

BEHAVIOR(Creatures_G,NoteTrackSample)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_note_track_sample)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::NoteTrackSample)
END_BEHAVIOR

BEHAVIOR(Creatures_G,CheckRtcScriptMessage)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_rtc_script_interp_command)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::CheckRtcScriptMessage)
END_BEHAVIOR

BEHAVIOR(Creatures_G,EnterTeleportZone)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_enter_teleport)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::EnterTeleportZone)
END_BEHAVIOR

BEHAVIOR(Creatures_G,ExecuteTeleportZone)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_execute_teleport)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::ExecuteTeleportZone)
END_BEHAVIOR

BEHAVIOR(Creatures_G,FX_WeaponBlur)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_fx_trace_b_start)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::FX_WeaponBlur)
END_BEHAVIOR

BEHAVIOR(Creatures_G,CheckFootOnGround)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_foot_on_ground_bone)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::CheckFootOnGround)
END_BEHAVIOR

BEHAVIOR(Creatures_G,CheckFootPrintOnGround)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_footprint_on_ground)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::CheckFootPrintOnGround)
END_BEHAVIOR

BEHAVIOR(Creatures_G,CheckStartMission)
    CATEGORY(cat_default)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_start_mission)
    ACTION
        ;// TODO: implement - see decomp_dump/Rat/\*\.cpp\.ghidra\.c \(Creatures_G::CheckStartMission)
END_BEHAVIOR

// clang-format on
