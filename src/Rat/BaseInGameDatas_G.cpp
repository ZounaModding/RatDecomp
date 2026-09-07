#include "BaseInGameDatas_G.h"
#include "Names.h"

void BaseInGameDatas_G::StartFadeFromBlack(Float i_Duration, Agent_ZHdl i_AgentHdl, int i_Unk1, U32 i_Unk2, Bool i_Unk3, Bool i_Unk4, Bool i_Unk5) {
}

void BaseInGameDatas_G::DrawDebugWindow(Renderer_Z* i_Renderer, COMMON_INFOS& i_Infos) {
}

void BaseInGameDatas_G::AddPage(InterfacePage* i_Page) {
    m_InterfacePages.Add(i_Page);
}
