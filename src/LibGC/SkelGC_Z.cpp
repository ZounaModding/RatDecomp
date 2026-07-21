#include "SkelGC_Z.h"
#include "GCMain_Z.h"

SkelGC_Z::SkelGC_Z() {
    m_LastFrameUpdated = gData.m_FrameCount - 1;
}

void SkelGC_Z::AfterEndLoad() {
    InitConstant();
}

void SkelGC_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Skel_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);
    ((SkelGC_Z*)(ObjectDatas_Z*)o_ObjectDatasHdl)->InitConstant();
}
