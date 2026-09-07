#include "InputDef_G.h"
#include "GameMgr_G.h"
#include "GCMain_Z.h"
#include "Console_Z.h"
#include "InputEngine_Z.h"
#include "Language_Z.h"
#include "ScriptManager_G.h"

static Name_Z ActionName[] = {
    Name_Z::GetID("LEFT_ACTION"),
    Name_Z::GetID("RIGHT_ACTION"),
    Name_Z::GetID("UP_ACTION"),
    Name_Z::GetID("DOWN_ACTION"),
    Name_Z::GetID("CAMERA_LEFT_ACTION"),
    Name_Z::GetID("CAMERA_RIGHT_ACTION"),
    Name_Z::GetID("CAMERA_UP_ACTION"),
    Name_Z::GetID("CAMERA_DOWN_ACTION"),
    Name_Z::GetID("CAMERA_RESET"),
    Name_Z::GetID("PAUSE_MODE"),
    Name_Z::GetID("INFOS"),
    Name_Z::GetID("CONTEXT"),
    Name_Z::GetID("JUMP"),
    Name_Z::GetID("FIGHT"),
    Name_Z::GetID("DASH"),
    Name_Z::GetID("SEARCH"),
    Name_Z::GetID("HUD"),
    Name_Z::GetID("NEXTCONTEXT1"),
    Name_Z::GetID("MINIGAME_0"),
    Name_Z::GetID("MINIGAME_1"),
    Name_Z::GetID("MINIGAME_2"),
    Name_Z::GetID("MINIGAME_3"),
    Name_Z::GetID("MINIGAME_4"),
    Name_Z::GetID("MINIGAME_5"),
    Name_Z::GetID("MINIGAME_6"),
    Name_Z::GetID("MINIGAME_7"),
    Name_Z::GetID("WII_RACC_X"),
    Name_Z::GetID("WII_RACC_Y"),
    Name_Z::GetID("WII_RACC_Z"),
    Name_Z::GetID("WII_RPOINT_X"),
    Name_Z::GetID("WII_RPOINT_Y"),
    Name_Z::GetID("WII_RDIST"),
    Name_Z::GetID("WII_LACC_X"),
    Name_Z::GetID("WII_LACC_Y"),
    Name_Z::GetID("WII_LACC_Z"),
    Name_Z::GetID("NEXTCONTEXT2"),
    Name_Z::GetID("MENU_UP"),
    Name_Z::GetID("MENU_DOWN"),
    Name_Z::GetID("MENU_RIGHT"),
    Name_Z::GetID("MENU_LEFT"),
    Name_Z::GetID("MENU_VALID"),
    Name_Z::GetID("MENU_CANCEL"),
    Name_Z::GetID("MENU_BACK"),
    Name_Z::GetID("MENU_VALI2"),
    Name_Z::GetID("MENU_L1"),
    Name_Z::GetID("MENU_R1"),
    Name_Z::GetID("MENU_START"),
    Name_Z::GetID("MENU_SELECT"),
    Name_Z::GetID("END_CONTEXT"),
};

U32 CInputDef_G::GetActionID(Char* i_ActionName) {
    S32 l_ActionNameIdx = 0;
    S32 l_ActionId = 0;
    while (ActionName[l_ActionNameIdx].m_ID != Name_Z::GetID("END_CONTEXT")) {
        if (ActionName[l_ActionNameIdx].m_ID == Name_Z::GetID("NEXTCONTEXT1") || ActionName[l_ActionNameIdx].m_ID == Name_Z::GetID("NEXTCONTEXT2") || ActionName[l_ActionNameIdx].m_ID == Name_Z::GetID("NEXTCONTEXT3")) {
            ++l_ActionNameIdx;
            l_ActionId = (l_ActionId / 32) * 32 + 32;
        }
        S32 l_NameId = i_ActionName ? Name_Z::GetID(i_ActionName) : 0;
        if (ActionName[l_ActionNameIdx] == Name_Z(l_NameId)) {
            return l_ActionId;
        }
        ++l_ActionNameIdx;
        ++l_ActionId;
    }
    return -1;
}

