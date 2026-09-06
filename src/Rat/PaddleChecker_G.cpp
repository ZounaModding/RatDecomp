#include "PaddleChecker_G.h"

PaddleChecker_G::PaddleChecker_G() {
}

void PaddleChecker_G::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_notpaused_first);
}

void PaddleChecker_G::Update(Float i_DeltaTime) {
}
