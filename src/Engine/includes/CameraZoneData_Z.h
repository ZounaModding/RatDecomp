#ifndef _CAMERAZONEDATA_Z_H_
#define _CAMERAZONEDATA_Z_H_

#include "CameraZone_ZHdl.h"
#include "Color_Z.h"
#include "Math_Z.h"

#define MAX_TRIGGER_PER_ZONE 4

#define FL_TRIGGER_NOFIX (1 << 14)
#define FL_TRIGGER_NOFOCUS (1 << 15)

struct Trigger_Z {
    Trigger_Z() { }

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
    CameraZoneData_Z() {
        m_ZoneId = -1;
        m_CameraZoneHdl = HANDLE_NULL;
    }

    S32 m_ZoneId;
    CameraZone_ZHdl m_CameraZoneHdl;
    S32 m_NbTriggerFromZone;
    S32 m_TriggerIds[MAX_TRIGGER_PER_ZONE];
    Trigger_Z m_TriggersFromZone[MAX_TRIGGER_PER_ZONE];
};

struct CameraZoneInterpolData_Z {
    CameraZoneInterpolData_Z() {
        m_IsInterpol = FALSE;
    }

    Bool m_IsInterpol;
    Vec3f m_CamFocus;
    Vec3f m_CamPos;
    Float m_CamFov;
    Float m_CamFar;
    Float m_Smooth;
    Color m_Color;
    Float m_Height;
    Float m_Distance;
};

#endif // _CAMERAZONEDATA_Z_H_
