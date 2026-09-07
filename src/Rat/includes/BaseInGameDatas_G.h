#ifndef _BASEINGAMEDATAS_G_H_
#define _BASEINGAMEDATAS_G_H_
#include "InGameDatas_Z.h"
#include "Material_ZHdl.h"

#define BASE_IN_GAME_TEXT_WINDOW_COUNT 4
#define BASE_IN_GAME_MATERIAL_COUNT 96
#define FADE_TIMER_FULL 1.0f

#define FADE_STATE_DISABLED 0
#define FADE_STATE_FROM_BLACK 1
#define FADE_STATE_TO_BLACK 2
#define FADE_STATE_BLACK 3
#define FADE_STATE_UNUSED 4
#define FADE_STATE_FROM_WHITE 5
#define FADE_STATE_TO_WHITE 6
#define FADE_STATE_WHITE 7

extern Char* STC_TextureNames[BASE_IN_GAME_MATERIAL_COUNT];

class InterfacePage;
class Renderer_Z;
struct COMMON_INFOS;

struct TEXTWINDOWSIZE {
    U8 m_Data[0x2bc];
};

typedef DynArray_Z<InterfacePage*, 4> InterfacePagePtrDA;

class BaseInGameDatas_G : public InGameDatas_Z {
protected:
    static U8 BigFontID;

    InterfacePagePtrDA m_InterfacePages;
    U8 m_Unk_0x38[4];
    TEXTWINDOWSIZE m_TextWindowSizes[BASE_IN_GAME_TEXT_WINDOW_COUNT];
    Bool m_UnkBool_0xb2c;
    Material_ZHdl m_MaterialHdls[BASE_IN_GAME_MATERIAL_COUNT];
    U32 m_FadeState;
    U8 m_Unk_0xcb4[0xc];
    Float m_FadeTimer;
    U8 m_Unk_0xcc4[0x5f0];

public:
    void InitAfterSharedLoad();
    void DrawDebugWindow(Renderer_Z* i_Renderer, COMMON_INFOS& i_Infos);
    void AddPage(InterfacePage* i_Page);
    void StartFadeFromBlack(Float i_Duration, Agent_ZHdl i_AgentHdl, int i_Unk1, U32 i_Unk2, Bool i_Unk3, Bool i_Unk4, Bool i_Unk5);

    void StartFadeFromBlack(Float i_Duration) {
        StartFadeFromBlack(i_Duration, Agent_ZHdl(), 0, 0, FALSE, FALSE, FALSE);
    }

    Bool IsBlackScreen() const {
        Bool l_IsBlackScreen = FALSE;
        if (m_FadeState != FADE_STATE_DISABLED && m_FadeTimer == FADE_TIMER_FULL) {
            l_IsBlackScreen = TRUE;
        }
        return l_IsBlackScreen;
    }
};
#endif // _BASEINGAMEDATAS_G_H_
