#ifndef _V_RAFT_H_
#define _V_RAFT_H_
#include "PhysicAgt_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(V_RAFT, IT_DYNBASE_G, 16)
public:
    V_RAFT();

    virtual ~V_RAFT() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

private:
    static DynPtrArray_Z<V_RAFT*> instances;
END_AGENT_CLASS

BEGIN_AGENT_CLASS(V_Book, V_RAFT, 65)
public:
    V_Book();

    virtual ~V_Book() {}
    virtual void Init();
private:
END_AGENT_CLASS

BEGIN_AGENT_CLASS(V_RAFTM, V_RAFT, 93)
public:
    V_RAFTM();

    virtual ~V_RAFTM() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _V_RAFT_H_
