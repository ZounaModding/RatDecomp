#ifndef _CREATURES_G_H_
#define _CREATURES_G_H_
#include "BaseAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(Creatures_G, BaseAgent_G, 34)
public:
    Creatures_G();

    virtual ~Creatures_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(ParticlesEfx);
    DECL_BHV(ParticlesEfxPause);
    DECL_BHV(ParticlesEfxReset);
    DECL_BHV(AnimChange);
    DECL_BHV(Contact);
    DECL_BHV(NoteTrackSample);
    DECL_BHV(CheckRtcScriptMessage);
    DECL_BHV(EnterTeleportZone);
    DECL_BHV(ExecuteTeleportZone);
    DECL_BHV(FX_WeaponBlur);
    DECL_BHV(CheckFootOnGround);
    DECL_BHV(CheckFootPrintOnGround);
    DECL_BHV(CheckStartMission);

private:
    static DynPtrArray_Z<Creatures_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct Creatures_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _CREATURES_G_H_
