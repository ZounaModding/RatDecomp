#ifndef _FILE_Z_H_
#define _FILE_Z_H_
#include "Types_Z.h"
#include "FileHdl_Z.h"
#include <String_Z.h>

#define FL_FILE_RDONLY (U32)(1 << 0)
#define FL_FILE_WRONLY (U32)(1 << 1)
#define FL_FILE_UNK_0x4 (U32)(1 << 2)
#define FL_FILE_UNK_0x8 (U32)(1 << 3)
#define FL_FILE_UNK_0x10 (U32)(1 << 4)
#define FL_FILE_UNK_0x20 (U32)(1 << 5)
#define FL_FILE_UNK_0x40 (U32)(1 << 6)
#define FL_FILE_SKIP_READ (U32)(1 << 7)

class File_Z {
public:
    File_Z();
    ~File_Z();
    void Init(S32 i_FileSize, const Char* i_Data);
    Bool Open(const Char* i_FilePath, U32 i_Flags);
    Bool Read(void* o_Data, U32 i_Size);
    void Write(const void* i_Data, U32 i_Size);
    void Flush();
    void Close();
    U32 GetSize();
    Bool IsEOF();
    Bool ReadString(Char* o_Str);
    Bool ReadLine(Char* o_Line);
    Char ReadChar();
    U32 GetCurPos();
    Bool GotoNextString();
    void SetCurPos(U32 i_Pos);

    void* GetBuf() { return m_Buffer; }

    static Bool DoFileExists(const Char* i_FilePath);

private:
    FileHdl_Z m_Hdl;
    Char* m_Buffer;
    S32 m_Size;
    S32 m_CurPos;
    S32 m_Unk;
    String_Z<512> m_Path;
};

#endif
