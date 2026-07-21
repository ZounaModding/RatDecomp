#include "ClassManager_Z.h"
#include "ABC_ScriptManager.h"
#include "StreamAgent_Z.h"

void ClassManager_Z::GetNameFromId(U32 i_ID, Name_Z& o_Name) {
    o_Name.m_ID = i_ID;
}

Bool ClassManager_Z::OpenBigFile(const Char* i_BfName, const Name_Z& i_StreamAgentClassName, Bool i_Unused) {
    StreamAgent_ZHdl* l_StrAgentHdl = gData.ScriptMgr->GetStreamAgent();

    if (!l_StrAgentHdl->IsValid()) {
        *l_StrAgentHdl = gData.ScriptMgr->SetStreamAgent(i_StreamAgentClassName);
    }
    else {
        Name_Z& l_ClassName = GetClassName(*l_StrAgentHdl);
        if (l_ClassName != i_StreamAgentClassName) {
            *l_StrAgentHdl = gData.ScriptMgr->SetStreamAgent(i_StreamAgentClassName);
        }
    }

    if (!l_StrAgentHdl->IsValid()) {
        *l_StrAgentHdl = gData.ScriptMgr->SetStreamAgent(Name_Z(Name_Z::GetID("StreamAgent_Z")));
    }

    String_Z<ARRAY_CHAR_MAX> l_BfPath;

    // Name file opening (does nothing in MASTER)
    l_BfPath.StrCpy(m_BfPathRead);
    l_BfPath.StrCat(i_BfName);
    l_BfPath.StrCat(m_BfNameExtension);
    OpenBigFileName(l_BfPath);

    // BigFile opening
    l_BfPath.StrCpy(m_BfPathRead);
    l_BfPath.StrCat(i_BfName);
    l_BfPath.StrCat(m_BfExtension);
    (*l_StrAgentHdl)->OpenBigFile(l_BfPath);

    return TRUE;
}

// $SABE: Stubbed out in MASTER
void ClassManager_Z::OpenBigFileName(const String_Z<ARRAY_CHAR_MAX>& i_NameFilePath) { }

Bool OpenBF() {
    if (gData.Cons->GetNbParam() < 2) {
        return FALSE;
    }

    ASSERTL_Z(!gData.ClassMgr->IsBigFileWrite(), "No Bigfile generation in not _NAME_Z_STRING Mode", 379);
    ASSERTL_Z(gData.ClassMgr->IsBigFileRead(), "No ASCII loading MASTERBF Mode", 382);

    Name_Z l_StreamAgentClassName = Name_Z::GetID("StreamAgent_Z");
    if (gData.Cons->GetNbParam() >= 3) {
        l_StreamAgentClassName = Name_Z(gData.Cons->GetStrParam(2));
    }

    return gData.ClassMgr->OpenBigFile(gData.Cons->GetStrParam(1), l_StreamAgentClassName, FALSE);
}
