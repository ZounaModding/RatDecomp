#include "Dialogs_System.h"
#include "Fonts_Z.h"
#include "ScriptManager_G.h"

void Dialog_MSGBOX::InitDialog() {
    m_MessageButtonId = DIALOG_MSGBOX_MESSAGE_BUTTON_ID;
    m_YesButtonId = DIALOG_MSGBOX_YES_BUTTON_ID;
    m_NoButtonId = DIALOG_MSGBOX_NO_BUTTON_ID;
    m_ContinueButtonId = DIALOG_MSGBOX_CONTINUE_BUTTON_ID;
}

void Dialog_SHOPMSGBOX::InitDialog() {
    m_MessageButtonId = DIALOG_SHOPMSGBOX_MESSAGE_BUTTON_ID;
    m_YesButtonId = DIALOG_SHOPMSGBOX_YES_BUTTON_ID;
    m_NoButtonId = DIALOG_SHOPMSGBOX_NO_BUTTON_ID;
    m_ContinueButtonId = DIALOG_SHOPMSGBOX_CONTINUE_BUTTON_ID;
}

void Dialog_ALERT::InitDialog() {
}

void Dialog_INPUTSTRING::SetupWesternAlphabet() {
    Float l_PosY = m_KeyPosY;
    S32 l_RemainingLines = 53 / m_KeysPerLine;
    S32 l_KeyIndex = 0;
    Float l_PosX = m_KeyPosX;
    Char l_Text[8];
    l_Text[1] = 0;

    for (S32 l_Character = 'A'; (U8)l_Character < 'Z' + 1; l_Character++) {
        Button_G* l_Button = PushButton();
        if ((U32)(U8)l_Character == 'A') {
            m_SelectedButtonId = l_Button->GetId();
        }
        l_Text[0] = l_Character;
        l_Button->SetAddText(l_Text);
        l_Button->SetSelectable(TRUE);
        l_Button->SetTextStyle(m_TextStyle);
        l_Button->SetBoxStyle(m_BoxStyle);
        l_Button->SetPosAndSize(l_PosX, l_PosY, m_KeySizeX, m_KeySizeY);
        if (l_KeyIndex % m_KeysPerLine == m_KeysPerLine - 1) {
            l_RemainingLines--;
            l_PosX = m_KeyPosX;
            l_PosY += m_KeySizeY + m_KeySpaceY;
        }
        else {
            l_PosX += m_KeySizeX + m_KeySpaceX;
        }
        l_KeyIndex++;
    }

    Button_G* l_Button = PushButton();
    l_Text[0] = '-';
    l_Button->SetAddText(l_Text);
    l_Button->SetSelectable(TRUE);
    l_Button->SetTextStyle(m_TextStyle);
    l_Button->SetBoxStyle(m_BoxStyle);
    l_Button->SetPosAndSize(l_PosX, l_PosY, m_KeySizeX, m_KeySizeY);
    if (l_KeyIndex % m_KeysPerLine == m_KeysPerLine - 1) {
        l_RemainingLines--;
        l_PosX = m_KeyPosX;
        l_PosY += m_KeySizeY + m_KeySpaceY;
    }
    else {
        l_PosX += m_KeySizeX + m_KeySpaceX;
    }
    l_KeyIndex++;

    for (S32 l_Character = 'a'; (U8)l_Character < 'z' + 1; l_Character++) {
        Button_G* l_Button = PushButton();
        l_Text[0] = l_Character;
        l_Button->SetAddText(l_Text);
        l_Button->SetSelectable(TRUE);
        l_Button->SetTextStyle(m_TextStyle);
        l_Button->SetBoxStyle(m_BoxStyle);
        l_Button->SetPosAndSize(l_PosX, l_PosY, m_KeySizeX, m_KeySizeY);
        if (l_KeyIndex % m_KeysPerLine == m_KeysPerLine - 1) {
            l_RemainingLines--;
            if (l_RemainingLines == 0) {
                Float l_Half = 0.5f;
                S32 l_RemainingKeys = 53 - l_KeyIndex;
                l_RemainingKeys--;
                l_PosY += m_KeySizeY + m_KeySpaceY;
                l_PosX = (m_KeysPerLine - l_RemainingKeys) * (m_KeySizeX + m_KeySpaceX) * l_Half + m_KeyPosX;
            }
            else {
                l_PosX = m_KeyPosX;
                l_PosY += m_KeySizeY + m_KeySpaceY;
            }
        }
        else {
            l_PosX += m_KeySizeX + m_KeySpaceX;
        }
        l_KeyIndex++;
    }
}

void Dialog_INPUTSTRING::InitDialog() {
    Dialog_MC_Base::InitDialog();
    SetupWesternAlphabet();
}

void Dialog_INPUTSTRING::Reset() {
    m_InputString.Empty();
    m_InputString.StrCpy("");
    UpdateEditButtons();
    GetButton(m_EditButtonId)->SetAddText(TT(m_TitleTextId));
    m_UnkS32_0x118 = 0;
}

void Dialog_INPUTSTRING::MenuActivated() {
    if (m_SelectedButtonId != -1) {
        SelectButton(m_SelectedButtonId);
    }
    Reset();
}

