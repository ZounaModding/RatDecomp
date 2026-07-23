#include "LodMove_Z.h"

void LodMove_Z::Init() {
}

void LodMove_Z::Reset() {
}

const Vec3f& LodMove_Z::GetPos(S32 i_Index) {
}

void LodMove_Z::SetPos(const Vec3f& i_Pos, S32 i_Index) {
}

void LodMove_Z::SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index) {
}

void LodMove_Z::Update(Float i_DeltaTime) {
}

void LodMove_Z::Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index) {
}

Bool LodMove_Z::UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index) {
    return FALSE;
}
