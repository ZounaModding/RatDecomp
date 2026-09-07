#include "MenuParser.h"
#include "Color_Z.h"
#include "Console_Z.h"
#include "Button_G.h"
#include "Dialog_G.h"
#include "Frame_G.h"
#include "MenuManager_G.h"
#include "Name_Z.h"
#include "Pack_Z.h"
#include "Program_Z.h"
#include "ScriptManager_G.h"
#include "Sys_Z.h"
#include "Memory_Z.h"
#include "Assert_Z.h"
#include <stdlib.h>
#include <string.h>

extern Name_Z BoxNames[34];
extern Name_Z DialogButtonNames[775];
extern Name_Z ButtonNames[775];
extern const Color COLOR_WHITE;

class Dialog_G;
class Frame_G;

U32 currentPlafeforme;
U32 menuPlafeforme;
Dialog_G* currentDialog;
Button_G* currentButton;
Frame_G* currentFrame;
MenuManager_G* currentMenuManager;

void RegisterMenuCommand() {
    gData.Cons->AddCommand("MENUSTyleTextStructDim", MENUSTyleTextStructDim, "No Comment");
    gData.Cons->AddCommand("MENUButtonNotAvailable", MENUButtonNotAvailable, "No Comment");
    gData.Cons->AddCommand("MENUButtonFullScreen", MENUButtonFullScreen, "No Comment");
    gData.Cons->AddCommand("MENUFrame", MENUFrame, "No Comment");
    gData.Cons->AddCommand("EndMENUFrame", EndMENUFrame, "No Comment");
    gData.Cons->AddCommand("MENUButtonStableY", MENUButtonStableY, "No Comment");
    gData.Cons->AddCommand("MENUGhostButton", MENUGhostButton, "No Comment");
    gData.Cons->AddCommand("MENUButtonSelectableWithMouse", MENUButtonSelectableWithMouse, "No Comment");
}

Bool StartMENUDefinition() {
    if (!gScriptMgr->m_PackedMenuCommandBuffer) {
        U32 l_Size = gData.Cons->GetCurrentInterpSize();
        Pack_Z l_Pack(l_Size);
        l_Pack.Pack((U8*)gData.Cons->GetCurrentInterpBuffer(), l_Size, 0);
        gScriptMgr->m_PackedMenuCommandBuffer = AllocAlignL_Z(l_Pack.GetPackedSize(), 0x3c, 4);
        Sys_Z::MemCpyFrom(gScriptMgr->m_PackedMenuCommandBuffer, l_Pack.GetPackedData(), l_Pack.GetPackedSize());
        gScriptMgr->m_PackedMenuCommandBufferSize = l_Pack.GetPackedSize();
    }
    menuPlafeforme = MENU_PLATFORM_ALL;
    currentMenuManager->ResetStyles();
    currentPlafeforme = MENU_PLATFORM_GC;
    return TRUE;
}

Bool MENUDialog() {
    return FALSE;
}

Bool MENUButton() {
    return FALSE;
}

Bool RemoveAllDialogs() {
    return FALSE;
}

Bool EndMENURessourceParsing() {
    currentMenuManager->InitVoices();
    currentMenuManager->SetRessourcesParsed(TRUE);
    return TRUE;
}

Bool EndMENUDialog() {
    return FALSE;
}

Bool MENUDEBug() {
    return FALSE;
}

Bool MENUUpdate() {
    return FALSE;
}

