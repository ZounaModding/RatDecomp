#include "FileHdl_Z.h"
#include "StreamManager_Z.h"
#include "Assert_Z.h"
#include "GCMain_Z.h"
#include <os.h>
// $VIOLET: TODO: Actually implement these in file_io.c
ExternC_Z FILE* fopen(const Char* i_FilePath, const Char* i_Mode);
ExternC_Z int fclose(FILE* i_File);
ExternC_Z size_t fread(void* i_Buffer, size_t i_Size, size_t i_Count, FILE* i_File);
ExternC_Z int fseek(FILE* i_File, unsigned long i_Offset, int i_Origin);
ExternC_Z long ftell(FILE* i_File);
ExternC_Z Char* strstr(const Char* i_String, const Char* i_SubString);

Bool FileHdl_Z::Open(const Char* i_FilePath, U32 i_Flags) {
    ASSERTL_Z(!IsOpened(), "", 6);

    Char l_FileName[128];
    Bool l_Result = GetRealFileName(i_FilePath, l_FileName);
    m_LastError = STR_ERROR_NONE;

    if (i_Flags & STR_READ_ONLY) {
        if (l_Result) {
            m_EntryNum = DVDConvertPathToEntrynum(l_FileName);
            if (m_EntryNum >= 0 && DVDFastOpen(m_EntryNum, &m_DvdFileInfo) == FALSE) {
                Close();
            }
            SetError(!IsOpened() ? STR_ERROR_UNKNOWN : STR_ERROR_NONE);
        }
        else {
            m_File = fopen(l_FileName, "rb");
        }
    }
    else if (i_Flags & STR_WRITE_ONLY) {
        m_File = fopen(l_FileName, "wb");
    }

    m_LastError = !IsOpened() ? STR_ERROR_UNKNOWN : STR_ERROR_NONE;
    return m_LastError == STR_ERROR_NONE;
}

void FileHdl_Z::SetError(U32 i_Error) {
    if (i_Error == STR_ERROR_NONE) {
        return;
    }
    if (i_Error != STR_ERROR_UNKNOWN) {
        m_LastError = i_Error;
        return;
    }

    m_LastError = STR_ERROR_UNKNOWN;

    S32 l_BlockStatus;
    for (;;) {
        l_BlockStatus = DVDGetCommandBlockStatus(&m_DvdFileInfo.cb);
        if (l_BlockStatus != DVD_STATE_BUSY && l_BlockStatus != DVD_STATE_WAITING) break;
    }

    switch (l_BlockStatus) {
        case DVD_STATE_NO_DISK:
            m_LastError = STR_ERROR_NO_DISK;
            break;
        case DVD_STATE_COVER_OPEN:
            m_LastError = STR_ERROR_COVER_OPEN;
            break;
        case DVD_STATE_WRONG_DISK:
            m_LastError = STR_ERROR_WRONG_DISK;
            break;
        case DVD_STATE_RETRY:
            m_LastError = STR_ERROR_RETRY;
            break;
        case DVD_STATE_FATAL_ERROR:
            m_LastError = STR_ERROR_FATAL;
    }
}

#pragma dont_inline on

Bool FileHdl_Z::WaitIO() {
    for (;;) {
        OSYieldThread();

        S32 l_Status = DVDGetDriveStatus();
        switch (l_Status) {
            case DVD_STATE_NO_DISK:
                m_LastError = STR_ERROR_COVER_OPEN;
                return FALSE;
            case DVD_STATE_COVER_OPEN:
                m_LastError = STR_ERROR_NO_DISK;
                return FALSE;
            case DVD_STATE_WRONG_DISK:
                m_LastError = STR_ERROR_WRONG_DISK;
                return FALSE;
            case DVD_STATE_RETRY:
                m_LastError = STR_ERROR_RETRY;
                return FALSE;
            case DVD_STATE_FATAL_ERROR:
                m_LastError = STR_ERROR_FATAL;
                return FALSE;
        }

        if (l_Status == DVD_STATE_END) {
            return TRUE;
        }
    }
}

#pragma dont_inline reset

