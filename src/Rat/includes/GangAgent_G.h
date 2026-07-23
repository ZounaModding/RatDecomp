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
private:
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
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RepulseAnimatedAgent_G, LightVolAnimatedAgent_Z, 54)
public:
    RepulseAnimatedAgent_G();

    virtual ~RepulseAnimatedAgent_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<RepulseAnimatedAgent_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _GANGAGENT_G_H_
