#include "DCConsole_Z.h"

void Console_Z::EnableFolder(U32 i_Folder) {
    m_FolderFlag |= i_Folder;
}

void Console_Z::DisableFolder(U32 i_Folder) {
    m_FolderFlag &= ~i_Folder;
}