void Dialog_INPUTSTRING::SetDimensions(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY, Float i_SpaceX, Float i_SpaceY) {
    m_KeyPosX = i_PosX;
    m_KeyPosY = i_PosY;
    m_KeySpaceX = i_SpaceX;
    m_KeySpaceY = i_SpaceY;
    m_KeySizeX = i_SizeX;
    m_KeySizeY = i_SizeY;
}

void Dialog_INPUTSTRING::SetStyles(S32 i_TextStyle, S32 i_BoxStyle) {
    m_TextStyle = i_TextStyle;
    m_BoxStyle = i_BoxStyle;
}

void Dialog_INPUTSTRING::UpdateEditButtons() {
    Char l_Text[16];
    l_Text[1] = 0;
    for (S32 i = 0; i < m_EditButtonIds.GetSize(); i++) {
        if (m_InputString.StrLen() > i) {
            l_Text[0] = m_InputString.Get()[i];
            Button_G* l_Button = GetButton(m_EditButtonIds[i]);
            l_Button->SetAddText(l_Text);
        }
        else {
            Button_G* l_Button = GetButton(m_EditButtonIds[i]);
            l_Button->SetText(0, 1);
            l_Button = GetButton(m_EditButtonIds[i]);
            l_Button->SetAddText("");
        }
    }
}

void Dialog_INPUTSTRINGJPN::InitDialog() {
    m_ResetKeyboardAfterTransition = FALSE;
    if (gScriptMgr->GetCTFGameMgr().GetCurrentLanguage() != LANG_JAPANESE_Z) {
        Dialog_INPUTSTRING::InitDialog();
    }
    else {
        m_Keyboard = 0;
        GetButton(m_LatinButtonId)->SetPushed(TRUE);
        m_FirstKeyboardButtonId = -1;
        m_LastKeyboardButtonId = -1;
    }
}

S32 Dialog_INPUTSTRINGJPN::StrLenJpn(Char* i_String) {
    S32 l_Length = 0;
    while (*i_String != 0) {
        i_String += GetUTF8CharBytes(i_String);
        l_Length++;
    }
    return l_Length;
}

void Dialog_INPUTSTRINGJPN::GetCharacter(Char* o_Character, Char* i_String, U32 i_Index) {
    S32 l_CharacterSize = 0;
    for (U32 i = 0; i <= i_Index; i++) {
        i_String += l_CharacterSize;
        l_CharacterSize = GetUTF8CharBytes(i_String);
    }
    memcpy(o_Character, i_String, l_CharacterSize);
    o_Character[l_CharacterSize] = 0;
}

void Dialog_INPUTSTRINGJPN::UpdateEditButtons() {
    if (gScriptMgr->GetCTFGameMgr().GetCurrentLanguage() != LANG_JAPANESE_Z) {
        Dialog_INPUTSTRING::UpdateEditButtons();
    }
    else {
        Char l_Text[16];
        for (S32 i = 0; i < m_EditButtonIds.GetSize(); i++) {
            if (StrLenJpn(m_InputString.Get()) > i) {
                GetCharacter(l_Text, m_InputString.Get(), i);
                Button_G* l_Button = GetButton(m_EditButtonIds[i]);
                l_Button->SetAddText(l_Text);
            }
            else {
                Button_G* l_Button = GetButton(m_EditButtonIds[i]);
                l_Button->SetText(0, 1);
                l_Button = GetButton(m_EditButtonIds[i]);
                l_Button->SetAddText("");
            }
        }
    }
}

void Dialog_INPUTSTRINGJPN::ResetKeyBoard() {
    if (m_FirstKeyboardButtonId != -1 && m_LastKeyboardButtonId != -1) {
        S32 l_ButtonIndex = 0;
        while (m_Buttons[l_ButtonIndex].GetId() != m_FirstKeyboardButtonId) {
            if (l_ButtonIndex >= m_Buttons.GetSize() - 1) {
                return;
            }
            l_ButtonIndex++;
        }

        if (l_ButtonIndex != m_Buttons.GetSize() - 1) {
            S32 l_RemoveIndex = l_ButtonIndex;
            while (m_Buttons[l_RemoveIndex].GetId() != m_LastKeyboardButtonId) {
                m_Buttons[l_RemoveIndex].Remove();
                m_Buttons.Remove(l_RemoveIndex);
            }
            if (l_ButtonIndex != m_Buttons.GetSize()) {
                m_Buttons[l_ButtonIndex].Remove();
                m_Buttons.Remove(l_ButtonIndex);
            }
        }
    }
}

void Dialog_INPUTSTRINGJPN::Update(Float i_DeltaTime) {
    if (gScriptMgr->GetCTFGameMgr().GetCurrentLanguage() != LANG_JAPANESE_Z) {
        Dialog_G::Update(i_DeltaTime);
    }
    else {
        Dialog_G::Update(i_DeltaTime);
        if (m_ResetKeyboardAfterTransition == TRUE && m_State == DIAL_STATE_0) {
            m_ResetKeyboardAfterTransition = FALSE;
            ResetKeyBoard();
            ChangeState(DIAL_STATE_1);
        }
    }
}
