#ifndef _CAMERAZONEDATA_Z_H_
#define _CAMERAZONEDATA_Z_H_

#include "CameraZone_ZHdl.h"
#include "Color_Z.h"
#include "Math_Z.h"

#define MAX_TRIGGER_PER_ZONE 4

struct Trigger_Z {
    Float m_Rotation;
    Float m_Fov;
    Float m_Height;
    Float m_Distance;
    Float m_Fog;
    Float m_Far;
    Float m_Smooth;
    Color m_Color;
    U16 m_Flag;
    U16 m_SplineId;
    U16 m_PointId;
    U16 m_AtPointId;
    Float m_SplineLength;
    Vec3f m_Unk;
};

struct CameraZoneData_Z {
    S32 m_ZoneId;
    CameraZone_ZHdl m_CameraZoneHdl;
    U32 m_NbTriggerFromZone;
    U32 m_TriggerIds[MAX_TRIGGER_PER_ZONE];
    Trigger_Z m_TriggersFromZone[MAX_TRIGGER_PER_ZONE];
};

#endif // _CAMERAZONEDATA_Z_H_
