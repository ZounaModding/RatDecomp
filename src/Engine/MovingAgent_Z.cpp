#include "MovingAgent_Z.h"

BEGIN_INIT_AGENT_CLASS(MovingAgent_Z, AnimatedAgent_Z)
END_INIT_AGENT_CLASS

MovingAgent_Z::MovingAgent_Z() {
    EnableFlag(FL_AGENT_UNK_0x1);
}

void MovingAgent_Z::Init() {
}

void MovingAgent_Z::SetObjectMove(const BaseObject_ZHdl& i_ObjectMoveHdl) {
}

void MovingAgent_Z::UseNode(Node_ZHdl& a1) {
}

void MovingAgent_Z::Suspend() {
}

void MovingAgent_Z::Restore() {
}

void MovingAgent_Z::RegisterWithGame(Game_ZHdl& a1) {
}

void MovingAgent_Z::UnRegisterFromGame(Game_ZHdl& a1) {
}

Bool MovingAgent_Z::MarkHandles() {
    return FALSE;
}
