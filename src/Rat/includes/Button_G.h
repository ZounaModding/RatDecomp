#ifndef _BUTTON_G_H_
#define _BUTTON_G_H_
#include "Types_Z.h"

class styleBitmap;

class Button_G {
public:
    void SetBlink(Bool i_Blink, Float i_OnTime, Float i_OffTime);
    void SetSelectable(Bool i_Selectable);
    void SetSelectableWithMouse(Bool i_Selectable);
    void SetHidden(Bool i_Hidden);
    void SetAvailable(Bool i_Available);
    void SetSurroundingBitmapsStyle(S32 i_Style);
    void SetFullScreen(Bool i_FullScreen);
    void SetStableY(Bool i_StableY);
    void SetTextVertAlign(Bool i_Align);
    void SetBoxStyle(S32 i_Style);
    void SetText(S32 i_TextId, S32 i_Align);
    void SetTextStyle(S32 i_Style);
    void SetDescTextId(S32 i_TextId);
    void SetDescButtonId(S32 i_ButtonId);
    void SetPictDesc(styleBitmap& i_Style);
    void SetPictDescButtonId(S32 i_ButtonId);
    void SetAutoShrinkBox();
    void SetCyclicAnimStyle(S32 i_Style);
};

#endif // _BUTTON_G_H_
