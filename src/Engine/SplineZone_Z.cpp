#include "SplineZone_Z.h"

SplineZone_Z::SplineZone_Z() {
    SetGeometryType(SPLINEZONE_Z);
}

void SplineZone_Z::Clean() {
}

void SplineZone_Z::Reset() {
}

Bool SplineZone_Z::GetZone(const Vec3f& i_CamPos, S32& o_ZoneId) {
    // TODO: Match
    return FALSE;
}
