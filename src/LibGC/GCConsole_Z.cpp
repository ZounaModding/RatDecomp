#include "GCConsole_Z.h"
#include "LowMemory_Z.h"

GCConsole_Z::GCConsole_Z() {
    m_PopupMenu = NewL_Z(13) PopupMenu_Z;
}

Bool GCConsole_Z::InitConsole() {
    InitFolders();
    EnableFlag(FL_CONSOLE_UNK_0x40);
    return TRUE;
}
