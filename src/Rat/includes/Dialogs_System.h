#ifndef _DIALOGS_SYSTEM_H_
#define _DIALOGS_SYSTEM_H_
#include "Dialog_MC_Base.h"
#include "String_Z.h"

#define DIALOG_MSGBOX_MESSAGE_BUTTON_ID 0x2fa
#define DIALOG_MSGBOX_YES_BUTTON_ID 0x2fb
#define DIALOG_MSGBOX_NO_BUTTON_ID 0x2fc
#define DIALOG_MSGBOX_CONTINUE_BUTTON_ID 0x2fd
#define DIALOG_SHOPMSGBOX_MESSAGE_BUTTON_ID 0x300
#define DIALOG_SHOPMSGBOX_YES_BUTTON_ID 0x301
#define DIALOG_SHOPMSGBOX_NO_BUTTON_ID 0x302
#define DIALOG_SHOPMSGBOX_CONTINUE_BUTTON_ID 0x303

class Dialog_MSGBOX : public Dialog_G {
public:
    virtual void DispatchMessages();
    virtual void UpdateFocusedDialog(Float i_DeltaTime);
    virtual void InitDialog();
    virtual void BeforeDialog();

protected:
    S32 m_MessageButtonId;
    S32 m_YesButtonId;
    S32 m_NoButtonId;
    S32 m_ContinueButtonId;
};

class Dialog_SHOPMSGBOX : public Dialog_MSGBOX {
public:
    virtual void InitDialog();
    virtual void BeforeDialog();
};

class Dialog_ALERT : public Dialog_G {
public:
    virtual void DispatchMessages();
    virtual void InitDialog();
    virtual void BeforeDialog();
    virtual void AfterDialog();

private:
    Bool m_MouseWasHidden;
};

class Dialog_INPUTSTRING : public Dialog_MC_Base {
public:
    virtual void Remove();
    virtual void DispatchMessages();
    virtual void InitDialog();
    virtual void MenuActivated();
    virtual void BeforeDialog();
    virtual void UpdateEditButtons();
    virtual void CallBack_ValidOn(Button_G* i_Button);
    virtual void CallBack_Clear();
    virtual void SetInputString() = 0;
    virtual S32 IsWordNotAllowed();

    void Reset();
    void SetupWesternAlphabet();
    void SetDimensions(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY, Float i_SpaceX, Float i_SpaceY);
    void SetStyles(S32 i_TextStyle, S32 i_BoxStyle);

protected:
    S32 m_KeysPerLine;
    S32 m_BoxStyle;
    S32 m_TextStyle;
    Float m_KeyPosX;
    Float m_KeyPosY;
    Float m_KeySizeX;
    Float m_KeySizeY;
    Float m_KeySpaceX;
    Float m_KeySpaceY;
    S32 m_UnkS32_0x10c;
    S32 m_TitleTextId;
    S32 m_EditButtonId;
    S32 m_UnkS32_0x118;
    DynArray_Z<S32, 2> m_EditButtonIds;
    String_Z<256> m_InputString;
    S32 m_ClearButtonId;
    S32 m_ValidateButtonId;
};

class Dialog_INPUTSTRINGJPN : public Dialog_INPUTSTRING {
public:
    virtual void Update(Float i_DeltaTime);
    virtual void InitDialog();
    virtual void BeforeDialog();
    virtual void AfterDialog();
    virtual void UpdateEditButtons();
    virtual void CallBack_ValidOn(Button_G* i_Button);
    virtual void CallBack_Clear();

    void ResetKeyBoard();
    void SetupKeyBoard();
    S32 StrLenJpn(Char* i_String);
    void GetCharacter(Char* o_Character, Char* i_String, U32 i_Index);

private:
    S32 m_Keyboard;
    S32 m_LatinButtonId;
    S32 m_HiraganaButtonId;
    S32 m_KatakanaButtonId;
    S32 m_KanjiButtonId;
    S32 m_FirstKeyboardButtonId;
    S32 m_LastKeyboardButtonId;
    Bool m_ResetKeyboardAfterTransition;
};
#endif // _DIALOGS_SYSTEM_H_
