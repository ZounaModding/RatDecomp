#include "ClassManager_Z.h"
#include "ABC_ScriptManager.h"
#include "StreamAgent_Z.h"
#include "Console_Z.h"
#include "KSys_Z.h"
#include "UnPack_Z.h"
#include "XRamManager_Z.h"

void ClassManager_Z::OpenBigFileStream(const Char* a1) {
    // TODO: Match
}

void ClassManager_Z::CloseBigFileStream() {
    // TODO: Match
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

void ClassManager_Z::CloseBigFile() {
    StreamAgent_Z* l_StrAgent = *gData.ScriptMgr->GetStreamAgent();
}

// TODO: Finish matching
Bool ClassManager_Z::LoadResourceLink(BigFileRsc_Z& io_Resource) {
    S32 l_CurRscSize;
    BigFileRscHeader_Z& l_RscHeader = *io_Resource.m_Header;
    Name_Z& l_RscName = io_Resource.m_Header->m_Name;
    Name_Z& l_ClassName = io_Resource.m_Header->m_ClassName;
    S32 l_HdlIdx = IsResourceRef(io_Resource.m_Header->m_Name.GetID());
    if (l_HdlIdx >= 0) {
        ASSERTLE_Z(m_HandleRecDA[l_HdlIdx].m_ObjPtr, "", 93, "HandleRecDA[result].ObjPtr");
        ASSERTLE_Z(m_HandleRecDA[l_HdlIdx].m_Name == l_RscName, "", 94, "HandleRecDA[result].Name==pRscName");
        return FALSE;
    }

    const BaseObject_ZHdl& l_NewObject = NewResource(l_ClassName, l_RscName);
    io_Resource.m_ObjPtr = m_HandleRecDA[l_NewObject.GetID()].m_ObjPtr;

    U8* l_DataStartPtr = (U8*)(io_Resource.m_Header + 1);
    U8* l_DataPtr = l_DataStartPtr;
    io_Resource.m_ObjPtr->LoadLinks((void**)&l_DataPtr);
    l_CurRscSize = (S32)l_DataPtr - (S32)l_DataStartPtr;

    ASSERTLE_Z(l_CurRscSize == l_RscHeader.m_LinkSize, "", 111, "CurRscSize==RscHeader.RscLinkSize");

    return TRUE;
}

void ClassManager_Z::LoadResourceData(BigFileRsc_Z& io_Resource) {
    S32 l_CurRscSize;
    BigFileRscHeader_Z& l_RscHeader = *io_Resource.m_Header;
    S32 l_HdlIdx = IsResourceRef(io_Resource.m_Header->m_Name.GetID());
    ASSERTLE_Z(l_HdlIdx >= 0, "LoadResourceData Unknown Object", 137, "result>=0");

    HandleRec_Z& l_HandleRec = m_HandleRecDA[l_HdlIdx];
    U8 l_Flag = m_ClassList[l_HandleRec.m_ClassID].m_Flag;
    io_Resource.m_ObjPtr = l_HandleRec.m_ObjPtr;

    U8* l_DataStartPtr = (U8*)(io_Resource.m_Header + 1);
    l_DataStartPtr = l_DataStartPtr + l_RscHeader.m_LinkSize;
    U8* l_DataPtr = l_DataStartPtr;

    if (l_RscHeader.m_CompressedSize) {
        UnPack_Z l_UnPack((const U8*)l_DataStartPtr, (U8*)io_Resource.m_Data);
        l_DataStartPtr = io_Resource.m_Data;
        l_DataPtr = l_DataStartPtr;
        io_Resource.m_ObjPtr->Load((void**)&l_DataPtr);
        l_CurRscSize = (S32)l_DataPtr - (S32)l_DataStartPtr;
        ASSERTLE_Z(l_CurRscSize == l_RscHeader.m_DataSize, "", 155, "CurRscSize==RscHeader.RscDataSize");
        return;
    }

    HdlID l_Id;
    l_Id.Ref.Key = (Char)l_HandleRec.m_Key;
    l_Id.Ref.ID = l_HdlIdx;
    if (l_Flag & FL_CLASS_TYPE_STREAM_XRAM) {
        S16 l_XRamBlock = gData.XRamMgr->AllocBlock(AlignUp_Z(l_RscHeader.m_DataSize, 32), l_Id.GblID, -1);
        l_HandleRec.m_xRamBlock = l_XRamBlock;
        if (l_XRamBlock >= 0) {
            gData.XRamMgr->Write(l_DataPtr, gData.XRamMgr->GetBlockPtr(l_HandleRec.m_xRamBlock), AlignUp_Z(l_RscHeader.m_DataSize, 32), FALSE);
            l_HandleRec.m_Flag |= HandleRec_Z::RSC_XRAM;
            return;
        }
    }

    if (l_Flag & FL_CLASS_TYPE_STREAM && io_Resource.m_Flag != BF_RSC_FLAG_NONE) {
        l_HandleRec.m_Flag |= HandleRec_Z::RSC_STR;
        return;
    }

    l_HandleRec.m_Flag &= ~HandleRec_Z::RSC_STR_LOADING;
    l_HandleRec.m_Flag |= HandleRec_Z::RSC_STR_DONE;

    io_Resource.m_ObjPtr->Load((void**)&l_DataPtr);
    l_CurRscSize = (S32)l_DataPtr - (S32)l_DataStartPtr;
    ASSERTLE_Z(l_CurRscSize == l_RscHeader.m_DataSize, "", 187, "CurRscSize==RscHeader.RscDataSize");
}

// TODO: Finish matching
Bool ClassManager_Z::LoadResource(BigFileRsc_Z& io_Resource) {
    if (!LoadResourceLink(io_Resource)) {
        return FALSE;
    }

    LoadResourceData(io_Resource);

    return TRUE;
}

// $SABE: Stubbed out in MASTER
void ClassManager_Z::OpenBigFileName(const String_Z<ARRAY_CHAR_MAX>& i_NameFilePath) { }

void ClassManager_Z::LoadName(Name_Z& o_Name, void** i_Data) {
    LOAD_Z(o_Name.m_ID);
    SetNameStrFromID(o_Name);
}

void ClassManager_Z::GetNameFromId(U32 i_ID, Name_Z& o_Name) {
    o_Name.m_ID = i_ID;
}

// $SABE: Stubbed out in MASTER
void ClassManager_Z::SetNameStrFromID(Name_Z& o_Name) { }

const String_Z<ARRAY_CHAR_MAX>& ClassManager_Z::GetNameStrFromId(const Name_Z& i_Name) const {
    // TODO: Match
    return m_DefaultNameString;
}

void ClassManager_Z::LoadLink(BaseObject_ZHdl& i_Hdl, void** i_Data) {
    LOAD_Z(i_Hdl.m_RealID.GblID);
}

void ClassManager_Z::LoadLinkId(void* i_ID, void** i_Data) {
    LOADBYTES_Z(i_ID, sizeof(S32));
}

Bool ClassManager_Z::UpdateLinkFromId(BaseObject_ZHdl& io_Hdl, S32 i_ID) {
    io_Hdl.m_RealID.GblID = i_ID;
    return UpdateLink(io_Hdl);
}

Bool ClassManager_Z::UpdateLink(BaseObject_ZHdl& io_Hdl) {
    Name_Z l_Name = io_Hdl.m_RealID.GblID;
    S32 l_HdlIdx = IsResourceRef(l_Name.m_ID);
    if (l_HdlIdx >= 0) {
        ASSERTLE_Z(m_HandleRecDA[l_HdlIdx].m_ObjPtr, "", 291, "HandleRecDA[result].ObjPtr");
        ASSERTLE_Z(l_Name.m_ID == m_HandleRecDA[l_HdlIdx].m_Name.GetID(), "", 292, "Name.ID==HandleRecDA[result].Name.GetID()");
        io_Hdl.m_RealID.Ref.ID = l_HdlIdx;
        io_Hdl.m_RealID.Ref.Key = m_HandleRecDA[l_HdlIdx].m_Key;
        return TRUE;
    }
    io_Hdl.m_RealID.GblID = 0;
    return FALSE;
}

void ClassManager_Z::UpdateLinkPtr(void* io_Ptr) {
    Name_Z l_Name = *(S32*)io_Ptr;
    *(BaseObject_Z**)io_Ptr = NULL;
    S32 l_HdlIdx = IsResourceRef(l_Name.m_ID);
    if (l_HdlIdx >= 0) {
        ASSERTLE_Z(m_HandleRecDA[l_HdlIdx].m_ObjPtr, "", 316, "HandleRecDA[result].ObjPtr");
        *(BaseObject_Z**)io_Ptr = m_HandleRecDA[l_HdlIdx].m_ObjPtr;
    }
}

BaseObject_ZHdl ClassManager_Z::GetResourceObject(const Char* i_RscName) {
    String_Z<264> l_RscNameString;
    l_RscNameString.StrCpy(i_RscName);

    S32 l_Len = gData.m_DBPath.StrLen();
    if (l_Len && l_RscNameString.StrnCmp(gData.m_DBPath, l_Len) == 0) {
        PathSysToDB(i_RscName, l_RscNameString);
    }
    else {
        Char* l_Char = l_RscNameString.StrChr('\\');
        while (l_Char) {
            *l_Char = KSYS_DIRECTORY_CHAR;
            l_Char = strchr(l_Char, '\\');
        }
    }

    Name_Z l_RscName(l_RscNameString.Get());

    S32 l_HdlIdx = IsResourceRef(l_RscName.m_ID);
    if (l_HdlIdx >= 0) {
        return m_HandleRecDA[l_HdlIdx].m_ObjPtr->GetHandle();
    }
    return HANDLE_NULL;
}

BaseObject_ZHdl ClassManager_Z::AssumeGetResourceObject(const Char* i_RscName) {
    BaseObject_ZHdl l_Hdl = GetResourceObject(i_RscName);
    BaseObject_Z* l_ObjPtr = l_Hdl;

    if (l_ObjPtr) {
        return l_Hdl;
    }

    String_Z<ARRAY_CHAR_MAX> l_RscNameString;
    l_RscNameString.StrCpy(i_RscName);

    S32 l_Len = gData.m_DBPath.StrLen();
    if (l_Len && l_RscNameString.StrnCmp(gData.m_DBPath, l_Len) == 0) {
        PathSysToDB(i_RscName, l_RscNameString);
    }
    else {
        Char* l_Char = l_RscNameString.StrChr('\\');
        while (l_Char) {
            *l_Char = KSYS_DIRECTORY_CHAR;
            l_Char = strchr(l_Char, '\\');
        }
    }

    Name_Z l_RscName(l_RscNameString.Get());

    S32 l_HdlIdx = IsResourceRef(l_RscName.m_ID);
    if (l_HdlIdx >= 0) {
        l_Hdl = m_HandleRecDA[l_HdlIdx].m_ObjPtr->GetHandle();
    }
    return l_Hdl;
}

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

Bool OpenBFS() {
    // TODO: Match
    return FALSE;
}

Bool ForceBF() {
    // TODO: Match
    return FALSE;
}

Bool CloseBF() {
    gData.ClassMgr->SetBigFileOpened(FALSE);
    gData.ClassMgr->CloseBigFile();
    return TRUE;
}