void CInputDef_G::InitInputs() {
    m_ActionContextIdx = gData.InputMgr->CreateActionContext(TRUE);
    InputActionContext_Z& l_ActionContext = gData.InputMgr->m_RegisteredInputActionContexts[m_ActionContextIdx];

    ADD_INPUT_ACTION(0, 0, 0.2f);
    ADD_INPUT_ACTION(1, 1, 0.2f);
    ADD_INPUT_ACTION(2, 2, 0.2f);
    ADD_INPUT_ACTION(3, 3, 0.2f);
    ADD_INPUT_ACTION(4, 4, 0.2f);
    ADD_INPUT_ACTION(5, 5, 0.2f);
    ADD_INPUT_ACTION(6, 6, 0.2f);
    ADD_INPUT_ACTION(7, 7, 0.2f);
    ADD_INPUT_ACTION(8, 8, 0.0f);
    ADD_INPUT_ACTION(9, 9, 0.0f);
    ADD_INPUT_ACTION(10, 10, 0.0f);
    ADD_INPUT_ACTION(11, 11, 0.0f);
    ADD_INPUT_ACTION(12, 12, 0.0f);
    ADD_INPUT_ACTION(13, 13, 0.0f);
    ADD_INPUT_ACTION(14, 14, 0.0f);
    ADD_INPUT_ACTION(15, 15, 0.0f);
    ADD_INPUT_ACTION(16, 16, 0.0f);
    ADD_INPUT_ACTION(17, 17, 0.0f);
    ADD_INPUT_ACTION(18, 32, 0.0f);
    ADD_INPUT_ACTION(19, 33, 0.0f);
    ADD_INPUT_ACTION(20, 34, 0.0f);
    ADD_INPUT_ACTION(21, 35, 0.0f);
    ADD_INPUT_ACTION(22, 36, 0.0f);
    ADD_INPUT_ACTION(23, 37, 0.0f);
    ADD_INPUT_ACTION(24, 38, 0.0f);
    ADD_INPUT_ACTION(25, 39, 0.0f);
    ADD_INPUT_ACTION(26, 40, 0.0f);
    ADD_INPUT_ACTION(27, 41, 0.0f);
    ADD_INPUT_ACTION(28, 42, 0.0f);
    ADD_INPUT_ACTION(29, 43, 0.0f);
    ADD_INPUT_ACTION(30, 44, 0.0f);
    ADD_INPUT_ACTION(31, 45, 0.0f);
    ADD_INPUT_ACTION(32, 46, 0.0f);
    ADD_INPUT_ACTION(33, 47, 0.0f);
    ADD_INPUT_ACTION(34, 48, 0.0f);
    ADD_INPUT_ACTION(35, 49, 0.0f);
    ADD_INPUT_ACTION(36, 64, 0.0f);
    ADD_INPUT_ACTION(37, 65, 0.0f);
    ADD_INPUT_ACTION(38, 66, 0.0f);
    ADD_INPUT_ACTION(39, 67, 0.0f);
    ADD_INPUT_ACTION(40, 68, 0.0f);
    ADD_INPUT_ACTION(41, 69, 0.0f);
    ADD_INPUT_ACTION(42, 70, 0.0f);
    ADD_INPUT_ACTION(43, 71, 0.0f);
    ADD_INPUT_ACTION(44, 72, 0.0f);
    ADD_INPUT_ACTION(45, 73, 0.0f);
    ADD_INPUT_ACTION(46, 74, 0.0f);
    ADD_INPUT_ACTION(47, 75, 0.0f);
    ADD_INPUT_ACTION(48, 76, 0.0f);

#undef ADD_INPUT_ACTION

    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[37]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[36]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[39]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[38]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[42]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[40]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[14]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[15]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[18]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[19]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[20]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[21]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[22]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[23]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[24]).m_ActionId, TRUE);
    gData.InputMgr->SetControlMode(l_ActionContext.GetAction(m_Actions[25]).m_ActionId, TRUE);
}

