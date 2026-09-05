#include "Rtc_Z.h"
#include "ClassManager_Z.h"

void Rtc_Z::Load(void** i_Data) {
    ClassManager_Z* l_ClassMgr = gData.ClassMgr;
    S32 l_Nb;
    S32 i;

    LOAD_Z(m_Duration);

    LOAD_Z(l_Nb);
    m_RtcAnimationNodes.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        m_RtcAnimationNodes[i].Load(i_Data);
    }

    LOAD_Z(l_Nb);
    m_AnimationCameras.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        m_AnimationCameras[i].Load(i_Data);
    }

    LOAD_Z(l_Nb);
    m_AnimationOmnis.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        m_AnimationOmnis[i].Load(i_Data);
    }

    LOAD_Z(l_Nb);
    m_RtcNodes.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadName(m_RtcNodes[i].m_ParentName, i_Data);
        l_ClassMgr->LoadName(m_RtcNodes[i].m_NodeName, i_Data);
        l_ClassMgr->LoadName(m_RtcNodes[i].m_WorldNodeFileName, i_Data);
        LOAD_Z(m_RtcNodes[i].m_ParentIdx);
        LOAD_Z(m_RtcNodes[i].m_IsWorldNode);
        l_ClassMgr->LoadLink(m_RtcNodes[i].m_NodeHdl, i_Data);
        l_ClassMgr->LoadLink(m_RtcNodes[i].m_ParentNodeHdl, i_Data);
    }

    LOAD_Z(l_Nb);
    m_RtcPersos.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        LOAD_Z(m_RtcPersos[i].m_NodeIdx);
        l_ClassMgr->LoadName(m_RtcPersos[i].m_NodeName, i_Data);
        l_ClassMgr->LoadLink(m_RtcPersos[i].m_ObjectHdl, i_Data);
        l_ClassMgr->LoadLink(m_RtcPersos[i].m_AnimHdl, i_Data);
    }

    LOAD_Z(l_Nb);
    m_AnimHdls.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_AnimHdls[i], i_Data);
    }

    LOAD_Z(l_Nb);
    m_SoundHdls.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        l_ClassMgr->LoadLink(m_SoundHdls[i], i_Data);
    }

    m_MsgKfr.Load(i_Data);
}

void Rtc_Z::EndLoad() {
    ResourceObject_Z::EndLoad();

    ClassManager_Z* l_ClassMgr = gData.ClassMgr;
    S32 i;

    for (i = 0; i < m_RtcNodes.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_RtcNodes[i].m_NodeHdl);
        l_ClassMgr->UpdateLink(m_RtcNodes[i].m_ParentNodeHdl);
    }

    for (i = 0; i < m_RtcAnimationNodes.GetSize(); i++) {
        m_RtcAnimationNodes[i].EndLoad();
    }

    for (i = 0; i < m_RtcPersos.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_RtcPersos[i].m_ObjectHdl);
        l_ClassMgr->UpdateLink(m_RtcPersos[i].m_AnimHdl);
    }

    for (i = 0; i < m_AnimHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_AnimHdls[i]);
    }

    for (i = 0; i < m_SoundHdls.GetSize(); i++) {
        l_ClassMgr->UpdateLink(m_SoundHdls[i]);
    }

    m_MsgKfr.EndLoad();
}

void Rtc_Z::AfterEndLoad() {
}
