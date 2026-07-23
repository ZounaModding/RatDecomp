#include "FlareGC_Z.h"

VizQueryStreamList_Z FlareDataGC_Z::VizQueryStreamList;

void FlareDataGC_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("FlareData_Z", 0));
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, m_Name);
    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);
    ObjectDatas_Z* l_ObjectDatas = (ObjectDatas_Z*)GETPTR(o_ObjectDatasHdl);
    l_ObjectDatas->UpdatePointers(o_ObjectHdl, 0);
}

void VizQueryStreamList_Z::SetStream() {
}

void VizQueryStreamList_Z::SetVtxDesc() {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
}

void FlareDataGC_Z::UpdatePointers(Object_Z* i_Obj, S32 i_Param) {
}
