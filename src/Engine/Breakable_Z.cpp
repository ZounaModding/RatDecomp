#include "Breakable_Z.h"

BEGIN_INIT_AGENT_CLASS(Breakable_Z, Throwable_Z)
END_INIT_AGENT_CLASS

Breakable_Z::Breakable_Z() {
    EnableFlag(FL_AGENT_BREAKABLE);
}

void Breakable_Z::Init() {
}

void Breakable_Z::Reset() {
}

void Breakable_Z::RegisterWithGame(Game_ZHdl& a1) {
}

void Breakable_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}

Bool Breakable_Z::MarkHandles() {
    return FALSE;
}

void Breakable_Z::UseNode(Node_ZHdl& a1) {
}

void Breakable_Z::Suspend() {
}

void Breakable_Z::Restore() {
}
