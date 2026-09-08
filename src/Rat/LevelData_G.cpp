#include "LevelData_G.h"
#include <string.h>

void LevelData_G::SetRTCs(Char* i_Rtc1, Char* i_Rtc2) {
    if (i_Rtc1) {
        strcpy(m_Rtc1, i_Rtc1);
    }
    if (i_Rtc2) {
        strcpy(m_Rtc2, i_Rtc2);
    }
}

void LevelData_G::ResetAdvancement() {
}

MaterialLib* LevelData_G::GetMaterialLib(const Name_Z& i_Name) {
    return NULL;
}
