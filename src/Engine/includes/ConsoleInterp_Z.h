#ifndef _CONSOLE_INTERP_Z_H_
#define _CONSOLE_INTERP_Z_H_
#include "DynArray_Z.h"
#include "Manipulator_Z.h"
#include "FileInterp_Z.h"

class StackCommandInterp_Z {
private:
    Char m_Unk[84];
};

class ConsoleInterp_Z : public Manipulator_Z {
public:
    void PushCommand(const Char* i_CommandLine, Bool i_Unk);

    inline S32 NbCommands() const { return m_StackCommIdx; }

    void Start(const Char* i_CommandLine, S32 i_Unk);
    void Start(const Char* i_CommandLine, Char** i_Args, S32 i_Unk);

    DynArray_Z<FileInterp_Z>& GetFileStack() {
        return m_FileStack;
    }

    virtual void Init();
    virtual ~ConsoleInterp_Z();
    virtual void Update(Float a1);

private:
    S32 m_StackCommIdx;
    StackCommandInterp_Z m_StackComm[2048];
    DynArray_Z<FileInterp_Z> m_FileStack;
};

#endif
