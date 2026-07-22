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

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct IT_LinkObj` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(IT_LinkHamecon, IT_DYNBASE_G, 29)
public:
    IT_LinkHamecon();

    virtual ~IT_LinkHamecon() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    static DynPtrArray_Z<IT_LinkHamecon*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_LinkHamecon` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_LINKOBJ_H_
