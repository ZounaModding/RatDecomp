#include "ObjectMove_Z.h"

void ObjectMove_Z::DisableCollision(World_Z* i_World, Float i_DeltaTime, S32 i_Index) {
}

void ObjectMove_Z::RestoreCollision(SeadZone_Z& i_Zone, S32 i_Index) {
}

void ObjectMove_Z::BeforeUpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_Pos) {
}

void ObjectMove_Z::UpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, Vec3f& io_NewPos, Vec3f& io_Dir, Vec3f& io_Orient, Float i_DeltaTime, S32 i_Index) {
}

void ObjectMove_Z::EndUpdateCollision(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_CurNodePos, const Vec3f& i_NewPos, Float i_DeltaTime, S32 i_Index) {
}

void ObjectMove_Z::UpdateDynamic(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos, S32 i_Index) {
}

void ObjectMove_Z::UpdateDepth(SeadZone_Z& i_Zone, const Vec3f& i_Pos) {
}

void ObjectMove_Z::GetSphere(Sphere_Z& o_Sphere, U16 i_Type, S32 i_Index) {
}
