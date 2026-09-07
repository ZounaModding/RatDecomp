#include "GameMgr_G.h"
#include "Language_Z.h"

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
}

void LanguageHandleSTR(Char* i_Text) { }

Bool LoadINPUT() {
    return FALSE;
}

Bool RESETTextAdd() {
    return FALSE;
}

Bool RemapTextAdd() {
    return FALSE;
}

Bool InputDefAdd() {
    return FALSE;
}
