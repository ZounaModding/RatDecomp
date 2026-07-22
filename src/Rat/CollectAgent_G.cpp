#include "CollectAgent_G.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(IT_COLLECT_G, ObjectAgent_G)
END_INIT_AGENT_CLASS

// clang-format on

IT_COLLECT_G::IT_COLLECT_G() {
    // TODO: field initialization - see decomp_dump/Rat/*.cpp.ghidra.c (IT_COLLECT_G::IT_COLLECT_G)
}

void IT_COLLECT_G::Init() {
    ObjectAgent_G::Init();
    // TODO: ActivateBhv(...) calls - see decomp_dump/Rat/*.cpp.ghidra.c (IT_COLLECT_G::Init)
}

DynPtrArray_Z<IT_COLLECT_G*> IT_COLLECT_G::STC_Instances;

void IT_COLLECT_G::AddToStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            return;
        }
    }
    STC_Instances.Add(this);
}

void IT_COLLECT_G::RemoveFromStaticList() {
    for (S32 i = 0; i < STC_Instances.GetSize(); i++) {
        if (STC_Instances[i] == this) {
            STC_Instances.Remove(i);
            if (STC_Instances.GetSize() == 0)
                STC_Instances.Minimize();
            return;
        }
    }
}
