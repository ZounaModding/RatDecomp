#ifndef _IT_BUMPER_H_
#define _IT_BUMPER_H_
#include "GangAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Bumper, LightVolAnimatedAgent_Z, 17)
public:
    IT_Bumper();

    virtual ~IT_Bumper() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<IT_Bumper*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _IT_BUMPER_H_
