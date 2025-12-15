#include "AnimationCamera_Z.h"
#include "Program_Z.h"
#include "Camera_Z.h"

void AnimationCamera_Z::UpdateFrame(Float i_Time, Camera_Z* i_Camera) {
    // Roll
    if (m_RollKfr.GetNbKeys()) {
        Float l_RollValue;
        m_RollKfr.GetValue(i_Time, l_RollValue);
        i_Camera->SetRoll(l_RollValue);
    }

    // Fov
    if (m_FovKfr.GetNbKeys()) {
        Float l_FovValue;
        m_FovKfr.GetValue(i_Time, l_FovValue);
        i_Camera->SetFov(l_FovValue);
    }

    // Far Clip
    if (m_FarClipKfr.GetNbKeys()) {
        Float l_FarValue;
        m_FarClipKfr.GetValue(i_Time, l_FarValue);
        i_Camera->SetFarClip(l_FarValue);
    }

    // Near Clip
    if (m_NearClipKfr.GetNbKeys()) {
        Float l_NearValue;
        m_NearClipKfr.GetValue(i_Time, l_NearValue);
        if (l_NearValue > 0.f) {
            i_Camera->SetNearClip(l_NearValue);
        }
    }
}

void AnimationCamera_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadName(m_ObjectName, i_Data);
    LOAD_Z(m_ObjectId);
    m_RollKfr.Load(i_Data);
    m_FovKfr.Load(i_Data);
    m_FarClipKfr.Load(i_Data);
    m_NearClipKfr.Load(i_Data);
}
