#ifndef _IT_LINKOBJ_H_
#define _IT_LINKOBJ_H_
#include "PhysicAgt_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_LinkObj, IT_DYNBASE_G, 17)
public:
    IT_LinkObj();

    virtual ~IT_LinkObj() {}
    virtual void Init();
private:
END_AGENT_CLASS

BEGIN_AGENT_CLASS(IT_LinkHamecon, IT_DYNBASE_G, 29)
public:
    IT_LinkHamecon();

    virtual ~IT_LinkHamecon() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<IT_LinkHamecon*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _IT_LINKOBJ_H_
