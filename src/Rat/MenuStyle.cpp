#include "MenuStyle.h"

void MenuBitmap::SetDimensions(Float i_X, Float i_Y, Float i_Width, Float i_Height) {
    m_X = i_X;
    m_Y = i_Y;
    m_Width = i_Width;
    m_Height = i_Height;
}

void MenuBitmap::GetDimensions(Float& o_X, Float& o_Y, Float& o_Width, Float& o_Height) const {
    o_X = m_X;
    o_Y = m_Y;
    o_Width = m_Width;
    o_Height = m_Height;
}

void styleBitmap::SetAll(Name_Z i_Name, Float i_X, Float i_Y, Float i_Width, Float i_Height) {
    m_Name = i_Name;
    for (S32 l_Index = 0; l_Index < MENU_STYLE_STATE_COUNT; ++l_Index) {
        m_States[l_Index].m_Name = i_Name;
    }
    SetAllDimensions(i_X, i_Y, i_Width, i_Height);
}

void styleBitmap::SetAllDimensions(Float i_X, Float i_Y, Float i_Width, Float i_Height) {
    for (S32 l_Index = 0; l_Index < MENU_STYLE_STATE_COUNT; ++l_Index) {
        m_States[l_Index].SetDimensions(i_X, i_Y, i_Width, i_Height);
    }
}
