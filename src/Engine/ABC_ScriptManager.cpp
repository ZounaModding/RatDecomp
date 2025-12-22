#include "ABC_ScriptManager.h"
#include "LodDynAgent_Z.h"
#include "Program_Z.h"
#include "CameraAgent_Z.h"
#include "SkinAgent_Z.h"
#include "StreamAgent_Z.h"
#include "LodAgent_Z.h"
#include "Breakable_Z.h"
#include "RtcAgent_Z.h"
#include "SubLevelVolumeAgent_Z.h"
#include "ParticlesAgent_Z.h"

void InitEngineScriptClasses() {
    INIT_AGENT_CLASS(CameraAgent_Z)
    INIT_AGENT_CLASS(Agent_Z)
    INIT_AGENT_CLASS(StreamAgent_Z)
    INIT_AGENT_CLASS(AnimatedMsgAgent_Z)
    INIT_AGENT_CLASS(AnimatedAgent_Z)
    INIT_AGENT_CLASS(LodDynAgent_Z)
    INIT_AGENT_CLASS(SkinAgent_Z)
    INIT_AGENT_CLASS(MovingAgent_Z)
    INIT_AGENT_CLASS(Throwable_Z)
    INIT_AGENT_CLASS(LodAgent_Z)
    INIT_AGENT_CLASS(Breakable_Z)
    INIT_AGENT_CLASS(RtcAgent_Z)
    INIT_AGENT_CLASS(VolumeAgent_Z)
    INIT_AGENT_CLASS(SubLevelVolumeAgent_Z)
    INIT_AGENT_CLASS(ParticlesAgent_Z)
}