Bool LanguageHandleSTR(Char* i_Text) {
    Char l_Replacement[512];
    Char l_Buffer[1024];

    for (S32 i = 0; i < gScriptMgr->GetCTFGameMgr().GetRemapTextInfos().GetSize(); i++) {
        RemapTextInfo& l_RemapTextInfo = gScriptMgr->GetCTFGameMgr().GetRemapTextInfos()[i];
        Char* l_Position;
        while ((l_Position = strstr(i_Text, l_RemapTextInfo.m_RemapText)) != NULL) {
            if (l_RemapTextInfo.m_TrTextId > 0) {
                strcpy(l_Replacement, TT(l_RemapTextInfo.m_TrTextId));
            }
            else {
                l_Replacement[0] = l_RemapTextInfo.m_InputButtonNameIndex;
                l_Replacement[1] = 0;
            }

            S32 l_PrefixLength = l_Position - i_Text;
            strncpy(l_Buffer, i_Text, l_PrefixLength);
            l_Buffer[l_PrefixLength] = 0;
            strcat(l_Buffer, l_Replacement);
            strcat(l_Buffer, i_Text + l_PrefixLength + strlen(l_RemapTextInfo.m_RemapText));
            strcpy(i_Text, l_Buffer);
        }
    }
    return FALSE;
}

Bool LoadINPUT() {
    String_Z<256> l_Command;
    Char l_InputName[256];

    strcpy(l_InputName, "input");
    if (gData.Cons->GetNbParam() > 1) {
        l_Command.Sprintf("BSource input\\%s%s.%s", l_InputName, gData.Cons->GetStrParam(1), "GC");
    }
    else {
        l_Command.Sprintf("BSource input\\%s.%s", l_InputName, "GC");
    }
    gData.Cons->InterpCommand(l_Command, 0);
    return TRUE;
}

Bool RESETTextAdd() {
    gScriptMgr->GetCTFGameMgr().GetRemapTextInfos().Empty();
    gScriptMgr->GetCTFGameMgr().GetRemapTextInfos().Minimize();
    return TRUE;
}

Bool RemapTextAdd() {
    Char* l_RemapText = gData.Cons->GetStrParam(1);
    Char* l_Text = gData.Cons->GetStrParam(2);
    RemapTextInfo l_RemapTextInfo;
    strcpy(l_RemapTextInfo.m_RemapText, l_RemapText);
    l_RemapTextInfo.m_InputButtonNameIndex = GetCharIdFromText(l_Text);
    l_RemapTextInfo.m_TrTextId = atoi(l_Text);
    if (l_RemapTextInfo.m_TrTextId <= 0) {
        l_RemapTextInfo.m_TrTextId = -1;
    }
    gScriptMgr->GetCTFGameMgr().GetRemapTextInfos().Add(l_RemapTextInfo);
    return TRUE;
}

Bool InputDefAdd() {
    Char* l_ActionName = gData.Cons->GetStrParam(1);
    Char l_PrimaryControl[256];
    Char l_SecondaryControl[256];

    l_PrimaryControl[0] = 0;
    l_SecondaryControl[0] = 0;
    if (gData.Cons->GetNbParam() > 2) {
        strcpy(l_PrimaryControl, gData.Cons->GetStrParam(2));
    }
    if (gData.Cons->GetNbParam() > 3) {
        strcpy(l_SecondaryControl, gData.Cons->GetStrParam(3));
    }

    S32 l_ActionId = gScriptMgr->GetCTFGameMgr().GetInputDef()->GetActionID(l_ActionName);
    if (l_ActionId >= 0) {
        InputPlatForm_Z* l_InputManager = gData.InputMgr;
        l_InputManager->SetControl(l_ActionId, Name_Z(Name_Z::GetID(l_PrimaryControl)), Name_Z(Name_Z::GetID(l_SecondaryControl)));
    }
    return TRUE;
}
