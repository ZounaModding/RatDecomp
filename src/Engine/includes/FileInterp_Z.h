#ifndef _FILE_INTERP_Z_H_
#define _FILE_INTERP_Z_H_
#include "BaseStream_Z.h"
#include "File_Z.h"
#include "Console_Z.h"

class ConsoleInterp_Z;

class FileInterp_Z : public BaseStream_Z {
    friend class ConsoleInterp_Z;

public:
    File_Z* GetFile() {
        return &m_File;
    }

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined);
    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined);
    Bool ReplaceParams(Char* i_CommandString, String_Z<CONSOLE_STATIC_COMMAND_LEN_MAX>& o_ReplacedCommandString);

private:
    File_Z m_File;
    S32 m_LinesOrCommandsParsed;
    S32 m_CommentCurrentDepth;
    S32 m_StackIndex;
    S32 m_Flags;
    String_Z<64> m_FileName;
    S32 m_ParamCount;
    String_Z<64> m_Params[16];
};

#endif
