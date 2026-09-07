#include "TextGameDraw_G.h"
#include "Game_Z.h"
#include "World_Z.h"

void TextGameDraw_G::Init() {
    ManipulatorSceneDraw_Z::Init();
}

void TextGameDraw_G::Reset() {
    m_ObjTextDrawDA.Empty();
    m_ObjTextDrawDA.Minimize();
}

void TextGameDraw_G::Activate() {
    Manipulator_Z::Activate();
}

void TextGameDraw_G::Update(Float i_DeltaTime) {
    for (S32 i = 0; i < m_ObjTextDrawDA.GetSize(); i++) {
        if (m_ObjTextDrawDA[i].m_HasScroll) {
            m_ObjTextDrawDA[i].m_ScrollRelated0 += i_DeltaTime / m_ObjTextDrawDA[i].m_ScrollRelated2;
            if (m_ObjTextDrawDA[i].m_ScrollRelated0 > m_ObjTextDrawDA[i].m_ScrollRelated1 + 1.0f) {
                m_ObjTextDrawDA[i].m_ScrollRelated0 = 0.0f;
            }
        }
    }
}

void TextGameDraw_G::Draw(const DrawInfo_Z& i_DrawInfo) {
}

void TextGameDraw_G::Minimize() {
}

void TextGameDraw_G::StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
}

void TextGameDraw_G::StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
}

void TextGameDraw_G::SharedDataInit(const Game_ZHdl& i_GameHdl) {
    m_GameHdl = i_GameHdl;
    Node_ZHdl l_RootNodeHdl = i_GameHdl->GetWorld()->GetRoot();
    ParseHierarchy(l_RootNodeHdl, FALSE);
}

void TextGameDraw_G::ParseHierarchy(const Node_ZHdl& i_NodeHdl, Bool i_Remove) {
}
