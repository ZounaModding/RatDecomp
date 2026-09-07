#include "ActionHelper_G.h"
#include "Game_Z.h"
#include "World_Z.h"

void ActionHelper_G::Init() {
    ManipulatorSceneDraw_Z::Init();
}

void ActionHelper_G::Activate() {
    ManipulatorSceneDraw_Z::Activate();
}

void ActionHelper_G::Reset() {
    m_ActionHelperDA.Empty();
    m_ActionHelperDA.Minimize();
}

Bool ActionHelper_G::Minimize() {
    m_ActionHelperDA.Empty();
    m_ActionHelperDA.Minimize();
    return TRUE;
}

void ActionHelper_G::Update(Float i_DeltaTime) {
}

void ActionHelper_G::Draw(const DrawInfo_Z& i_DrawInfo) {
}

void ActionHelper_G::StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
}

void ActionHelper_G::StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
}

void ActionHelper_G::SharedDataInit(const Game_ZHdl& i_GameHdl) {
    m_GameHdl = i_GameHdl;
    Node_ZHdl l_RootNodeHdl = i_GameHdl->GetWorld()->GetRoot();
    ParseHierarchy(l_RootNodeHdl, FALSE);
}

void ActionHelper_G::ParseHierarchy(const Node_ZHdl& i_NodeHdl, Bool i_Remove) {
}
