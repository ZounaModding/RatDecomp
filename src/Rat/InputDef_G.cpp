#include "GameMgr_G.h"
#include "Language_Z.h"

extern U32 ActionName[];

U32 CInputDef_G::GetActionID(Char* i_ActionName) {
    S32 l_ActionNameIdx = 0;
    S32 l_ActionId = 0;
    while (ActionName[l_ActionNameIdx] != Name_Z::GetID("END_CONTEXT")) {
        if (ActionName[l_ActionNameIdx] == Name_Z::GetID("NEXTCONTEXT1") || ActionName[l_ActionNameIdx] == Name_Z::GetID("NEXTCONTEXT2") || ActionName[l_ActionNameIdx] == Name_Z::GetID("NEXTCONTEXT3")) {
            ++l_ActionNameIdx;
            l_ActionId = (l_ActionId / 32) * 32 + 32;
        }
        U32 l_NameId = i_ActionName ? Name_Z::GetID(i_ActionName) : 0;
        if (l_NameId == ActionName[l_ActionNameIdx]) {
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
