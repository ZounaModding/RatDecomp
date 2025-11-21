#include "Console_Z.h"
#include "BaseObject_Z.h"
#include "GCMain_Z.h"
#include "Handle_Z.h"
#include "Memory_Z.h"
#include "PopupMenu_Z.h"
#include "Types_Z.h"
#include "ConsoleInterp_Z.h"
#include "Renderer_Z.h"
#include "File_Z.h"
#include "ConsoleInterp_ZHdl.h"
Extern_Z GCGlobals gData;

Console_Z::Console_Z()
    : m_UnkS32List_0x1384()
    , m_UnkBool_0x13ac() {
    m_UnkU32_0x23bc = 0;
    m_MaxComm = 0;
    m_UnkU32_0x23bc = 0;
    m_CommIndex = -1;
    m_CommandList = NULL;
    m_PopupMenu = NULL;
    m_Depth = 0;
    m_UnkBool_0x6c70 = FALSE;
    m_Flag = 0;
    m_FolderFlag = (1 << ConsoleMessage);
    m_UnkU32_0x1394 = 0;
    m_UnkU32_0x1398 = 0;
    m_UnkU32_0x139c = 0;
    m_UnkU32_0x13a4 = 0;
    m_UnkU32_0x13a8 = 0;
    m_Interp = NULL;
    m_ConsoleText = NULL;
    m_CommandNbVar = 0;
    m_StackNbVar = 0;
}

Console_Z::~Console_Z() {
    Command_Z* l_Curr = m_CommandList;
    while (l_Curr != NULL) {
        Command_Z* l_Next = l_Curr->m_Prev;
        delete l_Curr;
        l_Curr = l_Next;
    }
    S32 l_Unk = 0;
    delete m_ConsoleText;
    delete m_PopupMenu;
    m_ConsoleText = NULL;
    m_Interp = NULL;
}

void Console_Z::Init() {
    if (m_ConsoleText == NULL) {
        m_ConsoleText = NewL_Z(67) Char[0x6000];
        memset(m_ConsoleText, 0, 0x6000);
        SetVar("_MASTER");
        SetVar("_BIGFILE");
    }
}

void Console_Z::MarkHandles() {
    if (m_Interp != NULL) {
        m_Interp->MarkHandles();
    }
}

void Console_Z::DisableFlag(U32 i_Flag) {
    ASSERTLE_Z(!((m_Flag & FL_CONS_PAUSED) && (i_Flag & FL_CONS_PAUSED) && gData.ClassMgr->IsBigFileOpened()), "", 95, "! (Flag&(1<<5) && _Flag&(1<<5) && gData.ClassMgr->IsBigFileOpened())");
    m_Flag &= ~i_Flag;
}

void Console_Z::Update(Float i_DeltaTime) {
    if (m_PopupMenu != NULL) {
        m_PopupMenu->Update(i_DeltaTime);
    }
}

void Console_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    if (m_PopupMenu != NULL) {
        m_PopupMenu->Show(0x10, 0x20);
    }
}

void Console_Z::InterpFileBuffer(const Char* i_FileBuffer, S32 i_Unk) {
    if (m_Interp != NULL) {
        m_Interp->Start(i_FileBuffer, i_Unk);
    }
}

void* Console_Z::GetCurrentInterpBuffer() {
    U32 l_Size;
    File_Z* l_File;
    if (m_Interp == NULL) {
        return NULL;
    }
    DynArray_Z<FileInterp_Z>& fs = m_Interp->GetFileStack();
    if (fs.GetSize() != 0) {
        S32 l_Temp = fs.GetSize();
        l_File = fs[--l_Temp].GetFile();
    }
    else {
        l_File = NULL;
    }

    if (l_File != NULL) {
        return l_File->GetBuf();
    }

    return NULL;
}

U32 Console_Z::GetCurrentInterpSize() {
    U32 l_Size;
    File_Z* l_File;
    if (m_Interp == NULL) {
        return 0;
    }
    DynArray_Z<FileInterp_Z>& fs = m_Interp->GetFileStack();
    if (fs.GetSize() != 0) {
        int temp = fs.GetSize();
        l_File = fs[--temp].GetFile();
    }
    else {
        l_File = NULL;
    }

    if (l_File != NULL) {
        return l_File->GetSize();
    }

    return 0;
}

Bool Help() {
    gData.Cons->DisplayHelp();
    return TRUE;
}

Bool Pause() {
    gData.Cons->EnableFlag(FL_CONS_PAUSED);

    return TRUE;
}

void Console_Z::InterpFile() {
    if (gData.Cons->GetInterp() == NULL) {
        m_Interp = (ConsoleInterp_ZHdl)gData.ClassMgr->NewObject(Name_Z::GetID("ConsoleInterp_Z", 0), Name_Z::GetID("ConsoleInterp", 0));
        m_Interp->Deactivate();
    }
    GetInterp()->Start(m_StrParam[1], &m_StrParam[0], GetNbParam());
}

Bool Source() {
    gData.Cons->InterpFile();
    return TRUE;
}

Bool BSource() {
    gData.Cons->EnableFlag(FL_CONS_UNK0x10);
    gData.Cons->InterpFile();
    return TRUE;
}

void Dummy() {
    gData.Cons->CloseConsole();
}
