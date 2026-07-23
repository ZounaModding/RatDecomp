#ifndef _COLLECTAGENT_G_H_
#define _COLLECTAGENT_G_H_
#include "ObjectAgent_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_COLLECT_G, ObjectAgent_G, 15)
public:
    IT_COLLECT_G();

    virtual ~IT_COLLECT_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<IT_COLLECT_G*> STC_Instances;
END_AGENT_CLASS

// clang-format on
#endif // _COLLECTAGENT_G_H_
