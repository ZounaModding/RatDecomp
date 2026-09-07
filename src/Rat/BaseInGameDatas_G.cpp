#include "BaseInGameDatas_G.h"
#include "MaterialAnim_Z.h"
#include "Names.h"
#include "Program_Z.h"
#include "SystemDatas_Z.h"

Char* STC_TextureNames[BASE_IN_GAME_MATERIAL_COUNT] = {
    "INT_FOND_NOIR",
    "INT_WHITE",
    "INT_CASE",
    "INT_LIFE_REMY",
    "INT_LIFE01",
    "INT_LIFE02",
    "INT_LIFE03",
    "INT_LIFE04",
    "INT_LIFE05",
    "INT_LIFE06",
    "INT_LIFE07",
    "INT_LIFE08",
    "INT_LIFE_FOND",
    "INT_LIFE01MAX",
    "INT_LIFE02MAX",
    "INT_LIFE03MAX",
    "INT_LIFE04MAX",
    "INT_LIFE05MAX",
    "INT_REMY",
    "INT_VERT_JAUGE",
    "INT_GOLF_FOND",
    "INT_GOLF_BLANC",
    "INT_GOLF_VERT",
    "INT_GOLF_BARRE",
    "SKINNER_OK",
    "SKINNER_COLERE",
    "SKINNER_COLERE02",
    "INT_COLLECT",
    "INT_DANSE",
    "INT_SELECT",
    "INT_SPARK",
    "INT_SPARK02",
    "INT_SPARK03",
    "INT_WAVE",
    "INT_OPTION_PLOT",
    "INT_MSGBOX",
    "INT_REMYCUILLER",
    "INT_GUSTEAU",
    "INT_EMILE",
    "INT_FOND_RADAR",
    "INT_PLOT_RADAR",
    "INT_REMY_RADAR",
    "FX_SMELLO",
    "SMELLPATH",
    "SMELLPATHFRONT",
    "INT_TURBO",
    "INT_MAGNET",
    "INT_INVERT",
    "TRACEPAS",
    "WII_CURSEUR",
    "WII_CURSEUR1",
    "WII_CURSEUR2",
    "INT_BOXMENU",
    "INT_MENU01",
    "INT_MENU02",
    "INT_MENU03",
    "INT_MENU04",
    "INT_MENU_GRIS",
    "INT_OVER01",
    "INT_OVER02",
    "INT_OVER03",
    "INT_REGLET01",
    "INT_WAVES_COTE",
    "INT_CARTOUCHE",
    "BANDE_SELECT",
    "INFOBOX",
    "BOX_VIDEO",
    "BOX_MARRON",
    "BOX_VERTE",
    "SELECT",
    "TOUCHEPC",
    "TOUCHEPCSELECT",
    "LV_FOND",
    "INT_BOXMENU_BOOK",
    "INT_TEXTBOX",
    "INT_TEXTBOX02",
    "INT_PROFILE_FOND",
    "TOUCHEPC02",
    "FOND_GSHOP",
    "INT_BOX",
    "INT_BOX_SMALL",
    "INT_BOX_SHADOW",
    "INT_EQUERRE",
    "INT_EXCLAM_TOUR",
    "INT_EXCLAM",
    "INT_EXCLAM_BLANC",
    "INT_EXCLAM_FLECHE",
    "INT_MGITEM",
    "INT_TIMER",
    "INT_TIMER_CONT",
    "INT_TIMER_FULL",
    "INT_MGBOX",
    "INT_MGJAUGE",
    "INT_DEBATR",
    "INT_DEBATL",
    "INT_FLECHE",
};

U8 BaseInGameDatas_G::BigFontID = 0xff;

void BaseInGameDatas_G::InitAfterSharedLoad() {
    BigFontID = gData.SystemDatas->GetFontId(fontName);

    MaterialAnim_ZHdl l_MaterialAnimHdl;
    for (S32 i = 0; i < BASE_IN_GAME_MATERIAL_COUNT; ++i) {
        if (!m_MaterialHdls[i].IsValid()) {
            l_MaterialAnimHdl = gData.SystemDatas->GetMaterialByName(Name_Z(STC_TextureNames[i]));
            if (l_MaterialAnimHdl.IsValid()) {
                m_MaterialHdls[i] = l_MaterialAnimHdl->GetMaterial();
            }
        }
    }
}

void BaseInGameDatas_G::StartFadeFromBlack(Float i_Duration, Agent_ZHdl i_AgentHdl, int i_Unk1, U32 i_Unk2, Bool i_Unk3, Bool i_Unk4, Bool i_Unk5) {
}

void BaseInGameDatas_G::DrawDebugWindow(Renderer_Z* i_Renderer, COMMON_INFOS& i_Infos) {
}

void BaseInGameDatas_G::AddPage(InterfacePage* i_Page) {
    m_InterfacePages.Add(i_Page);
}
