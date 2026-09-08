#ifndef _MENUSTYLE_H_
#define _MENUSTYLE_H_
#include "Color_Z.h"
#include "Material_ZHdl.h"
#include "Name_Z.h"
#include "Types_Z.h"

#define MENU_STYLE_STATE_COUNT 6

struct MenuBitmap {
    MenuBitmap() {
        m_Name = Name_Z(0);
        m_MaterialHdl = HANDLE_NULL;
        m_X = 0.f;
        m_Y = 0.f;
        m_Width = 1.f;
        m_Height = 1.f;
    }

    void SetDimensions(Float i_X, Float i_Y, Float i_Width, Float i_Height);
    void GetDimensions(Float& o_X, Float& o_Y, Float& o_Width, Float& o_Height) const;

    void Set(Name_Z i_Name, Float i_X, Float i_Y, Float i_Width, Float i_Height) {
        m_Name = i_Name;
        SetDimensions(i_X, i_Y, i_Width, i_Height);
    }

    Name_Z m_Name;
    Material_ZHdl m_MaterialHdl;
    Float m_X;
    Float m_Y;
    Float m_Width;
    Float m_Height;
};

struct MenuBitmapColor : public Color {
    MenuBitmapColor() { }
};

struct BoxStyle {
    S32 m_Unk_0x00;
    Color m_Color;
};

template <class T>
class MenuStyle {
public:
    Name_Z m_Name;
    T m_States[MENU_STYLE_STATE_COUNT];
};

class styleBitmap : public MenuStyle<MenuBitmap> {
public:
    void SetMaterials();
    void SetAll(Name_Z i_Name, Float i_X, Float i_Y, Float i_Width, Float i_Height);
    void SetAllDimensions(Float i_X, Float i_Y, Float i_Width, Float i_Height);
};

class styleBitmapColor {
public:
    Name_Z m_Name;
    MenuBitmapColor m_States[MENU_STYLE_STATE_COUNT];
};

class styleText {
public:
    Name_Z m_Name;

private:
    U8 m_Unk_0x04[388];
};

class styleBox {
public:
    Name_Z m_Name;
    BoxStyle m_States[MENU_STYLE_STATE_COUNT];
    Float m_Unk_0x7c;
};

class SurroundingBitmapsStyle {
public:
    U8 m_Unk_0x00[244];
    Name_Z m_Name;
};

class CyclicAnimationStyle {
public:
    Name_Z m_Name;

private:
    U8 m_Unk_0x04[792];
};

typedef CyclicAnimationStyle CyclicAnimStyle;

#endif // _MENUSTYLE_H_
