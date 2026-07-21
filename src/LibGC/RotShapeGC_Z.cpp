#include "RotShapeGC_Z.h"

void RotShapeGCData_Z::Load(void** i_Data) {
    RotShapeData_Z::Load(i_Data);
}

void RotShapeGCData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    RotShapeData_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);
    UpdatePointers(o_ObjectHdl, -1);
}
