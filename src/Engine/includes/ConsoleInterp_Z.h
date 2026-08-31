#ifndef _CONSOLE_INTERP_Z_H_
#define _CONSOLE_INTERP_Z_H_
#include "DynArray_Z.h"
#include "Manipulator_Z.h"
#include "FileInterp_Z.h"

struct StackCommandInterp_Z {
    U32 m_CommandName;
    String_Z<CONSOLE_STACK_COMMAND_LEN_MAX> m_CommandLine;
};

class ConsoleInterp_Z : public Manipulator_Z {
public:
    // clang-format off

    virtual void Init();
    virtual ~ConsoleInterp_Z() { }
    virtual void Update(Float i_DeltaTime);

    // clang-format on

    static BaseObject_Z* NewObject() { return NewL_Z(70) ConsoleInterp_Z; }

    inline S32 NbCommands() const { return m_StackCommNb; }

    void Start(const Char* i_CommandLine, S32 i_FileSize);
    void Start(const Char* i_FileName, Char** i_CommandLine, S32 i_ParamCount);
    Bool PushCommand(const Char* i_CommandLine, Bool i_TopOfStack);
    Bool PopCommand(String_Z<CONSOLE_STACK_COMMAND_LEN_MAX>& o_CommandString);
    Bool DoCommand(U32 i_Depth, Char* i_CommandString, S32& io_CommentCurrentDepth, Bool& o_Skipped, FileInterp_Z* i_ReplacementFileInterp = NULL);
    void End();

    DynArray_Z<FileInterp_Z>& GetFileStack() {
        return m_FileStack;
    }

private:
    S32 m_StackCommNb;
    StackCommandInterp_Z m_StackComm[CONSOLE_MAX_STACK_COMMAND];
    DynArray_Z<FileInterp_Z> m_FileStack;
};

#endif
