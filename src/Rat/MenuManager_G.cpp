#include "Handle_Z.h"
#include "MenuManager_G.h"

static Bool FirstTimeEnteringMManager = TRUE;

void MenuManager_G::Init() {
    S32 i;
    BaseInGameDatas_G::Init();
    m_UnkBool_0x12b5 = FALSE;
    if (!FirstTimeEnteringMManager) {
        UnHideMouse();
    }
    FirstTimeEnteringMManager = FALSE;
    SetGroup(ag_draw_notpaused);
    for (S32 i = 0; i < 71; i++) {
        m_Dialogs[i] = NULL;
    }
    m_UnkBool_0x12ce = FALSE;
    m_NextDialog = -1;
    ResetKeys();
    SetDebug(FALSE);
    m_DialogColor = Color(0.f, 0.f, 0.f, 0.f);
    m_UnkBool_0xf340 = FALSE;
    m_UnkVec2_0xf338 = Vec2f(-1.f, -1.f);
    SetMenu3DScenery(new Book(this));
    m_UnkS32_0x12c4 = 0;
    m_UnkBool_0x12bc = FALSE;
    m_Menu3DAnimationId = m_Menu3DScenery->m_AnimationId;
    m_UnkBool_0xf328 = FALSE;
    m_VoiceTimer = -10.f;
    m_CurrentVoiceId = -1;
    for (i = 0; i < 4; i++) {
        m_ActionIds[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        m_ActionEnabled[i] = 0;
    }
    m_ActionEnabled[0] = TRUE;
    m_UnkBool_0xf2b8 = FALSE;
    m_UnkS32_0x12b8 = 0;
    m_UnkBool_0x12b4 = TRUE;
}

void MenuManager_G::SetRessourcesParsed(Bool i_Parsed) {
    m_RessourcesParsed = i_Parsed;
}

void MenuManager_G::ResetStyles() {
}

styleText* MenuManager_G::GetStyleTextByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 64; ++l_Index) {
        if (m_StyleTexts[l_Index].m_Name == i_Name) {
            return &m_StyleTexts[l_Index];
        }
    }
    return NULL;
}

S32 MenuManager_G::GetIdStyleText(styleText* i_Style) {
    for (S32 l_Index = 0; l_Index < 64; ++l_Index) {
        if (&m_StyleTexts[l_Index] == i_Style) {
            return l_Index;
        }
    }
    return -1;
}

styleBox* MenuManager_G::GetStyleBoxByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 32; ++l_Index) {
        if (m_StyleBoxes[l_Index].m_Name == i_Name) {
            return &m_StyleBoxes[l_Index];
        }
    }
    return NULL;
}

S32 MenuManager_G::GetIdStyleBox(styleBox* i_Style) {
    for (S32 l_Index = 0; l_Index < 32; ++l_Index) {
        if (&m_StyleBoxes[l_Index] == i_Style) {
            return l_Index;
        }
    }
    return -1;
}

S32 MenuManager_G::GetIdStyleBitmap(styleBitmap* i_Style) {
    for (S32 l_Index = 0; l_Index < 64; ++l_Index) {
        if (&m_StyleBitmaps[l_Index] == i_Style) {
            return l_Index;
        }
    }
    return -1;
}

S32 MenuManager_G::GetIdStyleBitmapColor(styleBitmapColor* i_Style) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (&m_StyleBitmapColors[l_Index] == i_Style) {
            return l_Index;
        }
    }
    return -1;
}

styleBitmap* MenuManager_G::GetStyleBitmapByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 64; ++l_Index) {
        if (m_StyleBitmaps[l_Index].m_Name == i_Name) {
            return &m_StyleBitmaps[l_Index];
        }
    }
    return NULL;
}

styleBitmapColor* MenuManager_G::GetStyleBitmapColorByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (m_StyleBitmapColors[l_Index].m_Name == i_Name) {
            return &m_StyleBitmapColors[l_Index];
        }
    }
    return NULL;
}

SurroundingBitmapsStyle* MenuManager_G::GetSurroundingBitmapsStyleByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (m_SurroundingBitmapsStyles[l_Index].m_Name == i_Name) {
            return &m_SurroundingBitmapsStyles[l_Index];
        }
    }
    return NULL;
}

S32 MenuManager_G::GetIdSurroundingBitmapsStyle(SurroundingBitmapsStyle* i_Style) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (&m_SurroundingBitmapsStyles[l_Index] == i_Style) {
            return l_Index;
        }
    }
    return -1;
}

CyclicAnimStyle* MenuManager_G::GetCyclicAnimStyleByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (m_CyclicAnimStyles[l_Index].m_Name == i_Name) {
            return &m_CyclicAnimStyles[l_Index];
        }
    }
    return NULL;
}

S32 MenuManager_G::GetCyclicAnimStyleIdByName(const Name_Z& i_Name) {
    for (S32 l_Index = 0; l_Index < 16; ++l_Index) {
        if (m_CyclicAnimStyles[l_Index].m_Name == i_Name) {
            return l_Index;
        }
    }
    return -1;
}

void MenuManager_G::InitVoices() {
}
