#include "V_Raft.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(V_RAFT, IT_DYNBASE_G)
END_INIT_AGENT_CLASS

BEGIN_INIT_AGENT_CLASS(V_RAFTM, V_RAFT)
END_INIT_AGENT_CLASS

BEGIN_INIT_AGENT_CLASS(V_Book, V_RAFT)
END_INIT_AGENT_CLASS

// clang-format on

V_RAFT::V_RAFT() {
}

DynPtrArray_Z<V_RAFT*> V_RAFT::instances;

void V_RAFT::AddToStaticList() {
    instances.Add(this);
}

void V_RAFT::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}

void V_RAFT::Init() {
    IT_DYNBASE_G::Init();
}

V_RAFTM::V_RAFTM() {
}

void V_RAFTM::Init() {
    V_RAFT::Init();
}

V_Book::V_Book() {
}

void V_Book::Init() {
    V_RAFT::Init();
}
