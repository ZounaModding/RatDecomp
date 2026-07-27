#ifndef _FILE_HDL_Z_H_
#define _FILE_HDL_Z_H_
#include "Types_Z.h"
#include "ansi_files.h"
#include "dvd.h"

#define FILE_SEEK_START 0
#define FILE_SEEK_CUR 1
#define FILE_SEEK_END 2

enum GCStreamError {
    STR_ERROR_NO_DISK = 2,
    STR_ERROR_WRONG_DISK = 3,
    STR_ERROR_RETRY = 4,
    STR_ERROR_COVER_OPEN = 5,
    STR_ERROR_FATAL = 6,
};

enum Format {
    STR_READ_ONLY = 0x1,
    STR_WRITE_ONLY = 0x2,
    STR_READWRITE = 0x4,
    STR_ASCII = 0x8,
    STR_BINARY = 0x10
};

class FileHdl_Z {
public:
    FileHdl_Z() {
        m_File = NULL;
        m_EntryNum = -1;
        m_CurrentPos = 0;
    }

    ~FileHdl_Z() {
        Close();
    }

    Bool Open(const Char* i_FilePath, U32 i_Flags);
    void SetError(U32 i_Error);
    Bool WaitIO();
    S32 Read(void* i_Buffer, S32 i_Size);
    S32 Write(const void* i_Buffer, S32 i_Size);
    U32 GetSize();
    S32 GetCurPos();
    Bool Close();
    Bool DoFileExists(const Char* i_FilePath);
    S32 GetFileDate(const Char* i_FilePath);
    void SetFileDate(const Char* i_FilePath, S32 i_Date);
    U32 Seek(S32 i_Offset, S32 i_Origin);
    static Bool GetRealFileName(const Char* i_FilePath, Char* o_RealName);
    void CheckDisc(Bool i_Unk);

    void Flush() { }

    Bool IsOpened() const {
        return ((m_EntryNum != -1) || m_File);
    }

    U32 GetLastError() const {
        return m_LastError;
    }

    const DVDFileInfo& GetDvdFileInfo() const { return m_DvdFileInfo; }

private:
    DVDFileInfo m_DvdFileInfo;
    S32 m_CurrentPos;
    S32 m_EntryNum;
    FILE* m_File;
    U32 m_LastError;
};

#endif
