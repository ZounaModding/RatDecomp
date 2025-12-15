#include "AnimationOmni_Z.h"
#include "Program_Z.h"
#include "Omni_Z.h"

void AnimationOmni_Z::UpdateFrame(Float i_Time, Omni_Z* i_Omni) {
    // Start
    if (m_StartKfr.GetNbKeys()) {
        Float l_StartValue;
        m_StartKfr.GetValue(i_Time, l_StartValue);
        i_Omni->SetStart(l_StartValue);
    }

    // End
    if (m_EndKfr.GetNbKeys()) {
        Float l_EndValue;
        m_EndKfr.GetValue(i_Time, l_EndValue);
        i_Omni->SetEnd(l_EndValue);
    }

    // Color
    if (m_ColorKfr.GetNbKeys()) {
        Vec3f l_ColorValue;
        m_ColorKfr.GetValue(i_Time, l_ColorValue);
        i_Omni->SetColor(l_ColorValue);
    }
}

void AnimationOmni_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadName(m_ObjectName, i_Data);
    LOAD_Z(m_ObjectId);
    m_ColorKfr.Load(i_Data);
    m_StartKfr.Load(i_Data);
    m_EndKfr.Load(i_Data);
}
