#ifndef _GCCONSOLE_Z_H_
#define _GCCONSOLE_Z_H_
#include "Console_Z.h"

class GCConsole_Z;

typedef void (GCConsole_Z::*FolderProcDraw_Z)(Float i_DTime);

class Folder_Z {
public:
    String_Z<128> m_Name;
    FolderProcDraw_Z m_FolderProc;
};

class GCConsole_Z : public Console_Z {
private:
    DynArray_Z<Folder_Z> m_FolderDA;

public:
    GCConsole_Z();

    virtual ~GCConsole_Z();
    virtual U32 InitConsole();
    virtual void CloseConsole();
    virtual void Update(Float a1);
    virtual void Flush();
    virtual void DisplayStatus(U32 a1, const Char* a2);
    virtual void SaveMessage(const Char* a1);
};

#endif
