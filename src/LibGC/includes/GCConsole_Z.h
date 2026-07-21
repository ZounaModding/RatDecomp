#ifndef _GCCONSOLE_Z_H_
#define _GCCONSOLE_Z_H_
#include "Console_Z.h"

class GCConsole_Z;

typedef void (GCConsole_Z::*FolderProcDraw_Z)(Float i_DTime);

class Folder_Z {
public:
    Folder_Z()
        : m_FolderProc(NULL) {
    }

    Char m_Name[128];
    FolderProcDraw_Z m_FolderProc;
};

class GCConsole_Z : public Console_Z {
private:
    DynArray_Z<Folder_Z> m_FolderDA;

public:
    GCConsole_Z();

    virtual ~GCConsole_Z();
    virtual Bool InitConsole();
    virtual void CloseConsole();
    virtual void Update(Float a1);
    virtual void Flush();
    virtual void DisplayStatus(U32 a1, const Char* a2);
    virtual void SaveMessage(const Char* a1);

    void InitConsoleSystem();
    void InitFolders();
    void SwitchFolder(S32 i_Folder);
    void SystemDraw(Float i_DTime);

    void MessageDraw(Float i_DTime) { }

    void ScriptDraw(Float i_DTime) { }

    void FrameRedrawSystem(Bool i_Redraw);
};

#endif
