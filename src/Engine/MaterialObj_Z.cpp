#include "MaterialObj_Z.h"

Bool MaterialObj_Z::MarkHandles() {
    return FALSE;
}

void MaterialObj_Z::Load(void** i_Data) {
}

void MaterialObj_Z::EndLoad() {
}

MaterialAnim_ZHdl MaterialObj_Z::GetMaterialByName(const Name_Z& i_Name) {
    return MaterialAnim_ZHdl();
}
