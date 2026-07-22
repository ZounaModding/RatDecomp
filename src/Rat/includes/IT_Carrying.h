#ifndef _IT_CARRYING_H_
#define _IT_CARRYING_H_
#include "Throwable_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_Carrying, Throwable_Z, 16)
public:
    IT_Carrying();

    virtual ~IT_Carrying() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckRespawn);
    DECL_BHV(EnterTeleportZone);

private:
    static DynPtrArray_Z<IT_Carrying*> instances;
    // TODO: fields - see decomp_dump/types.h `struct IT_Carrying` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_CARRYING_H_
