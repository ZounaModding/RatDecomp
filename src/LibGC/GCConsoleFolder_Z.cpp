#include "GCConsole_Z.h"

void GCConsole_Z::InitFolders() {
    m_FolderDA.SetSize(4);

    sprintf(m_FolderDA[0].m_Name, "System Infos");
    m_FolderDA[0].m_FolderProc = &GCConsole_Z::SystemDraw;

    sprintf(m_FolderDA[1].m_Name, "Message");
    m_FolderDA[1].m_FolderProc = &GCConsole_Z::MessageDraw;

    sprintf(m_FolderDA[2].m_Name, "Script");
    m_FolderDA[2].m_FolderProc = &GCConsole_Z::ScriptDraw;

    InitConsoleSystem();
    SwitchFolder(0);
}

void GCConsole_Z::SwitchFolder(S32 i_Folder) {
    for (S32 i = 0; i < m_FolderDA.GetSize(); i++) {
        if (i != i_Folder) {
            DisableFolder(1 << i);
        }
    }

    if (i_Folder >= 0) {
        EnableFolder(1 << i_Folder);
    }
}

void Console_Z::EnableFolder(U32 i_Folder) {
    m_FolderFlag |= i_Folder;
}

void Console_Z::DisableFolder(U32 i_Folder) {
    m_FolderFlag &= ~i_Folder;
}

void GCConsole_Z::Update(Float i_DTime) {
    Console_Z::Update(i_DTime);

    for (S32 i = 0; i < m_FolderDA.GetSize(); i++) {
        if ((m_FolderFlag & (1 << i)) && m_FolderDA[i].m_FolderProc) {
            (this->*m_FolderDA[i].m_FolderProc)(i_DTime);
        }
    }
}
