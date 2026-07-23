#include "ObjectThrow_Z.h"

void ObjectThrow_Z::Init() {
}

void ObjectThrow_Z::Reset() {
}

void ObjectThrow_Z::SetNodeObject(const Node_ZHdl& i_NodeHdl, S32 i_Index) {
}

void ObjectThrow_Z::Update(Float i_DeltaTime) {
}

void ObjectThrow_Z::Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index) {
}

void ObjectThrow_Z::UnLinkThrow(ObjectMoveData_Z* i_MoveData) {
}

void ObjectThrow_Z::Abort() {
}

void ObjectThrow_Z::UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index) {
}

void ObjectThrow_Z::UpdateCollision(const ColLineResult_Z& i_Result, ObjectMoveSphereColl_Z& io_SphereColl, Float i_DeltaTime, S32 i_Index) {
}

void ObjectThrow_Z::EndLiveTimeObject() {
}

void ObjectThrow_Z::FatherUnLinked(S32 i_Index) {
}

Bool ObjectThrow_Z::UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index) {
    return FALSE;
}
