#include "AnimFrame_Z.h"
#include "Program_Z.h"

// TODO: Finish matching
void AnimFrame_Z::Load(void** i_Data) {
    gData.ClassMgr->LoadLink(m_AnimatedNodeHdl, i_Data);
    LOAD_Z(m_MaxTime);
    m_TransKfr.Load(i_Data);
    m_RotKfr.Load(i_Data);
    m_ScaleKfr.Load(i_Data);
    m_TimeKfr.Load(i_Data);
    m_ColorKfr.Load(i_Data);
    m_AmbientKfr.Load(i_Data);
    m_MsgKfr.Load(i_Data);
    S32 i;
    S32 l_Nb;
    LOAD_Z(l_Nb);
    m_FollowSplineKfr.SetNbKey(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        Node_ZHdl& l_NodeHdl = m_FollowSplineKfr.m_Keys[i].m_NodeHdl;
        gData.ClassMgr->LoadLink(l_NodeHdl, i_Data);
        LOADRANGE_Z(m_FollowSplineKfr.m_Keys[i].m_Axis, m_FollowSplineKfr.m_Keys[i].m_Orient);
    }
    LOAD_Z(l_Nb);
    m_StartStopAnimKfr.m_Keys.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        S32 l_Nb2;
        LOAD_Z(l_Nb2);
        m_StartStopAnimKfr.m_Keys[i].m_StartStops.SetSize(l_Nb2);
        LOAD_Z(m_StartStopAnimKfr.m_Keys[i].m_Time);
        for (S32 j = 0; j < l_Nb2; j++) {
            StartStop_ZDA& l_StartStops = m_StartStopAnimKfr.m_Keys[i].m_StartStops;
            gData.ClassMgr->LoadLink(l_StartStops[j].m_AnimHdl, i_Data);
            // $SABE: If I do m_StartStopAnimKfr.m_Keys[i].m_StartStops again it generates the lwzx's
            U32 l_Value;
            LOAD_Z(l_StartStops[j].m_Value);
        }
    }
    LOADRANGE_Z(m_CurTime, m_PlayFlag);
}

void AnimFrame_Z::EndLoad() {
    S32 i;
    S32 j;
    m_ResObjLink.EndLoad();
    gData.ClassMgr->UpdateLink(m_AnimatedNodeHdl);
    for (i = 0; i < m_FollowSplineKfr.GetNbKeys(); i++) {
        gData.ClassMgr->UpdateLink(m_FollowSplineKfr.m_Keys[i].m_NodeHdl);
    }
    for (i = 0; i < m_StartStopAnimKfr.GetNbKeys(); i++) {
        for (j = 0; j < m_StartStopAnimKfr.m_Keys[i].GetNb(); j++) {
            gData.ClassMgr->UpdateLink(m_StartStopAnimKfr.m_Keys[i].Get(j).m_AnimHdl);
        }
    }
    m_MsgKfr.EndLoad();
}
