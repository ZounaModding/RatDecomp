#include "Manipulator_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "ManipulatorManager_Z.h"

Manipulator_Z::Manipulator_Z() {
    m_PreviousPtr = NULL;
    m_NextPtr = NULL;
    m_IsActive = FALSE;
    m_ManipGroup = ag_no_group;
    m_TimeSpentRunning = 0.0f;
}

Manipulator_Z::~Manipulator_Z() {
    if (gData.ManipulatorMgr != NULL)
        gData.ManipulatorMgr->Remove(this);
}

void Manipulator_Z::Init() {
    ASSERTLE_Z(!m_IsActive && m_ManipGroup == ag_no_group, "", 24, "!bActive && Group==ag_no_group");
    gData.ManipulatorMgr->Add(this);
}

void Manipulator_Z::Activate() {
    if (!m_IsActive) {
        gData.ManipulatorMgr->Activate(this);
        m_IsActive = TRUE;
        ActionOnActivate();
    }
}

void Manipulator_Z::Deactivate() {
    if (m_IsActive) {
        gData.ManipulatorMgr->Deactivate(this);
        m_IsActive = FALSE;
        ActionOnDeactivate();
    }
}

void Manipulator_Z::SetGroup(ActivableGroup_Z i_Group) {
    if (i_Group != m_ManipGroup) {
        gData.ManipulatorMgr->Order(this, i_Group);
    }
}

void Manipulator_Z::Heartbeat(Float i_DeltaTime) {
    m_TimeSpentRunning += i_DeltaTime;
    Update(i_DeltaTime);
}

ManipulatorDraw_Z::ManipulatorDraw_Z() {
    m_ViewportId = GLOBAL_VIEWPORT;
    m_ManipGroup = ag_ui_draw;
}

void ManipulatorDraw_Z::Init() {
    gData.ManipulatorMgr->Add(this);
}

void ManipulatorDraw_Z::Draw(const DrawInfo_Z& i_DrawInfo) {
    Draw(i_DrawInfo.m_Vp);
}

void ManipulatorDraw_Z::VpRegister() {
    gData.MainRdr->GetViewport(m_ViewportId).RegisterManip(GetHandle());
}

void ManipulatorDraw_Z::VpUnRegister() {
    gData.MainRdr->GetViewport(m_ViewportId).UnregisterManip(GetHandle());
}

void ManipulatorDraw_Z::VpRegister(S32 i_VpId) {
    m_ViewportId = i_VpId;
    gData.MainRdr->GetViewport(m_ViewportId).RegisterManip(GetHandle());
}

ManipulatorSceneDraw_Z::ManipulatorSceneDraw_Z() {
    m_ManipGroup = ag_draw;
}

void ManipulatorSceneDraw_Z::Init() {
    gData.ManipulatorMgr->Add(this);
}

void ManipulatorSceneDraw_Z::Draw(const DrawInfo_Z& i_DrawInfo) { }

void ManipulatorSceneDraw_Z::Update(Float i_DeltaTime) { }
