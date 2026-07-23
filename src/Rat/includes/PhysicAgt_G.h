#ifndef _PHYSICAGT_G_H_
#define _PHYSICAGT_G_H_
#include "BaseAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_DYNBASE_G, BaseAgent_G, 20)
public:
    IT_DYNBASE_G();

    virtual ~IT_DYNBASE_G() {}
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<IT_DYNBASE_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _PHYSICAGT_G_H_
