#include "LodAgent_Z.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(LodAgent_Z, Throwable_Z)
END_INIT_AGENT_CLASS

// clang-format on

LodAgent_Z::LodAgent_Z() {
    DisableFlag(FL_AGENT_UNK_0x4);
    EnableFlag(FL_AGENT_UNK_0x8);
    m_MaxSqrDistActive = -1.0f;
    m_MinSqrDistActive = -1.0f;
}
