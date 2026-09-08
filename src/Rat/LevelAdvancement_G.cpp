#include "LevelAdvancement_G.h"

void MissionDef_G::IncreaseNbTimesPlayed() {
    m_NbTimesPlayed++;
}

Bool MissionDef_G::IsMissionOnCurrentPlateform() {
    if (m_PlatformFlags == MISSION_PLATFORM_ALL) {
        return TRUE;
    }
    return m_PlatformFlags & MISSION_PLATFORM_GC;
}
