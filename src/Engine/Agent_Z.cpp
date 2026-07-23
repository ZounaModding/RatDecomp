#include "Agent_Z.h"

BEGIN_INIT_AGENT_CLASS(Agent_Z, ABC_Agent)
END_INIT_AGENT_CLASS

Agent_Z::Agent_Z() {
    m_AgentFlags = 0;
    EnableFlag(FL_AGENT_DEFAULT);
    m_ClassSoundIndex = -1;
    m_MaxSqrDistActive = User_DefaultAgentMaxSqrDistActive;
    m_MinSqrDistActive = User_DefaultAgentMinSqrDistActive;
}

void Agent_Z::Init() { }

void Agent_Z::Reset() { }

void Agent_Z::UseNode(Node_ZHdl& a1) { }

void Agent_Z::RegisterWithGame(Game_ZHdl& a1) { }

void Agent_Z::UnRegisterFromGame(Game_ZHdl& a1) { }
