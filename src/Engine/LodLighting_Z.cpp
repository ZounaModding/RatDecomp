#include "Lod_Z.h"
#include "Program_Z.h"

void LodData_Z::SetDfltColor(const Color& i_Color) {
    m_DefaultColor = i_Color;

    for (S32 i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        m_ObjectDatasHdls[i]->SetDfltColor(i_Color);
    }
}

void LodData_Z::SetDfltColorAlpha(Float i_Alpha) {
}

void LodData_Z::SetDfltColorNoAlpha(const Color& i_Color) {
}

Bool LodData_Z::SetLightingData(Node_Z* i_Node, Node_Z* i_ReceiverNode, Float i_InterpolationTime) {
    if (!m_ActorData) {
        return FALSE;
    }
    m_ActorData->SetLightingData(i_Node, i_ReceiverNode, i_InterpolationTime);
    return TRUE;
}

void LodData_Z::SetLightingData(LightData_ZHdl i_LightDataHdl) {
}
