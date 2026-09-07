#include "DCConsole_Z.h"

DCConsole_Z::DCConsole_Z() {
    m_PopupMenu = New_Z PopupMenu_Z;
}

void Console_Z::EnableFolder(U32 i_Folder) {
    m_FolderFlag |= i_Folder;
}

void Console_Z::DisableFolder(U32 i_Folder) {
    m_FolderFlag &= ~i_Folder;
}
