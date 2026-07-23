#include "IT_LinkObj.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_LinkObj, IT_DYNBASE_G)
END_INIT_AGENT_CLASS

// clang-format on

IT_LinkObj::IT_LinkObj() {
}

void IT_LinkObj::Init() {
    IT_DYNBASE_G::Init();
}

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_LinkHamecon, IT_DYNBASE_G)
END_INIT_AGENT_CLASS

// clang-format on

IT_LinkHamecon::IT_LinkHamecon() {
}

void IT_LinkHamecon::Init() {
    IT_DYNBASE_G::Init();
}

DynPtrArray_Z<IT_LinkHamecon*> IT_LinkHamecon::instances;

void IT_LinkHamecon::AddToStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            return;
        }
    }
    instances.Add(this);
}

void IT_LinkHamecon::RemoveFromStaticList() {
    for (S32 i = 0; i < instances.GetSize(); i++) {
        if (instances[i] == this) {
            instances.Remove(i);
            break;
        }
    }
    instances.Minimize();
}
