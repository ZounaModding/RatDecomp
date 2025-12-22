#include "Agent_Z.h"

BEGIN_INIT_AGENT_CLASS(Agent_Z, ABC_Agent)
END_INIT_AGENT_CLASS

Agent_Z::Agent_Z() {
    m_AgentFlags = 0;
    EnableFlag(FL_AGENT_UNK_0x200000);
    m_ClassSoundIndex = -1;
    m_MaxSqrDistActive = User_DefaultAgentMaxSqrDistActive;
    m_MinSqrDistActive = User_DefaultAgentMinSqrDistActive;
}
