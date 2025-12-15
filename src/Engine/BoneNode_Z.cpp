#include "BoneNode_Z.h"
#include "Program_Z.h"

BoneNode_Z::BoneNode_Z() {
}

BoneNode_Z::~BoneNode_Z() {
    S16 l_WorldMatId = GetWorldMatrixId();
    S16 l_InvWorldMatId = GetInverseWorldMatrixId();
    if (gData.MatrixBuffer) {
        gData.MatrixBuffer->RemoveMatrix(l_WorldMatId);
        gData.MatrixBuffer->RemoveMatrix(l_InvWorldMatId);
        SetWorldMatrixId(0);
        SetInverseWorldMatrixId(0);
    }
}
