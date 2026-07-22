#ifndef _FRIENDS_G_H_
#define _FRIENDS_G_H_
#include "Creatures_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(Friends_G, Creatures_G, 14)
public:
    Friends_G();

    virtual ~Friends_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckContextualSound);

private:
    static DynPtrArray_Z<Friends_G*> instances;
    // TODO: fields - see decomp_dump/types.h `struct Friends_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _FRIENDS_G_H_
