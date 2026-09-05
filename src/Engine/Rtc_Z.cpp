#include "Rtc_Z.h"
#include "Node_Z.h"
#include "Animation_Z.h"
#include "Sound_Z.h"

Rtc_Z::Rtc_Z() { }

Rtc_Z::~Rtc_Z() { }

Bool Rtc_Z::MarkHandles() {
    S32 i;

    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }

    for (i = 0; i < m_RtcAnimationNodes.GetSize(); i++) {
        m_RtcAnimationNodes[i].MarkHandles();
    }

    for (i = 0; i < m_RtcNodes.GetSize(); i++) {
        if (!m_RtcNodes[i].m_IsWorldNode) {
            m_RtcNodes[i].m_NodeHdl->MarkHandles();
        }
    }

    for (i = 0; i < m_RtcPersos.GetSize(); i++) {
        if (m_RtcPersos[i].m_AnimHdl.IsValid()) {
            m_RtcPersos[i].m_AnimHdl->MarkHandles();
        }

        if (m_RtcPersos[i].m_ObjectHdl.IsValid()) {
            m_RtcPersos[i].m_ObjectHdl->MarkHandles();
        }
    }

    for (i = 0; i < m_AnimHdls.GetSize(); i++) {
        if (m_AnimHdls[i].IsValid()) {
            m_AnimHdls[i]->MarkHandles();
        }
    }

    for (i = 0; i < m_SoundHdls.GetSize(); i++) {
        if (m_SoundHdls[i].IsValid()) {
            m_SoundHdls[i]->MarkHandles();
        }
    }

    m_MsgKfr.MarkHandles();

    return TRUE;
}

S32 Rtc_Z::GetCameraByName(const Name_Z& i_Name) {
    S32 i = 0;
    for (S32 i = 0; i < m_AnimationCameras.GetSize(); i++) {
        if (m_AnimationCameras[i].GetName() == i_Name) {
            return m_AnimationCameras[i].GetObjectId();
        }
    }
    return -1;
}
