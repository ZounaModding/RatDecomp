#include "ParticlesAgent_Z.h"

BEGIN_INIT_AGENT_CLASS(ParticlesAgent_Z, Agent_Z)
END_INIT_AGENT_CLASS

ParticlesAgent_Z::ParticlesAgent_Z() {
    EnableFlag(FL_AGENT_PARTICLES);
}

void ParticlesAgent_Z::Init() {
}

void ParticlesAgent_Z::Reset() {
}

void ParticlesAgent_Z::Suspend() {
}

void ParticlesAgent_Z::Restore() {
}

Bool ParticlesAgent_Z::MarkHandles() {
    return FALSE;
}

void ParticlesAgent_Z::UseNode(Node_ZHdl& a1) {
}

void ParticlesAgent_Z::RegisterWithGame(Game_ZHdl& a1) {
}

void ParticlesAgent_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}

void ParticlesAgent_Z::DoFollow() {
}
