#include "FileHdl_Z.h"
#include "StreamManager_Z.h"
#include "Assert_Z.h"
#include "Program_Z.h"
#include <sys/stat.h>
#include <utime.h>

Bool FileHdl_Z::Open(const Char* i_FilePath, U32 i_Flags) {
    ASSERT_Z(!IsOpened(), "");

    Char l_FileName[ARRAY_CHAR_MAX];

    m_LastError = STR_ERROR_NONE;
    m_CurrentPos = 0;

    GetRealFileName(i_FilePath, l_FileName);

    if (i_Flags & STR_READ_ONLY) {
        m_File = fopen(l_FileName, "rb");
    }
    else if (i_Flags & STR_WRITE_ONLY) {
        m_File = fopen(l_FileName, "wb");
    }

    m_LastError = m_File ? STR_ERROR_NONE : STR_ERROR_UNKNOWN;
    return m_LastError == STR_ERROR_NONE;
}

void FileHdl_Z::SetError(U32 i_Error) {
    m_LastError = i_Error;
}

S32 FileHdl_Z::Read(void* i_Buffer, S32 i_Size) {
    if (!m_File || !i_Buffer || i_Size < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    if (i_Size == 0) {
        m_LastError = STR_ERROR_NONE;
        return 0;
    }

    const S32 l_Read = fread(i_Buffer, 1, i_Size, m_File);

    const long l_Position = ftell(m_File);
    if (l_Position >= 0) {
        m_CurrentPos = static_cast<S32>(l_Position);
    }
    else {
        m_CurrentPos += static_cast<S32>(l_Read);
    }

    if (l_Read != static_cast<S32>(i_Size)) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    m_LastError = STR_ERROR_NONE;
    return i_Size;
}

S32 FileHdl_Z::Write(const void* i_Buffer, S32 i_Size) {
    if (!m_File || !i_Buffer || i_Size < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    if (i_Size == 0) {
        m_LastError = STR_ERROR_NONE;
        return 0;
    }

    const S32 l_Written = fwrite(i_Buffer, 1, i_Size, m_File);

    const long l_Position = ftell(m_File);
    if (l_Position >= 0) {
        m_CurrentPos = static_cast<S32>(l_Position);
    }
    else {
        m_CurrentPos += static_cast<S32>(l_Written);
    }

    if (l_Written != static_cast<S32>(i_Size)) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    m_LastError = STR_ERROR_NONE;
    return i_Size;
}

U32 FileHdl_Z::GetSize() {
    if (!m_File) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    const long l_CurrentPosition = ftell(m_File);
    if (l_CurrentPosition < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    if (fseek(m_File, 0, SEEK_END) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    const long l_Size = ftell(m_File);

    if (fseek(m_File, l_CurrentPosition, SEEK_SET) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    m_CurrentPos = static_cast<S32>(l_CurrentPosition);

    if (l_Size < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    m_LastError = STR_ERROR_NONE;
    return static_cast<U32>(l_Size);
}

S32 FileHdl_Z::GetCurPos() {
    if (!m_File) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    const long l_Position = ftell(m_File);
    if (l_Position < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    m_CurrentPos = static_cast<S32>(l_Position);
    m_LastError = STR_ERROR_NONE;

    return static_cast<U32>(l_Position);
}

Bool FileHdl_Z::Close() {
    if (!m_File) {
        m_CurrentPos = 0;
        m_LastError = STR_ERROR_NONE;
        return TRUE;
    }

    FILE* l_File = m_File;
    m_File = NULL;
    m_CurrentPos = 0;

    if (fclose(l_File) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return FALSE;
    }

    m_LastError = STR_ERROR_NONE;
    return TRUE;
}

U32 FileHdl_Z::Seek(S32 i_Offset, S32 i_Origin) {
    if (!m_File) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    S32 l_Whence;

    switch (i_Origin) {
        case FILE_SEEK_START:
            l_Whence = SEEK_SET;
            break;

        case FILE_SEEK_CUR:
            l_Whence = SEEK_CUR;
            break;

        case FILE_SEEK_END:
            l_Whence = SEEK_END;
            break;

        default:
            m_LastError = STR_ERROR_UNKNOWN;
            return static_cast<U32>(-1);
    }

    if (fseek(m_File, i_Offset, l_Whence) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    const long l_Position = ftell(m_File);
    if (l_Position < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return static_cast<U32>(-1);
    }

    m_CurrentPos = static_cast<S32>(l_Position);
    m_LastError = STR_ERROR_NONE;

    return static_cast<U32>(l_Position);
}

void FileHdl_Z::Flush() {
    if (m_File) {
        if (fflush(m_File) != 0) {
            m_LastError = 1;
        }
        else {
            m_LastError = 0;
        }
    }
}

Bool FileHdl_Z::DoFileExists(const Char* i_FilePath) {
    if (!i_FilePath) {
        m_LastError = STR_ERROR_UNKNOWN;
        return FALSE;
    }

    Char l_RealName[128];
    GetRealFileName(i_FilePath, l_RealName);

    struct stat l_Stat;
    if (stat(l_RealName, &l_Stat) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return FALSE;
    }

    if (!S_ISREG(l_Stat.st_mode)) {
        m_LastError = STR_ERROR_UNKNOWN;
        return FALSE;
    }

    m_LastError = STR_ERROR_NONE;
    return TRUE;
}

S32 FileHdl_Z::GetFileDate(const Char* i_FilePath) {
    if (!i_FilePath) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    Char l_RealName[128];
    GetRealFileName(i_FilePath, l_RealName);

    struct stat l_Stat;
    if (stat(l_RealName, &l_Stat) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return -1;
    }

    m_LastError = STR_ERROR_NONE;
    return static_cast<S32>(l_Stat.st_mtime);
}

void FileHdl_Z::SetFileDate(const Char* i_FilePath, S32 i_Date) {
    if (!i_FilePath || i_Date < 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return;
    }

    Char l_RealName[128];
    GetRealFileName(i_FilePath, l_RealName);

    struct utimbuf l_Times;

    struct stat l_Stat;
    if (stat(l_RealName, &l_Stat) == 0) {
        l_Times.actime = l_Stat.st_atime;
    }
    else {
        l_Times.actime = static_cast<time_t>(i_Date);
    }

    l_Times.modtime = static_cast<time_t>(i_Date);

    if (utime(l_RealName, &l_Times) != 0) {
        m_LastError = STR_ERROR_UNKNOWN;
        return;
    }

    m_LastError = STR_ERROR_NONE;
}

static Bool IsScriptFile(const Char* i_Path) {
    const U32 l_Length = strlen(i_Path);

    return l_Length >= 4 && strcmp(i_Path + l_Length - 4, ".TSC") == 0;
}

void FileHdl_Z::GetRealFileName(
    const Char* i_FilePath,
    Char* o_RealName
) {
    Char l_NormalizedPath[ARRAY_CHAR_MAX - 4];

    Char* l_Dst = l_NormalizedPath;

    while (*i_FilePath) {
        Char l_Char = *i_FilePath++;

        if (l_Char == '\\') {
            l_Char = '/';
        }

        if (l_Char >= 'a' && l_Char <= 'z') {
            l_Char -= 'a' - 'A';
        }

        *l_Dst++ = l_Char;
    }

    *l_Dst = '\0';

    const Char* l_Root = gData.m_AliasPath;

    if (
        (gData.m_GameFlag & FL_GAME_SCRIPT_ANY_FS) && IsScriptFile(l_NormalizedPath)
    ) {
        l_Root = "/rd/";
    }

    const Char* l_Path = l_NormalizedPath;

    while (*l_Path == '/') {
        ++l_Path;
    }

    strcpy(o_RealName, l_Root);
    strcat(o_RealName, l_Path);
}