S32 FileHdl_Z::Read(void* i_Buffer, S32 i_Size) {
    S32 l_Result;
    if (m_File) {
        l_Result = fread(i_Buffer, 1, (i_Size + 31) & ~31, m_File);
    }
    else {
        S32 l_Size = (i_Size + 31) & ~31;
        if (DVDReadAsyncPrio(&m_DvdFileInfo, i_Buffer, l_Size, (m_CurrentPos + 31) & ~31, NULL, 2) && WaitIO()) {
            m_CurrentPos += l_Size;
            return i_Size;
        }
        return -1;
    }

    if (l_Result < 0) {
        return l_Result;
    }
    return i_Size;
}

U32 FileHdl_Z::GetSize() {
    S32 l_Size;
    if (m_File) {
        U32 l_CurrentPos;
        l_CurrentPos = Seek(0, FILE_SEEK_CUR);
        l_Size = Seek(0, FILE_SEEK_END);
        Seek(l_CurrentPos, FILE_SEEK_START);
        return l_Size;
    }

    l_Size = m_DvdFileInfo.length;
    SetError(l_Size > 0 ? STR_ERROR_NONE : STR_ERROR_UNKNOWN);
    return l_Size;
}

Bool FileHdl_Z::Close() {
    Bool l_Result = TRUE;

    if (IsOpened()) {
        if (m_File) {
            l_Result = !fclose(m_File);
            m_File = NULL;
        }

        if (m_EntryNum >= 0) {
            l_Result = !!DVDClose(&m_DvdFileInfo);
            m_EntryNum = -1;
        }
    }

    m_CurrentPos = 0;
    return l_Result;
}

U32 FileHdl_Z::Seek(S32 i_Offset, S32 i_Origin) {
    if (m_File) {
        if (i_Origin == FILE_SEEK_START) {
            fseek(m_File, i_Offset, SEEK_SET);
        }
        if (i_Origin == FILE_SEEK_CUR) {
            fseek(m_File, i_Offset, SEEK_CUR);
        }
        if (i_Origin == FILE_SEEK_END) {
            fseek(m_File, i_Offset, SEEK_END);
        }
        return ftell(m_File);
    }

    if (DVDSeekAsyncPrio(&m_DvdFileInfo, i_Offset, NULL, 2) && WaitIO()) {
        m_CurrentPos = i_Offset;
        return i_Offset;
    }
    return -1;
}

Bool FileHdl_Z::GetRealFileName(const Char* i_FilePath, Char* o_RealName) {
    S32 l_Length = 0;
    U32 l_FileNameLength;
    Bool l_IsSpecialFile;
    Bool l_IsLongExtension;
    Bool l_IsScriptOrHeader;
    Char* l_FileNameEnd;
    Char* l_Extension;
    Char* l_Found;
    Char* l_RealName = o_RealName;
    while (*i_FilePath) {
        if (*i_FilePath == '\\') {
            *o_RealName = '/';
        }
        else {
            *o_RealName = *i_FilePath;
        }
        l_Length++;
        i_FilePath++;
        o_RealName++;
    }
    l_RealName[l_Length] = 0;
    strupr(l_RealName);

    l_FileNameLength = strlen(l_RealName);
    if (gData.m_GameFlag & FL_GAME_UNK_0x800) {
        if ((gData.m_GameFlag & FL_GAME_UNK_0x400)) {
            if (strstr(l_RealName + (l_FileNameLength -= 4), ".TSC")) {
                return FALSE;
            }
        }
        return TRUE;
    }
    else {
        l_FileNameEnd = &l_RealName[l_FileNameLength];
        l_IsSpecialFile = TRUE;
        l_IsLongExtension = TRUE;
        l_IsScriptOrHeader = TRUE;
        l_Extension = &l_RealName[l_FileNameLength - 4];

        l_Found = strstr(l_Extension, ".TSC");
        if (!l_Found) {
            l_Found = strstr(l_FileNameEnd - 2, ".H");
            if (!l_Found) {
                l_IsScriptOrHeader = FALSE;
            }
        }
        if (!l_IsScriptOrHeader) {
            l_Found = strstr(l_Extension, ".REV");
            if (!l_Found) {
                l_IsLongExtension = FALSE;
            }
        }
        if (!l_IsLongExtension) {
            l_Found = strstr(l_Extension, ".GC");
            if (!l_Found) {
                l_IsSpecialFile = FALSE;
            }
        }
        return !l_IsSpecialFile;
    }
}