Bool MENUPlatform() {
    if (gData.Cons->GetNbParam() == 1) {
        menuPlafeforme = MENU_PLATFORM_ALL;
    }
    else {
        S32 l_Index;
        S32 l_Count = gData.Cons->GetNbParam() - 1;
        menuPlafeforme = 0;
        for (l_Index = 0; l_Index < l_Count; ++l_Index) {
            const Char* l_Platform = gData.Cons->GetParamStr(l_Index + 1);
            if (!stricmp(l_Platform, "PC")) {
                menuPlafeforme |= MENU_PLATFORM_PC;
            }
            else if (!stricmp(l_Platform, "PS2")) {
                menuPlafeforme |= MENU_PLATFORM_PS2;
            }
            else if (!stricmp(l_Platform, "GC")) {
                menuPlafeforme |= MENU_PLATFORM_GC;
            }
            else if (!stricmp(l_Platform, "REVO")) {
                menuPlafeforme |= MENU_PLATFORM_WII;
            }
            else if (!stricmp(l_Platform, "MAC")) {
                menuPlafeforme |= MENU_PLATFORM_MAC;
            }
            else if (!stricmp(l_Platform, "XBOX")) {
                menuPlafeforme |= MENU_PLATFORM_XBOX;
            }
            else if (!stricmp(l_Platform, "INTERFACE_DEMO_PS2")) {
                menuPlafeforme |= MENU_PLATFORM_INTERFACE_DEMO_PS2;
            }
        }
    }
    return TRUE;
}

Bool MENUButtonBlink() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetBlink(TRUE, 0.5f, 0.2f);
    return TRUE;
}

Bool MENUButtonSelectable() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetSelectable(TRUE);
    return TRUE;
}

Bool MENUButtonSelectableWithMouse() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetSelectableWithMouse(TRUE);
    return TRUE;
}

Bool MENUButtonHidden() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetHidden(TRUE);
    return TRUE;
}

Bool MENUButtonNotAvailable() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetAvailable(FALSE);
    return TRUE;
}

Bool MENUButtonBitmap() {
    return FALSE;
}

Bool MENUButtonSurroundingBitmaps() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    SurroundingBitmapsStyle* l_Style = currentMenuManager->GetSurroundingBitmapsStyleByName(Name_Z(gData.Cons->GetParamStr(1)));
    if (!l_Style) {
        gData.Cons->GetParamStr(1);
        ASSERTL_Z(FALSE, "SurroundingBitmapsStyle inconnu, voir console", 0x180);
    }
    currentButton->SetSurroundingBitmapsStyle(currentMenuManager->GetIdSurroundingBitmapsStyle(l_Style));
    return TRUE;
}

Bool MENUButtonFullScreen() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 1) {
        return FALSE;
    }
    currentButton->SetFullScreen(TRUE);
    return TRUE;
}

Bool MENUButtonStableY() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 1) {
        return FALSE;
    }
    currentButton->SetStableY(TRUE);
    return TRUE;
}

Bool MENUButtonBox() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 2) {
        return FALSE;
    }
    styleBox* l_Style = currentMenuManager->GetStyleBoxByName(Name_Z(gData.Cons->GetParamStr(1)));
    if (!l_Style) {
        gData.Cons->GetParamStr(1);
        ASSERTL_Z(FALSE, "Style Box inconnu, voir console", 0x1a9);
    }
    currentButton->SetBoxStyle(currentMenuManager->GetIdStyleBox(l_Style));
    return TRUE;
}

Bool MENUButtonAlignVert() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 2) {
        return FALSE;
    }
    if (!stricmp(gData.Cons->GetParamStr(1), "TRUE")) {
        currentButton->SetTextVertAlign(TRUE);
    }
    else {
        currentButton->SetTextVertAlign(FALSE);
    }
    return TRUE;
}

Bool MENUButtonText() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 4) {
        return FALSE;
    }
    const Char* l_AlignName = gData.Cons->GetParamStr(3);
    S32 l_Align = 1;
    if (!stricmp(l_AlignName, "ALIGN_LEFT")) {
        l_Align = 0;
    }
    else if (!stricmp(l_AlignName, "ALIGN_RIGHT")) {
        l_Align = 2;
    }
    styleText* l_Style = currentMenuManager->GetStyleTextByName(Name_Z(gData.Cons->GetParamStr(2)));
    if (!l_Style) {
        gData.Cons->GetParamStr(2);
        ASSERTL_Z(FALSE, "Style Text inconnu, voir console", 0x1d6);
    }
    currentButton->SetText(atoi(gData.Cons->GetParamStr(1)), l_Align);
    currentButton->SetTextStyle(currentMenuManager->GetIdStyleText(l_Style));
    return TRUE;
}

