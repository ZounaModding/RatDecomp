#include "MaterialGC_Z.h"

MaterialGC_Z::MaterialGC_Z() {
}

MaterialGC_Z::~MaterialGC_Z() {
}

void MaterialGC_Z::EndLoad() {
    Material_Z::EndLoad();
}

void MaterialGC_Z::Clone(Material_ZHdl& o_MaterialHdl) {
    Material_Z::Clone(o_MaterialHdl);
}
