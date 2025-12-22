#include "Throwable_Z.h"
#include "ObjectThrow_Z.h"
#include "ObjectThrow_ZHdl.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(Throwable_Z, MovingAgent_Z)
    ADD_BEHAVIOR(CheckThrown)
END_INIT_AGENT_CLASS

Throwable_Z::Throwable_Z() {
    DisableFlag(FL_AGENT_UNK_0x1);
    EnableFlag(FL_AGENT_UNK_0x4);
}

void Throwable_Z::Init() {
    MovingAgent_Z::Init();
    ResetCollisionMessages();
    ActivateCategory(cat_default);
    if (HasFlag(FL_AGENT_UNK_0x4)) {
        SetObjectMove(gData.ClassMgr->NewObject("ObjectThrow_Z"));
    }
}

BEHAVIOR(Throwable_Z,CheckThrown)
    CATEGORY(cat_default)
    CATEGORY(cat_check_thrown)
    CATEGORY_PRIVATE(cat_thrown)
BEGIN_BEHAVIOR
    START_COND
        Message(msg_thrown_begin)
    START_ACTION
        ThrowBegin();
        EnableFlag(FL_AGENT_UNK_0x10000)
    FINISH_COND
        Message(msg_thrown_end)
    FINISH_ACTION
        ThrowEnd();
        DisableFlag(FL_AGENT_UNK_0x10000)
    CONDITION
        !Message(msg_thrown_begin) && (
            (Message(msg_lod_collide) || Message(msg_decor_collide)) &&
            GetObjectThrow()->IsOwnerCollisionRestored()
        )
    ACTION
        ResetCollisionMessages();
END_BEHAVIOR

// clang-format on
