#include "Agent_Z.h"
#include "ClassManager_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "SoundManager_Z.h"

BEGIN_INIT_AGENT_CLASS(Agent_Z, ABC_Agent)
END_INIT_AGENT_CLASS

Agent_Z::Agent_Z() {
    m_TypeFlags = 0;
    EnableFlag(FL_AGENT_DEFAULT);
    m_ClassSoundIndex = -1;
    m_MaxSqrDistActive = User_DefaultAgentMaxSqrDistActive;
    m_MinSqrDistActive = User_DefaultAgentMinSqrDistActive;
}

void Agent_Z::Init() {
    ActivateCategory(cat_default);
}

void Agent_Z::Reset() {
    FlushAllMessages(TRUE);
    ResetAllBehaviorsExceptCategories(m_NonResetableCategories);
    DisableFlag(FL_AGENT_THROWN);
    ActivateCategory(cat_default);
}

void Agent_Z::UseNode(Node_ZHdl& i_NodeHdl) {
    m_NodeHdl = i_NodeHdl;
    i_NodeHdl->GetAgent() = GetHandle();

    if (HasFlag(FL_AGENT_NEED_SOUND)) {
        Object_ZHdl l_ObjectHdl;
        Object_Z* l_Object = i_NodeHdl->GetObjectA();

        if (l_Object) {
            l_ObjectHdl = l_Object->GetHandle();
        }
        else {
            l_ObjectHdl = HANDLE_NULL;
        }

        gData.SoundMgr->GetClassID(l_ObjectHdl->GetName(), gData.ClassMgr->GetHandleName(l_ObjectHdl), m_ClassSoundIndex, FALSE);
    }
}

void Agent_Z::RegisterWithGame(Game_ZHdl& i_GameHdl) {
    m_GameHdl = i_GameHdl;
}

void Agent_Z::UnRegisterFromGame(Game_ZHdl& a1) { }
