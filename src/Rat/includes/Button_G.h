#ifndef _BUTTON_G_H_
#define _BUTTON_G_H_
#include "Types_Z.h"

class styleBitmap;

class Button_G {
public:
    S32 GetId() { return m_Id; }

    Bool IsSelected() { return m_Selected; }

    Bool IsHidden() { return m_Hidden; }

    Bool IsPushed() { return m_Pushed; }

    S32 GetTextId() { return m_TextId; }

    S32 GetTextStyle() { return m_TextStyle; }

    Char* GetAddText() const { return m_AddText; }

    S32 GetUserValue1() { return m_UserValue1; }

    void SetBlink(Bool i_Blink, Float i_OnTime, Float i_OffTime) {
        m_Blink = i_Blink;
        m_BlinkOnTime = i_OnTime;
        m_BlinkOffTime = i_OffTime;
    }

    void SetSelectable(Bool i_Selectable);
    void SetSelectableWithMouse(Bool i_Selectable);
    void SetHidden(Bool i_Hidden);

    void SetAvailable(Bool i_Available) { m_Available = i_Available; }

    void SetOwned(Bool i_Owned) { m_Owned = i_Owned; }

    void SetPushed(Bool i_Pushed) { m_Pushed = i_Pushed; }

    void SetSelectableWithPad(Bool i_Selectable) { m_SelectableWithPad = i_Selectable; }

    void SetSurroundingBitmapsStyle(S32 i_Style) { m_SurroundingBitmapsStyle = i_Style; }

    void SetFullScreen(Bool i_FullScreen) { m_FullScreen = i_FullScreen; }

    void SetStableY(Bool i_StableY) { m_StableY = i_StableY; }

    void SetCyclicAnimated(Bool i_Animated) { m_CyclicAnimated = i_Animated; }

    void SetTextVertAlign(Bool i_Align) { m_TextVertAlign = i_Align; }

    void SetBoxStyle(S32 i_Style) { m_BoxStyle = i_Style; }

    void SetText(S32 i_TextId, S32 i_Align) {
        m_TextId = i_TextId;
        m_TextAlign = i_Align;
    }

    void SetTextAlign(S32 i_Align) { m_TextAlign = i_Align; }

    void SetTextStyle(S32 i_Style) { m_TextStyle = i_Style; }

    void SetDescTextId(S32 i_TextId);

    void SetDescButtonId(S32 i_ButtonId) { m_DescButtonId = i_ButtonId; }

    void SetPictDesc(styleBitmap& i_Style);

    void SetPictDescButtonId(S32 i_ButtonId) { m_PictDescButtonId = i_ButtonId; }

    void SetAutoShrinkBox() { m_AutoShrinkBox = TRUE; }

    void SetCyclicAnimStyle(S32 i_Style) { m_CyclicAnimStyle = i_Style; }

    void SetLinkedButtonId(S32 i_ButtonId) { m_LinkedButtonId = i_ButtonId; }

    void SetBitmapStyle(S32 i_Style) { m_BitmapStyle = i_Style; }

    void SetBitmapColorStyle(S32 i_Style) { m_BitmapColorStyle = i_Style; }

    void SetUserValue1(S32 i_Value) { m_UserValue1 = i_Value; }

    void SetUserValue2(S32 i_Value) { m_UserValue2 = i_Value; }

    void SetAddText(Char* i_Text);
    void SetPosAndSize(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY);
    void SetTextId(S32 i_TextId, S32 i_Align);
    void Remove();

private:
    S32 m_Id;
    U8 m_Unk_0x004[168];
    Bool m_Selectable;
    Bool m_SelectableWithMouse;
    Bool m_SelectableWithPad;
    Bool m_Selected;
    Bool m_Available;
    Bool m_Owned;
    Bool m_Hidden;
    Bool m_Pushed;
    Bool m_Blink;
    U8 m_Pad_0x0b5[3];
    Float m_BlinkOffTime;
    Float m_BlinkOnTime;
    U8 m_Unk_0x0c0[8];
    Bool m_FullScreen;
    U8 m_Unk_0x0c9[12];
    Bool m_CyclicAnimated;
    U8 m_Unk_0x0d6[26];
    Bool m_StableY;
    U8 m_Pad_0x0f1[3];
    S32 m_Page;
    U8 m_Unk_0x0f8[8];
    S32 m_TextId;
    S32 m_Unk_0x104;
    Char* m_AddText;
    S32 m_TextAlign;
    Bool m_TextVertAlign;
    U8 m_Pad_0x111[3];
    S32 m_TextStyle;
    U8 m_Unk_0x118[12];
    S32 m_DescButtonId;
    S32 m_PictDescButtonId;
    U8 m_Unk_0x12c[148];
    S32 m_LinkedButtonId;
    S32 m_BoxStyle;
    Bool m_AutoShrinkBox;
    U8 m_Pad_0x1c9[3];
    S32 m_SurroundingBitmapsStyle;
    U8 m_Unk_0x1d0[4];
    S32 m_BitmapStyle;
    S32 m_BitmapColorStyle;
    U8 m_Unk_0x1dc[152];
    S32 m_CyclicAnimStyle;
    U8 m_Unk_0x278[4];
    S32 m_UserValue1;
    S32 m_UserValue2;
    U8 m_Unk_0x284[8];
};

#endif // _BUTTON_G_H_
