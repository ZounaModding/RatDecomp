#ifndef _FILE_Z_H_
#define _FILE_Z_H_
#include "Types_Z.h"
#include "FileHdl_Z.h"
#include "String_Z.h"

class File_Z {
public:
    File_Z();
    ~File_Z();
    void Init(S32 i_Unk, const Char* i_FileName);
    Bool Open(const Char* i_FileName, U32 i_Flags);
    Bool Write(const void* i_Data, U32 i_Size);
    void Flush();
    void Close();
    U32 GetSize();

    void* GetBuf() { return m_FileBuf; }

private:
    FileHdl_Z m_FileHdl;
    void* m_FileBuf;
    S32 m_FileSize;
    S32 m_FilePos;
    S32 m_Unk;
    String_Z<512> m_FileName;
};

#endif
