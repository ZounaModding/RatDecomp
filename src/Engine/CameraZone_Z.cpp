#include "CameraZone_Z.h"

void CameraZone_Z::Clean() {
}

void CameraZone_Z::Reset() {
}

Bool CameraZone_Z::GetCameraZoneData(const Vec3f& i_WorldPosition, CameraZoneData_Z& o_Data) {
    return FALSE;
}

U16 CameraZone_Z::GetTriggerFlag(S32 i_TriggerId) {
    return 0;
}

void CameraZone_Z::EnableTriggerFlag(S32 i_TriggerId, U16 i_Flag) {
}

Bool CameraZone_Z::GetCameraZoneInterpol(const Vec3f& i_WorldPosition, const Vec3f& i_WorldFocus, const CameraZoneData_Z& i_Data, CameraZoneInterpolData_Z& o_Data) {
    return FALSE;
}