Bool MENUButtonDesc() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 3) {
        return FALSE;
    }
    currentButton->SetDescTextId(atoi(gData.Cons->GetParamStr(2)));
    Name_Z l_Name(gData.Cons->GetParamStr(1));
    S32 l_ButtonId;
    for (l_ButtonId = 0; l_ButtonId < 775; ++l_ButtonId) {
        if (l_Name == (const Name_Z&)ButtonNames[l_ButtonId] && currentDialog->GetNameDialog() == (const Name_Z&)DialogButtonNames[l_ButtonId]) {
            break;
        }
    }
    currentButton->SetDescButtonId(l_ButtonId);
    return TRUE;
}

Bool MENUButtonPictDesc() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 3) {
        return FALSE;
    }
    styleBitmap* l_Style = currentMenuManager->GetStyleBitmapByName(Name_Z(gData.Cons->GetParamStr(2)));
    if (!l_Style) {
        return FALSE;
    }
    currentButton->SetPictDesc(*l_Style);
    Name_Z l_Name(gData.Cons->GetParamStr(1));
    S32 l_ButtonId;
    for (l_ButtonId = 0; l_ButtonId < 775; ++l_ButtonId) {
        if (l_Name == (const Name_Z&)ButtonNames[l_ButtonId] && currentDialog->GetNameDialog() == (const Name_Z&)DialogButtonNames[l_ButtonId]) {
            break;
        }
    }
    currentButton->SetPictDescButtonId(l_ButtonId);
    return TRUE;
}

S32 GetBoxFromtText(Char* i_Text) {
    Name_Z l_Name = Name_Z(i_Text);
    for (S32 l_Index = 0; l_Index < 34; ++l_Index) {
        const Name_Z& l_BoxName = BoxNames[l_Index];
        if (l_Name == l_BoxName) {
            return l_Index;
        }
    }
    return 0;
}

void GetColorFromText(Color& o_Color, Char* i_Text) {
    o_Color = COLOR_WHITE;
    o_Color.r = atof(std::strstr(i_Text, "r") + 1);
    o_Color.g = atof(std::strstr(i_Text, "g") + 1);
    o_Color.b = atof(std::strstr(i_Text, "b") + 1);
    o_Color.a = atof(std::strstr(i_Text, "a") + 1);
    o_Color *= 1.0f / 512.0f;
    o_Color.a /= 255.0f;
}

Bool MENUStyleText() {
    return FALSE;
}

Bool MENUStyleTextScroll() {
    return FALSE;
}

Bool MENUStyleBox() {
    return FALSE;
}

Bool MENUButtonAnimateCyclicTRUE() {
    return FALSE;
}

Bool MENUSTyleTextStruct() {
    return FALSE;
}

Bool MENUStyleTextEqual() {
    return FALSE;
}

Bool MENUStyleBoxCoinScale() {
    return FALSE;
}

Bool MENUStyleBitmapColor() {
    return FALSE;
}

Bool MENUStyleBitmap() {
    return FALSE;
}

Bool MENUStyleBitmapDim() {
    return FALSE;
}

Bool MENUSurroundingBitMaps() {
    return FALSE;
}

Bool MenuButtonMAJ() {
    return FALSE;
}

Bool MENUBoxAutoShrink() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    currentButton->SetAutoShrinkBox();
    return TRUE;
}

Bool MENUStyleCyclicAnimation() {
    return FALSE;
}

Bool MENUStateAnimation() {
    if (!(currentPlafeforme & menuPlafeforme)) {
        return TRUE;
    }
    if (gData.Cons->GetNbParam() != 2) {
        return FALSE;
    }
    S32 l_Style = currentMenuManager->GetCyclicAnimStyleIdByName(Name_Z(gData.Cons->GetParamStr(1)));
    currentButton->SetCyclicAnimStyle(l_Style);
    return TRUE;
}
