#include "GCConsole_Z.h"

GCConsole_Z::GCConsole_Z() {
    m_PopupMenu = NewL_Z(13) PopupMenu_Z;
}

Bool GCConsole_Z::InitConsole() {
    InitFolders();
    EnableFlag(FL_CONSOLE_UNK_0x40);
    return TRUE;
}

void GCConsole_Z::CloseConsole() {
}

void GCConsole_Z::SaveMessage(const Char* a1) {
}

void GCConsole_Z::Flush() {
}
