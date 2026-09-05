#include "ObjectsMove_Z.h"

void ObjectsMove_Z::Shut() {
}

void ObjectsMove_Z::Init() {
}

void ObjectsMove_Z::Reset() {
}

void ObjectsMove_Z::IsVisible() {
}

void ObjectsMove_Z::SetNodeObjectNb(S32 i_Count) {
}

void ObjectsMove_Z::RemoveObjectMove(S32 i_Index) {
}

void ObjectsMove_Z::EndLiveTimeObject(S32 i_Index) {
    RemoveObjectMove(i_Index);
}

const Vec3f& ObjectsMove_Z::GetPos(S32 i_Index) {
    Node_Z* l_Node = GetNode(i_Index);

    return l_Node->GetWorldTranslation();
}

Float ObjectsMove_Z::GetScale(S32 i_Index) {
    Node_Z* l_Node = GetNode(i_Index);
    Mat4x4& l_Mat = l_Node->GetWorldMatrix();
    Float l_X = l_Mat.m[0][0];
    Float l_Y = l_Mat.m[1][0];
    Float l_Z = l_Mat.m[2][0];

    return Sqrt(l_X * l_X + l_Y * l_Y + l_Z * l_Z);
}

Quat& ObjectsMove_Z::GetRot(S32 i_Index) {
    Node_Z* l_Node = GetNode(i_Index);

    return l_Node->GetRotInWorld();
}

void ObjectsMove_Z::SetPos(const Vec3f& i_Pos, S32 i_Index) {
}

void ObjectsMove_Z::SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index) {
}

void ObjectsMove_Z::SetPosAndScale(const Vec3f& i_Pos, Float i_Scale, S32 i_Index) {
}

void ObjectsMove_Z::SetScale(Float i_Scale, S32 i_Index) {
}

void ObjectsMove_Z::SetPosRotAndScale(const Vec3f& i_Pos, const Quat& i_Rot, Float i_Scale, S32 i_Index) {
}

void ObjectsMove_Z::ComputeNewPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime, S32 i_Index) {
}

void ObjectsMove_Z::Update(Float i_DeltaTime) {
}

void ObjectsMove_Z::Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index) {
}

void ObjectsMove_Z::SetCollideData(const Node_ZHdl& i_NodeHdl, const ObjectMoveCollData_Z& i_CollideData) {
}

void ObjectsMove_Z::UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index) {
}

void ObjectsMove_Z::UpdateCollision(const ColLineResult_Z& i_Result, ObjectMoveSphereColl_Z& io_SphereColl, Float i_DeltaTime, S32 i_Index) {
}

void ObjectsMove_Z::FatherUnLinked(S32 i_Index) {
}
