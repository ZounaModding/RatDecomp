#ifndef _GANGAGENT_G_H_
#define _GANGAGENT_G_H_
#include "AnimatedAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(LightVolAnimatedAgent_Z, AnimatedAgent_Z, 13)
public:
    LightVolAnimatedAgent_Z();

    virtual ~LightVolAnimatedAgent_Z() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct LightVolAnimatedAgent_Z` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(GangAgent_G, LightVolAnimatedAgent_Z, 24)
public:
    GangAgent_G();

    virtual ~GangAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckPlayerDist);

private:
    static DynPtrArray_Z<GangAgent_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct GangAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RepulseAnimatedAgent_G, LightVolAnimatedAgent_Z, 54)
public:
    RepulseAnimatedAgent_G();

    virtual ~RepulseAnimatedAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<RepulseAnimatedAgent_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct RepulseAnimatedAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _GANGAGENT_G_H_
