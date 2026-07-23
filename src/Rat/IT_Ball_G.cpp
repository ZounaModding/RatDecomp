#include "IT_Ball_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_BALL, IT_DYNBASE_G)
END_INIT_AGENT_CLASS

// clang-format on

IT_BALL::IT_BALL() {
}

void IT_BALL::Init() {
    IT_DYNBASE_G::Init();
}

DynPtrArray_Z<IT_BALL*> IT_BALL::instances;

void IT_BALL::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_BALL::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}
