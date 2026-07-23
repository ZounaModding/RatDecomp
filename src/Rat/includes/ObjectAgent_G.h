#ifndef _OBJECTAGENT_G_H_
#define _OBJECTAGENT_G_H_
#include "BaseAgent_G.h"
#include "Agent_Z.h"
#include "AnimatedAgent_Z.h"
#include "DynPtrArray_Z.h"

// clang-format off

BEGIN_AGENT_CLASS(ObjectAgent_G, BaseAgent_G, 17)
public:
    ObjectAgent_G();

    virtual ~ObjectAgent_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

BEGIN_AGENT_CLASS(IT_BREAKBASE_G, Agent_Z, 66)
public:
    IT_BREAKBASE_G();

    virtual ~IT_BREAKBASE_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(FireInside);
    DECL_BHV(GenerateObject);
private:
    static DynPtrArray_Z<IT_BREAKBASE_G*> instances;
END_AGENT_CLASS

BEGIN_AGENT_CLASS(IT_TIMED_PLATFORM_G, AnimatedAgent_Z, 108)
public:
    IT_TIMED_PLATFORM_G();

    virtual ~IT_TIMED_PLATFORM_G() {}
    virtual void Init();

    DECL_BHV(CheckPlayer);
    DECL_BHV(CheckRespawn);

private:
END_AGENT_CLASS

BEGIN_AGENT_CLASS(PLTF02_GROUP, AnimatedAgent_Z, 116)
public:
    PLTF02_GROUP();

    virtual ~PLTF02_GROUP() {}
    virtual void Init();

    static Float animDefaultDuration;
private:
END_AGENT_CLASS

// clang-format on
#endif // _OBJECTAGENT_G_H_
