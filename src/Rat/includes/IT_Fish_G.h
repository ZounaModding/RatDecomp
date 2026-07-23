#ifndef _IT_FISH_G_H_
#define _IT_FISH_G_H_
#include "MovingAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_FISH_G, MovingAgent_Z, 17)
public:
    IT_FISH_G();

    virtual ~IT_FISH_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<IT_FISH_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _IT_FISH_G_H_
